#ifndef CONST_H
#define CONST_H

#include <string> 
#include "Symbol.h"
using namespace std;


class Const: public Symbol {
public:
	Const(string id, string value) : Symbol(id), value(value) {}
	string value;
	
	virtual string toString()
	{
		return identifier + "," + value;
	}
	
};

#endif

