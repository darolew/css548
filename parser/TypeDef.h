// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// TODO: Write description of this file.
#ifndef TYPEDEF_H
#define TYPEDEF_H

#include "OfType.h"
using namespace std;

class TypeDef : public OfType {

public:
    TypeDef(string alias, string typeName) : OfType(alias, typeName) {}
};

#endif
