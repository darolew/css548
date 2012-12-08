// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// FunctionType defines a Pascal function or procedure. Stores the name,
// the parameters, and (for functions) the return type.

#ifndef FUNCTION_H
#define FUNCTION_H

#include <vector>
#include "AbstractType.h"
#include "Parameter.h"

//
//TODO: Having this function inherit from AbstractType is class abuse. A
//      function it *not* a type, just like a variable is not a type. It
//      *has* a type (a return type, if it is not a procedure).
//
class Function : public AbstractType {

private:
    vector<Parameter*> params;  // formal parameters (ordered list)

public:
    Function();
    virtual void generateDefinition(string);
    virtual bool insert();
    void addParam(Parameter*);
    void setReturnType(AbstractType*);
    virtual bool isFunction();
    virtual bool isProcedure();
    void endFunction();
    virtual void push();
    int numParams();
    Parameter *getParam(int);
    
    //TODO: Make this private
    AbstractType *returnType; // return type; left NULL for procedures
};

#endif
