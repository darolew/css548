#ifndef VARIABLE_H
#define VARIABLE_H

#include <string> 
#include "AbstractType.h"
using namespace std;

//NOTE: "type" is a C++ reservered word. "_type" is not.
 
class Variable: public Symbol {

public:
	Variable(string id, Symbol *_type) : Symbol(id), _type(_type) {}
	Symbol *_type;
};

#endif

