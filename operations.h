#ifndef __OPERATIONS_H_INCLUDED__
#define __OPERATIONS_H_INCLUDED__

#include <map>
#include <string>

using namespace std;

// classes storing operations
// AbstractOperation is the base class
// We store the operations in a map, so we need different operations to be of the same type
struct AbstractOperation {
public:
	int order;
	AbstractOperation(int ord) : order(ord) {};
	virtual ~AbstractOperation() {}; // to make class virtual
};

// unary operation template
// practically, we will use EvalObj* instead of T
// the template is given here for generality
template <class T>
class UnaryOperation : public AbstractOperation {
private:
	T (*un_oper)(const T); // 'un_oper' is a pointer to function taking one argument of type T and returning T
public:
	UnaryOperation(int ord, T (*op)(const T)): AbstractOperation(ord), un_oper(op) {} // simple constructor
	T eval_un(const T par) const { // very useful! we will take the class from map and run this method; this allows to avoid a lot of 'if-else' statements
		return un_oper(par); // C++ understands that we want to run the actual function although un_oper is actually a pointer to function
		// we could write equivalently: 'return (*un_oper)(par)'
	}
};

// binary operation template
// same like the previous one, but it takes two arguments
template <class T>
class BinaryOperation : public AbstractOperation {
private:
	T (*bin_oper)(const T, const T); // 'bin_oper' is a pointer to function taking two arguments of type T and returning T
public:
	BinaryOperation(int ord, T (*op)(const T, const T)): AbstractOperation(ord), bin_oper(op) {}
	T eval_bin(const T par1, const T par2 ) const {
		return bin_oper(par1,par2);
	}
};

typedef map<string, AbstractOperation*> OperationsMap_t; // this is how we will store the operations

// default operations map. Constructor creates all necessary operations.
// if needed, it's easy to add any new operations
class DefOperationsMap : public map<string, AbstractOperation*> {
public:
	DefOperationsMap(); // see operations.cpp for details
	~DefOperationsMap();
};

#endif