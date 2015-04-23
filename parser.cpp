#include "parser.h"
#include <cstdlib>

// this is the function you need. Takes the string and returns the BoolExpr result
// it uses two messy functions
Gecode::BoolExpr Parser::evaluate_constraint(string input) {
	RpnQueue_t* rpn = input_to_RPN(input); // form the queue with postfix notation
	EvalObj* result = evaluate_RPN(rpn); // get the result
	//now we need to convert this result

	// try convert the result to boolean expression
	EvalObjBoolExpr* boolRes = dynamic_cast<EvalObjBoolExpr*> (result); // this class will contain BoolExpr
	if (!boolRes)
		throw EvalError("The result is not boolean!");
	Gecode::BoolExpr bexpr = boolRes->value;
	delete rpn;
	delete result;
	return (bexpr);
}

// read the input string and form the RnpQueue
RpnQueue_t* Parser::input_to_RPN(string input) {
	stack<string> operatorStack;
	RpnQueue_t* rpnQueue = new RpnQueue_t();
	bool lastOp = true; // indicates if the last element read was operator
	string::iterator iter=input.begin();
	// skip spaces at the beginning
	while (iter!=input.end() && isspace(*iter)) 
		iter++;
	// read string and extract operations, numbers and variables
	while (iter!=input.end()) {
		if (isdigit(*iter)) { // it is an integer number
			if (!lastOp)
				throw ParseError("Unexpected term: two terms without operator");
			string numstr("");
			// read the whole number
			while (isdigit(*iter))
				numstr.push_back(*iter++);
			// push the string with the number to the output
			rpnQueue->push(numstr);
			lastOp = false;
		} else if (isalpha(*iter)) { // it is a variable
			if (!lastOp)
				throw ParseError("Unexpected term: two terms without operator");
			string varstr("");
			// read the whole variable name (contains letters and digits)
			while (isalnum(*iter))
				varstr.push_back(*iter++);
			rpnQueue->push(varstr);
			lastOp = false;
		} else { // it is an operator or parenthesis
			switch (*iter) {
			case '(':
				if (!lastOp)
					throw ParseError("Unexpected '(' in the expression");
				operatorStack.push("(");
				iter++;
				break;
			case ')':
				if (lastOp) // previous one was operator, so we don't expect ')' after that
					throw ParseError("Unexpected ')' in the expression");
				while (!operatorStack.empty() && operatorStack.top()!="(") { // push all operators from the stack to the output until we encounter "("
					rpnQueue->push(operatorStack.top()); // pust top operator to the output
					operatorStack.pop(); // delete top operator from the stack
				}
				if (operatorStack.empty())
					throw ParseError("Unexpected ')' in the expression");
				operatorStack.pop(); // delete "(" from the stack, we don't need it anymore
				iter++;
				break;
			default: // it is an operator
				string opstr("");
				// read the whole operator
				while (iter!=input.end() && !isspace(*iter) && !isalnum(*iter) && *iter!='(' && *iter!=')')
					opstr.push_back(*iter++);
				if (opMap.find(opstr)==opMap.end())
					throw ParseError("Unknown operator: '" + opstr + "'");
				if (lastOp) {
					if (!dynamic_cast<UnaryOperation<EvalObj*>*>(opMap[opstr])) // if the operation is not unary
						throw ParseError("The operator '" + opstr + "' requires two arguments");
				}
				// now we can push all the operators with higher precedence (with smaller value of 'order') to the output
				while (!operatorStack.empty() && (opMap[opstr]->order >= opMap[operatorStack.top()]->order)) {
					rpnQueue->push(operatorStack.top());
					operatorStack.pop();
				}
				// now push the current operator to output
				operatorStack.push(opstr);
				lastOp = true;
			}
		}

		// skip spaces
		while (iter!=input.end() && isspace(*iter)) 
			iter++;
	}
	// now we finished reading input, so we just push all the operators that are currently on stack to output
	while (!operatorStack.empty()) {
		if (operatorStack.top()=="(") // this is the end, so there should not be any '('; if there are, it is an error
			throw ParseError("Unbalanced parenthesis '('");
		rpnQueue->push(operatorStack.top());
		operatorStack.pop();
	}
	// done!
	return rpnQueue;
}

// evaluate the expression presented in the postfix form
EvalObj* Parser::evaluate_RPN(RpnQueue_t* rpn) {
	stack<EvalObj*> evalStack;
	while(!rpn->empty()) {
		string current = rpn->front(); // read current element
		rpn->pop();
		char first_char = *(current.begin());
		if (isdigit(first_char)) { // this is a constant integer
			evalStack.push(new EvalObjInt(atoi(current.c_str())));
		} else if (isalpha(first_char)) { // this is a variable
			if (current.length()<3)
				throw EvalError("Unknown variable: '" + current + "'");
			string::iterator iter = current.begin();
			first_char = toupper(first_char);
			char second_char = toupper(*++iter);
			char third_char = toupper(*++iter);
			if ((first_char!='X') || (second_char!='S' && second_char!='T') || !isdigit(third_char))
				throw EvalError("Unknown variable: '" + current);
			string numstr("");
			// reading the number after XS or XT
			while(iter!=current.end()) {
				numstr.push_back(*iter++);
			}
			int num = atoi(numstr.c_str());
			if (second_char=='S') // the variable has name XS
				evalStack.push(new EvalObjIntVar(avxs[num]));
			else // the variable has name XT
				evalStack.push(new EvalObjIntVar(avxt[num]));
		} else { // this is an operator
			// check if it is binay or unary
			BinaryOperation<EvalObj*>* bin_op = dynamic_cast<BinaryOperation<EvalObj*>*>(opMap[current]);
			UnaryOperation<EvalObj*>* un_op = dynamic_cast<UnaryOperation<EvalObj*>*>(opMap[current]);
			if (un_op) { // this is an unary operation
				if (evalStack.size()<1)
					throw EvalError("Argument for '" + current + "' expected");
				// taking the value from stack
				EvalObj* arg = evalStack.top(); evalStack.pop();
				// evaluating unary operator and pushing the result on stack
				evalStack.push(un_op->eval_un(arg));
				delete arg;
			} else if (bin_op) { // binary operation
				if (evalStack.size()<2)
					throw EvalError("Two arguments for '" + current + "' expected");
				// taking two arguments from the stack
				EvalObj* right = evalStack.top(); evalStack.pop();
				EvalObj* left = evalStack.top(); evalStack.pop();
				// evaluating and pushing on the stack
				evalStack.push(bin_op->eval_bin(left,right));
				delete left;
				delete right;
			} else { // this should not happen, but for completeness
				throw EvalError("Unknown type of operator '" + current + "'");
			}
		}
	}
	if (evalStack.size()!=1)
		throw EvalError("Syntax error: no more operators to apply");
	// if we are here then everything went okay --> the result is on the stack
	return(evalStack.top());
}