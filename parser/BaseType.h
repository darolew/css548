// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// TODO: Write description of this file.
#ifndef BASETYPE_H
#define BASETYPE_H

#include "Type.h"
using namespace std;

class BaseType : public Type {
    string cName;

public:
    BaseType(string ident, string cName) : Type(ident)
    {
        this->cName = cName;
    }
};

#endif
