// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// TODO: Write description of this file.
#ifndef VARIABLE_H
#define VARIABLE_H

#include "OfType.h"
using namespace std;

class Variable: public OfType {

public:
    Variable(string id, string typeName) : OfType(id, typeName) {}
        
    virtual bool isType() 
    {
        return false;
    }
};

#endif
