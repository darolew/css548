// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// TODO: Write description of this file.
#ifndef CONST_H
#define CONST_H

#include <string> 
#include "Symbol.h"
#include "Range.h"
using namespace std;

class Const: public Symbol {
    Terminal term;

public:
    Const(string id, Terminal term) : Symbol(id)
    {
        this->term = term;
    }

    virtual string toString()
    {
        return identifier + "," + term.str;
    }
};

#endif
