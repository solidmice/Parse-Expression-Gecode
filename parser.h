#ifndef __PARSER_H_INCLUDED__
#define __PARSER_H_INCLUDED__

#include <stack>
#include <queue>
#include <string>

#include "operations.h"
#include "evalobj.h"

using namespace std;

typedef queue<string> RpnQueue_t;

// the class that performs parsing and evaluation
// you should firstly transform the input string into the Queue of terms and operations which represents the postfix notation
// you use input_to_RPN for that (takes string - gives a pointer to the queue of strings)
// then you evaluate the RPN queue with 'evaluate_RPN'. it takes the queue and gives the result as EvalObj*
// to extract the actual result you need to cast the abstract EvalObj* to EvalObjBoolExpr*, and then take the saved BoolExpr value

// the whole procedure is implemented in 'evaluate_constraint'. It takes the string and returns BoolExpr (as you need)

// the parser can actually get not only boolean answers, but of any used type (int, IntVar and so on)
// so if you need these results, you need to make the convertion similarly to that done in 'evaluate_constraint()'
// see parser.cpp for details
class Parser {
private:
	OperationsMap_t opMap;
	Gecode::IntVarArray avxs; // NO CHECKING FOR BOUNDARIES! If you have XS100000 in the input string, the parser will just take
	Gecode::IntVarArray avxt; // avxs[100000] which can lead to errors
public:
	RpnQueue_t* input_to_RPN(string input);
	EvalObj* evaluate_RPN(RpnQueue_t* rpn);
public:
	Parser(Gecode::IntVarArray xs=Gecode::IntVarArray(), Gecode::IntVarArray xt=Gecode::IntVarArray(), OperationsMap_t om=DefOperationsMap()): opMap(om), avxs(xs), avxt(xt) {}
	Gecode::BoolExpr evaluate_constraint(string input);
};

// the class that is thrown when we have an error during parsing
class ParseError {
public:
	string msg;
	ParseError(string m): msg(m) {}
};


#endif