#ifndef VARIABLE_H
#define VARIABLE_H

#include <string> 
#include "Type.h"
using namespace std;


class Variable: public Symbol {

public:
	Variable(string id, Type * type) : Symbol(id, yvar), type(type) {}
	Type* type;

};

#endif

