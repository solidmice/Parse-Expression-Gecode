#include "operations.h"
#include "evalobj.h"

// Constructor of DefOperationsMap
 DefOperationsMap::DefOperationsMap(): map<string,AbstractOperation*>(){
	int order = 0; // operation precedence

	// for each operation we put the pointer to
	// BinaryOperation or UnaryOperation class (with T= EvalObj*)
	// the constructor takes two arguments: 'order' and 'operation'
	// order says which priority the operation has
	// if order==0, the operation is calculated first,
	// order==1 go next, and so on
	// 'operation' - is a pointer to some function
	// you can just give the name of function, compiler will understand this
	// or you can equivalently write '&mul' instead of 'mul' and so on
		
	// multiplication and division first
	(*this)["*"] = new BinaryOperation<EvalObj*>(order, mul); 
	(*this)["/"] = new BinaryOperation<EvalObj*>(order, div);
	order++;
	// then summation and subtraction (with the unary '+' and '-' as well)
	(*this)["+"] = new BinaryOperation<EvalObj*>(order, add);
	(*this)["-"] = new BinaryOperation<EvalObj*>(order, sub);
	(*this)["~-"] = new UnaryOperation<EvalObj*>(order, id);
	(*this)["~+"] = new UnaryOperation<EvalObj*>(order, neg);
	order++;
	// then comparison operations
	(*this)["<"] = new BinaryOperation<EvalObj*>(order, le);
	(*this)[">"] = new BinaryOperation<EvalObj*>(order, gr);
	(*this)["<="] = new BinaryOperation<EvalObj*>(order, leq);
	(*this)[">="] = new BinaryOperation<EvalObj*>(order, geq);
	(*this)["=="] = new BinaryOperation<EvalObj*>(order, eq);
	(*this)["!="] = new BinaryOperation<EvalObj*>(order, neq);
	order++;
	// then negation
	(*this)["~!"] = new UnaryOperation<EvalObj*>(order, not);
	order++;
	// then AND
	(*this)["&&"] = new BinaryOperation<EvalObj*>(order, and);
	order++;
	// then OR
	(*this)["||"] = new BinaryOperation<EvalObj*>(order, or);
	order++;
	(*this)["("] = new UnaryOperation<EvalObj*>(order,NULL);
}

 DefOperationsMap::~DefOperationsMap() {
	 for (map::iterator iter=this->begin(); iter!=this->end(); iter++) {
		 delete (*iter).second;
	 }
 }