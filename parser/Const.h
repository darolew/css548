#ifndef CONST_H
#define CONST_H

#include <string> 
#include "Symbol.h"
using namespace std;


class Const: public Symbol {

public:
	Const(string id, string value) : Symbol(id, yconst), value(value) {}
	string value;

};

#endif

