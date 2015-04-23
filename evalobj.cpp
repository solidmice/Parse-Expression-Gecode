#include "evalobj.h"

// these are the operations used during evaluation
// the arguments are all pointers to abstract type EvalObj*
// this allows to use the same type of arguments for operations while performing operations over different types of variables

// NOTE: notation of all operations is given in DefOperationsMap (see operations.h and operations.cpp)
// here the notation is used only for error messages

// multiplication ('*')
EvalObj* mul(EvalObj* p1, EvalObj* p2) {
	// try to cast each of the parameters to three types which support arithmetical functions
	// the one which is not NULL is the actual type of the stored class

	// try int
	const EvalObjInt* par1 = dynamic_cast<const EvalObjInt*>(p1);
	const EvalObjInt* par2 = dynamic_cast<const EvalObjInt*>(p2);
	// try IntVar
	const EvalObjIntVar* par11 = dynamic_cast<const EvalObjIntVar*>(p1);
	const EvalObjIntVar* par22 = dynamic_cast<const EvalObjIntVar*>(p2);
	// try LinIntExpr
	const EvalObjLinIntExpr* par111 = dynamic_cast<const EvalObjLinIntExpr*>(p1);
	const EvalObjLinIntExpr* par222 = dynamic_cast<const EvalObjLinIntExpr*>(p2);

	// if all three variants don't fit to at least one parameter, we can't continue, it's an error
	if ((par1==NULL && par11==NULL && par111==NULL) || (par2==NULL && par22 == NULL && par222==NULL))
		throw EvalError("Cannot apply '*' to non-numeric arguments");

	if (par1 && par2) { // both are integer
		// in this case we just do the operation with integers and store the integer result
		return new EvalObjInt(par1->value * par2->value);
	} else if (par1) { // only first is integer
		// this means that another one is some Gecode type
		// so here we use the overloaded '*' operator, and the type is chosen with the use of thinary operator '? :'
		return new EvalObjLinIntExpr(par1->value * (par22?par22->value:par222->value));
	} else if (par2) { // only second is integer
		// the same like in previous case, but now the first argument is Gecode type
		return new EvalObjLinIntExpr((par11?par11->value:par111->value) * par2->value);
	} else { // none is integer
		// both of arguments are of some Gecode types
		// we just need to pick which one with the use of '? :'
		return new EvalObjLinIntExpr((par11? par11->value : par111->value) * (par22?par22->value:par222->value));
	}
}

// division ('/')
EvalObj* div(EvalObj* p1, EvalObj* p2) {
	const EvalObjInt* par1 = dynamic_cast<const EvalObjInt*>(p1);
	const EvalObjInt* par2 = dynamic_cast<const EvalObjInt*>(p2);
	const EvalObjIntVar* par11 = dynamic_cast<const EvalObjIntVar*>(p1);
	const EvalObjIntVar* par22 = dynamic_cast<const EvalObjIntVar*>(p2);
	const EvalObjLinIntExpr* par111 = dynamic_cast<const EvalObjLinIntExpr*>(p1);
	const EvalObjLinIntExpr* par222 = dynamic_cast<const EvalObjLinIntExpr*>(p2);
	if ((par1==NULL && par11==NULL && par111==NULL) || (par2==NULL && par22 == NULL && par222==NULL))
		throw EvalError("Cannot apply '/' to non-numeric arguments");
	if (par1 && par2) { // both integer
		return new EvalObjInt(par1->value / par2->value);
	} else if (par1) { // only first integer
		return new EvalObjLinIntExpr(par1->value / (par22?par22->value:par222->value));
	} else if (par2) { // only second integer
		return new EvalObjLinIntExpr((par11?par11->value:par111->value) / par2->value);
	} else { // none is integer
		return new EvalObjLinIntExpr((par11? par11->value : par111->value) / (par22?par22->value:par222->value));
	}
}

// addition ('+')
EvalObj* add(EvalObj* p1, EvalObj* p2) {
	const EvalObjInt* par1 = dynamic_cast<const EvalObjInt*>(p1);
	const EvalObjInt* par2 = dynamic_cast<const EvalObjInt*>(p2);
	const EvalObjIntVar* par11 = dynamic_cast<const EvalObjIntVar*>(p1);
	const EvalObjIntVar* par22 = dynamic_cast<const EvalObjIntVar*>(p2);
	const EvalObjLinIntExpr* par111 = dynamic_cast<const EvalObjLinIntExpr*>(p1);
	const EvalObjLinIntExpr* par222 = dynamic_cast<const EvalObjLinIntExpr*>(p2);
	if ((par1==NULL && par11==NULL && par111==NULL) || (par2==NULL && par22 == NULL && par222==NULL))
		throw EvalError("Cannot apply '+' to non-numeric arguments");
	if (par1 && par2) { // both integer
		return new EvalObjInt(par1->value + par2->value);
	} else if (par1) { // only first integer
		return new EvalObjLinIntExpr(par1->value + (par22?par22->value:par222->value));
	} else if (par2) { // only second integer
		return new EvalObjLinIntExpr((par11?par11->value:par111->value) + par2->value);
	} else { // none is integer
		return new EvalObjLinIntExpr((par11? par11->value : par111->value) + (par22?par22->value:par222->value));
	}
}

