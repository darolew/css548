#ifndef TYPEDEF_H
#define TYPEDEF_H

#include <string.h>
#include "AbstractType.h"

using namespace std;

class TypeDef : public AbstractType {

public:
    TypeDef(string alias, AbstractType *type)
        : AbstractType(alias), type(type) {}

private:
	AbstractType *type;
};

#endif
