// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// TODO: Write description of this file.
#ifndef NAMEDTYPE_H
#define NAMEDTYPE_H

#include "AbstractType.h"
using namespace std;

class NamedType : public AbstractType {
    AbstractType *type;

public:
    NamedType(string alias, AbstractType *type) : AbstractType(alias)
    {
        this->type = type;
    }
};

#endif
