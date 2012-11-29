// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// FunctionType defines a Pascal function or procedure. Stores the name,
// the parameters, and (for functions) the return type.

#ifndef FUNCTION_H
#define FUNCTION_H

#include <list>
#include "AbstractType.h"
#include "Parameter.h"

class Function : public Symbol {

private:
    list<Parameter*> params;  // formal parameters (ordered list)
    AbstractType *returnType; // return type; left NULL for procedures

public:
    Function();
    virtual void generateDefinition(string);
    virtual bool insertInto();
    void addParam(Parameter *param);
    void setReturnType(AbstractType *rt);
    virtual bool isFunction();
    virtual bool isProcedure();
    void endFunction();
    virtual string toString();
    virtual string toStringLong();
};

#endif
