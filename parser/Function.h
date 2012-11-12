// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// TODO: Write description of this file.
#ifndef FUNCTION_H
#define FUNCTION_H

#include <list>
#include "AbstractType.h"
#include "Variable.h"

class Function : public Symbol {

private:
    list<Variable> params;
    AbstractType *returnType;
    
public:

    Function() : Symbol(""), returnType(NULL) {}
    Function(string id, AbstractType *returnType, list<Variable> params)
        : Symbol(id)
    {
        this->params = params;
        this->returnType = returnType;
    }

    virtual string toString(void);
	virtual string toStringLong(void);  
    virtual bool insertInto();
    void addParam(Variable *param);
    void setReturnType(AbstractType *rt);
};

#endif