// subtraction ('-')
 EvalObj* sub(EvalObj* p1, EvalObj* p2) {
	const EvalObjInt* par1 = dynamic_cast<const EvalObjInt*>(p1);
	const EvalObjInt* par2 = dynamic_cast<const EvalObjInt*>(p2);
	const EvalObjIntVar* par11 = dynamic_cast<const EvalObjIntVar*>(p1);
	const EvalObjIntVar* par22 = dynamic_cast<const EvalObjIntVar*>(p2);
	const EvalObjLinIntExpr* par111 = dynamic_cast<const EvalObjLinIntExpr*>(p1);
	const EvalObjLinIntExpr* par222 = dynamic_cast<const EvalObjLinIntExpr*>(p2);
	if ((par1==NULL && par11==NULL && par111==NULL) || (par2==NULL && par22 == NULL && par222==NULL))
		throw EvalError("Cannot apply '-' to non-numeric arguments");
	if (par1 && par2) { // both integer
		return new EvalObjInt(par1->value - par2->value);
	} else if (par1) { // only first integer
		return new EvalObjLinIntExpr(par1->value - (par22?par22->value:par222->value));
	} else if (par2) { // only second integer
		return new EvalObjLinIntExpr((par11?par11->value:par111->value) - par2->value);
	} else { // none is integer
		return new EvalObjLinIntExpr((par11? par11->value : par111->value) - (par22?par22->value:par222->value));
	}
}

 // identity (unary '+'). I denoted it as '~+' following your notation for unary operator '~!'
 // using just '+' will be ambiguius because during evaluation we need to know how many arguments to use
 EvalObj* id(EvalObj* p1) {
	const EvalObjInt* par1 = dynamic_cast<const EvalObjInt*>(p1);
	const EvalObjIntVar* par11 = dynamic_cast<const EvalObjIntVar*>(p1);
	const EvalObjLinIntExpr* par111 = dynamic_cast<const EvalObjLinIntExpr*>(p1);
	if (par1==NULL && par11==NULL && par111==NULL)
		throw EvalError("Cannot apply '~+' to non-numeric arguments");
	return p1;
 }

 // negation (unary '-'). I denoted it as '~-'
 EvalObj* neg(EvalObj* p1) {
	const EvalObjInt* par1 = dynamic_cast<const EvalObjInt*>(p1);
	const EvalObjIntVar* par11 = dynamic_cast<const EvalObjIntVar*>(p1);
	const EvalObjLinIntExpr* par111 = dynamic_cast<const EvalObjLinIntExpr*>(p1);
	if (par1==NULL && par11==NULL && par111==NULL)
		throw EvalError("Cannot apply '~-' to non-numeric arguments");
	if (par1) { // integer
		return new EvalObjInt(-par1->value);
	} else {
		return new EvalObjLinIntExpr(-(par11? par11->value : par111->value));
	}
 }

// generic comparison operation
// all actual comparison operations are evaluated through this
// unlike the arithmetical operations where we need to use the 'operator+' of different types,
// here we can write one generic function, and all actual comparison functions will call this one

// NOTE: I didn't find the way of adding just simple boolean values to Gecode.
// So the program doesn't support the expressions like "(2>1)&&(3>2)"
// We always need to have variable on the left or right side. For example, "(XS1+2>1)&&(3>XT2)" will work
EvalObj* comparison(EvalObj* p1, EvalObj* p2, Gecode::IntRelType irt, string name) {
	const EvalObjInt* par1 = dynamic_cast<const EvalObjInt*>(p1);
	const EvalObjInt* par2 = dynamic_cast<const EvalObjInt*>(p2);
	const EvalObjIntVar* par11 = dynamic_cast<const EvalObjIntVar*>(p1);
	const EvalObjIntVar* par22 = dynamic_cast<const EvalObjIntVar*>(p2);
	const EvalObjLinIntExpr* par111 = dynamic_cast<const EvalObjLinIntExpr*>(p1);
	const EvalObjLinIntExpr* par222 = dynamic_cast<const EvalObjLinIntExpr*>(p2);
	if ((par1==NULL && par11==NULL && par111==NULL) || (par2==NULL && par22 == NULL && par222==NULL))
		throw EvalError("Cannot apply '" + name + "' to non-numeric arguments");
	if (par1 && par2) { // both integer
		// comparison of integers is boolean
		// and I didn't find how to put booleans in Gecode
		// so for now comparison of constants is now allowed
		// it doesn't make much sence any way (we can just compute the constants by ourselves, and then change formula accordingly)
		throw EvalError("Comparison of constants is not supported");
	} else if (par1) { // only first integer
		// the following can look strange but it is correct:
		// the EvalObjBoolExpr has constructor with BoolExpr
		// however, BoolExpr has constroctor with LinIntRel
		// so when we call EvalObjBoolExpr(linIntRel), compiler applies EvalObjBoolExpr(BoolExpr(linIntRel))
		// that's why it works
		return new EvalObjBoolExpr(Gecode::LinIntRel(par1->value, irt, par22?par22->value:par222->value));
	} else if (par2) { // only second integer
		return new EvalObjBoolExpr(Gecode::LinIntRel(par11?par11->value:par111->value, irt, par2->value));
	} else { // none is integer
		return new EvalObjBoolExpr(Gecode::LinIntRel(par11?par11->value:par111->value, irt, par22?par22->value:par222->value));
	}
}


