#ifndef __EVALOBJ_H_INCLUDED__
#define __EVALOBJ_H_INCLUDED__

#include <string>
#include <gecode/int.hh>
#include <gecode/minimodel.hh>

using namespace std;

// these objects are used for evaluation
// the base class is EvalObj and all others are its children
// these classes allow to store the results of the evaluation in one stack no matter which value is the actual result
/*
EvalObj; // base class
EvalObjInt; // stores the results of type 'int' (when we read a constant from the input)
EvalObjIntVar; // stores the results of type 'Gecode::IntVar' (when we read a variable)
EvalObjLinIntExpr; // stores the results of type 'Gecode::LinIntExpr' (when we do the operations with variables)
EvalObjLinIntRel; // stores the results of type 'Gecode::LinIntRel' (when we apply the comparison operations)
EvalObjBoolExpr; // stores the results of type 'Gecode::BoolExpr' (when we use the logical operations)
*/

// base class
class EvalObj {
public:
	virtual ~EvalObj(){};
};

// template: child of EvalObj able to store the data of an arbitrary type T
template <class T>
class EvalObjTemplate : public EvalObj {
public:
	T value;
	EvalObjTemplate(T val) : value(val){}
};

typedef EvalObjTemplate<int> EvalObjInt; // stores the results of type 'int' (when we read a constant from the input)
typedef EvalObjTemplate<Gecode::IntVar> EvalObjIntVar; // stores the results of type 'Gecode::IntVar' (when we read a variable)
typedef EvalObjTemplate<Gecode::LinIntExpr> EvalObjLinIntExpr; // stores the results of type 'Gecode::LinIntExpr' (when we do the operations with variables)
typedef EvalObjTemplate<Gecode::LinIntRel> EvalObjLinIntRel; // stores the results of type 'Gecode::LinIntRel' (when we apply the comparison operations)
typedef EvalObjTemplate<Gecode::BoolExpr> EvalObjBoolExpr; // stores the results of type 'Gecode::BoolExpr' (when we use the logical operations)

class EvalError {
public:
	string msg;
	EvalError(string m): msg(m) {}
};

// operations used during evaluation
// the source is in .cpp file

EvalObj* mul(EvalObj*, EvalObj*); // multiplication ('*')
EvalObj* div(EvalObj*, EvalObj*); // division ('/')

EvalObj* add(EvalObj*, EvalObj*); // addition ('+')
EvalObj* sub(EvalObj*, EvalObj*); // subtraction ('-')
EvalObj* id(EvalObj* p1); // identity (unary '+')
EvalObj* neg(EvalObj* p1); // negation (unary '-')

EvalObj* le(EvalObj*, EvalObj*);
EvalObj* gr(EvalObj*, EvalObj*);
EvalObj* leq(EvalObj*, EvalObj*);
EvalObj* geq(EvalObj*, EvalObj*);
EvalObj* eq(EvalObj*, EvalObj*);
EvalObj* neq(EvalObj*, EvalObj*);

EvalObj* not(EvalObj*);
EvalObj* and(EvalObj*, EvalObj*);
EvalObj* or(EvalObj*, EvalObj*);

#endif