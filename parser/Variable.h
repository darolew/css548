#ifndef VARIABLE_H
#define VARIABLE_H

#include <string> 
#include "AbstractType.h"
using namespace std;

//NOTE: "type" is a C++ reservered word. "_type" is not.
 
class Variable: public Symbol {

public:
	Variable(string id, AbstractType * _type) : Symbol(id, yvar), _type(_type) {}
	AbstractType* _type;

};

#endif