// less ('<')
EvalObj* le(EvalObj* p1, EvalObj* p2) {
	return comparison(p1, p2, Gecode::IntRelType::IRT_LE,"<");
}

// greater ('>')
EvalObj* gr(EvalObj* p1, EvalObj* p2){
	return comparison(p1, p2, Gecode::IntRelType::IRT_GR,">");
}

// less or equal ('<=')
EvalObj* leq(EvalObj* p1, EvalObj* p2) {
	return comparison(p1, p2, Gecode::IntRelType::IRT_LQ,"<=");
}

// greater or equal ('>=')
EvalObj* geq(EvalObj* p1, EvalObj* p2) {
	return comparison(p1, p2, Gecode::IntRelType::IRT_GQ,">=");
}

// equal ('==')
EvalObj* eq(EvalObj* p1, EvalObj* p2) {
	return comparison(p1, p2, Gecode::IntRelType::IRT_EQ,"==");
}

// not equal ('!=')
EvalObj* neq(EvalObj* p1, EvalObj* p2) {
	return comparison(p1, p2, Gecode::IntRelType::IRT_NQ,"!=");
}

// not
EvalObj* not(EvalObj* p1) {
	const EvalObjLinIntRel* par1 = dynamic_cast<const EvalObjLinIntRel*>(p1);
	const EvalObjBoolExpr* par11 = dynamic_cast<const EvalObjBoolExpr*>(p1);
	if(par1==NULL && par11==NULL)
		throw EvalError("Cannot apply '~!' to non-boolean argument");
	Gecode::BoolExpr res = Gecode::BoolExpr(par1? par1->value : par11->value, Gecode::BoolExpr::NodeType::NT_NOT);
	return new EvalObjBoolExpr(res);
}

// and
EvalObj* and(EvalObj* p1, EvalObj* p2) {
	const EvalObjLinIntRel* par1 = dynamic_cast<const EvalObjLinIntRel*>(p1);
	const EvalObjBoolExpr* par11 = dynamic_cast<const EvalObjBoolExpr*>(p1);
	const EvalObjLinIntRel* par2 = dynamic_cast<const EvalObjLinIntRel*>(p2);
	const EvalObjBoolExpr* par22 = dynamic_cast<const EvalObjBoolExpr*>(p2);
	if ((par1==NULL && par11==NULL) || (par2==NULL && par22 == NULL))
		throw EvalError("Cannot apply '&&' to non-boolean arguments");
	Gecode::BoolExpr res = Gecode::BoolExpr(par1? par1->value : par11->value, Gecode::BoolExpr::NodeType::NT_AND, par2?par2->value:par22->value);
	return new EvalObjBoolExpr(res);
}

// or
EvalObj* or(EvalObj* p1, EvalObj* p2) {
	const EvalObjLinIntRel* par1 = dynamic_cast<const EvalObjLinIntRel*>(p1);
	const EvalObjBoolExpr* par11 = dynamic_cast<const EvalObjBoolExpr*>(p1);
	const EvalObjLinIntRel* par2 = dynamic_cast<const EvalObjLinIntRel*>(p2);
	const EvalObjBoolExpr* par22 = dynamic_cast<const EvalObjBoolExpr*>(p2);
	if ((par1==NULL && par11==NULL) || (par2==NULL && par22 == NULL))
		throw EvalError("Cannot apply '||' to non-boolean arguments");
	Gecode::BoolExpr res = Gecode::BoolExpr(par1? par1->value : par11->value, Gecode::BoolExpr::NodeType::NT_OR, par2?par2->value:par22->value);
	return new EvalObjBoolExpr(res);
}