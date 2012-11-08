// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// TODO: Write description of this file.
#ifndef VARIABLE_H
#define VARIABLE_H

#include "AbstractType.h"
using namespace std;

class Variable: public Symbol {
    AbstractType *type;

public:
    Variable(string id, AbstractType *type) : Symbol(id)
    {
        this->type = type;
    }
    
    virtual string toString(void)
    {
        if (type)
            return identifier + " " + type->toString();
        return identifier + " <unknown_type>\n";
    }
};

#endif
