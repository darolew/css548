// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// TODO: Write description of this file.
#ifndef CONST_H
#define CONST_H

#include <string> 
#include "Symbol.h"
using namespace std;

class Const: public Symbol {
    string value;

public:
    Const(string id, string value) : Symbol(id)
    {
        this->value = value;
    }

    virtual string toString()
    {
        return identifier + "," + value;
    }
};

#endif
