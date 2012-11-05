// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// TODO: Write description of this file.
#ifndef ABSTRACTTYPE_H
#define ABSTRACTTYPE_H

#include "Symbol.h"

class AbstractType : public Symbol {

public:
    AbstractType *type;

    AbstractType(string id) : Symbol(id) {}

    AbstractType(string id, AbstractType *type)
        : Symbol(id)
    {
        this->type = type;
    }
    
    virtual bool isType() 
    {
        return true;
    }
};

#endif
