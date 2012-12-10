// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This file contains the definition of the Parameter class.

#ifndef PARAMETER_H
#define PARAMETER_H

#include "Variable.h"
using namespace std;

class Parameter : public Variable {
public:
    bool isReference;

    Parameter(string, AbstractType*, bool);
    virtual void generateDefinition(string);
};

#endif
