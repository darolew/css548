// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// TODO: Write description of this file.
#ifndef TYPE_H
#define TYPE_H

#include "Symbol.h"

class Type : public Symbol {

public:
    Type(string id) : Symbol(id) {}
    
    virtual bool isType() 
    {
        return true;
    }
};

#endif
