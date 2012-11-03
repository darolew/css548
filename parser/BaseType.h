#ifndef BASETYPE_H
#define BASETYPE_H

#include <string.h>
#include "Type.h"

using namespace std;

class BaseType : public Type {

public:
	BaseType(string ident, string cName)
	    : Type(ident), cName(cName) {}

	string cName;
};

#endif
