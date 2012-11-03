#ifndef TYPEDEF_H
#define TYPEDEF_H

#include <string.h>
#include "Type.h"

using namespace std;

class TypeDef : public Type {

public:
    TypeDef(string alias, Type *type)
        : Type(alias), type(type) {}

private:
	Type *type;
};

#endif
