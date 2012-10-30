#ifndef BASETYPE_H
#define BASETYPE_H

#include <string.h>
using namespace std;

class BaseType {

public:
	BaseType(string pName, string cName) : pascalName(pName), cName(cName) {}

private:
	string pascalName;
	string cName;
};

#endif
