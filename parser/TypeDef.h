#ifndef TYPEDEF_H
#define TYPEDEF_H

#include <string.h>
#include "OfType.h"

using namespace std;

class TypeDef : public OfType {

public:
    TypeDef(string alias, string typeName) : OfType(alias, typeName) {}

};

#endif
