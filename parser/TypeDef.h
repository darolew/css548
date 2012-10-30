#ifndef TYPEDEF_H
#define TYPEDEF_H

#include <string.h>
#include "BaseType.h"

using namespace std;

class BaseType {

public:
	BaseType(string pName, string cName) : pascalName(pName), cName(cName) {}

private:
	string alais;
	BaseType* type;
};

#endif