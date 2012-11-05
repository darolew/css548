// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// TODO: Write description of this file.
#ifndef FUNCTION_H
#define FUNCTION_H

#include <list>
#include "AbstractType.h"
#include "Variable.h"
using namespace std;

class Function: public Symbol {

public:
    list<Variable*> params;
    AbstractType *returnType;

    Function() : Symbol(""), returnType(NULL) {}
    Function(string id, AbstractType *returnType, list<Variable*> params)
        : Symbol(id)
    {
        this->params = params;
        this->returnType = returnType;
    }

    virtual string toString(void)
    {
        stringstream ss (stringstream::in | stringstream::out);
        ss << (returnType ? "function" : "procedure");
        ss << " " << identifier << "(";
        list<Variable*>::iterator it = params.begin();
        for (; it != params.end(); it++) {
            Variable *var = *it;
            ss << var->toString() << ", ";
        }
        ss << ") ";
        if (returnType)
            ss << ": " << returnType->toString();
        ss << endl;
        return ss.str();
    }
   
    virtual bool insertInto(SymTable &symTable) 
    {
        Symbol::insertInto(symTable);
        list<Variable*>::iterator it = params.begin();
        for(; it!=params.end(); ++it)
            (*it)->insertInto(symTable);
        return true; //TODO: Change return type to void because we exit on error
    }     
};

#endif
