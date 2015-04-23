# Parse-Expression-Gecode
C++ Application parsing math expressions to create specialized objects for loading into a constraint solving system

**_evalobj.h_** defines generic class *EvalObj* used for evaluation of parsed mathematical expression, and its inheritants with defined arithmetical and logical operations on them

**_operations.h_** defines generic classes *AbstractOperation*, *UnaryOperation* and *BinaryOperation*, and a map *DefOperationsMap* containing all operations that will be used for evaluation.

**_parser.h_** defines generic functions for parsing mathematical expressions based on operations symbols defined in Operations Map and evaluating them based on corresponding functions defined in Operations Map. Parser can work with any objects as long as they are inheritants of *EvalObj* and *AbstractOperation*
