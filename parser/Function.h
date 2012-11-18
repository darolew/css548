// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// FunctionType defines a Pascal function or procedure. Stores the name,
// the parameters, and (for functions) the return type.

#ifndef FUNCTION_H
#define FUNCTION_H

#include <list>
#include "AbstractType.h"
#include "Variable.h"

class Function : public Symbol {

private:
    list<Variable> params;    // formal parameters (ordered list)
    AbstractType *returnType; // return type; left NULL for procedures

public:
    Function();
    void addParam(Variable *param);
    virtual void generateDefinition(string);
    virtual bool insertInto();
    void setReturnType(AbstractType *rt);
    virtual string toString();
    virtual string toStringLong();
};

#endif
