// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// TODO: Write description of this file.
#ifndef BASETYPE_H
#define BASETYPE_H

#include "AbstractType.h"
using namespace std;

class BaseType : public AbstractType {
    string cName;

public:
    BaseType(string ident, string cName) : AbstractType(ident)
    {
        this->cName = cName;
    }
};

#endif
