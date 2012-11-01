#ifndef BASETYPE_H
#define BASETYPE_H

#include <string.h>
#include "AbstractType.h"

using namespace std;

class BaseType : public AbstractType {

public:
	BaseType(string ident, int token, string cName) : AbstractType(ident, token), cName(cName) {}

private:
	string cName;
};

#endif
