#ifndef BASETYPE_H
#define BASETYPE_H

#include <string.h>
#include "Type.h"

using namespace std;

class BaseType : public Type {

public:
	BaseType(string ident, int token, string cName) : Type(ident, token), cName(cName) {}

private:
	string cName;
};

#endif
