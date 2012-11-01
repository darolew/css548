#ifndef TYPEDEF_H
#define TYPEDEF_H

#include <string.h>
#include "AbstractType.h"

using namespace std;

class TypeDef : AbstractType {

public:

private:
	string alais;
	BaseType* type;
};

#endif
