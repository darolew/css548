#ifndef VARIABLE_H
#define VARIABLE_H

#include <string> 
#include "OfType.h"
using namespace std;
class SymTable;

class Variable: public OfType {

public:
	Variable(string id, string typeName) : OfType(id, typeName) {}
		
   	virtual bool isType() 
	{
	    return false;
	}
};

#endif

