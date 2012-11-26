// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This class represents a Pascal constant and its value.

#ifndef CONST_H
#define CONST_H

#include <string>
#include "Symbol.h"
#include "Range.h"
using namespace std;

class Const: public Symbol {

private:
    //Constant's value
    Terminal term;

public:
    //Refer to the .cpp file for information about these methods.
    Const(string, Terminal);
    virtual void generateDefinition(string);
    virtual string toString();
};

#endif
