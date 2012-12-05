// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This file contains the definition of the Variable class. Note that Variable
// objects are also used for record fields.

#ifndef VARIABLE_H
#define VARIABLE_H

#include "AbstractType.h"
using namespace std;

class Variable: public Symbol {
public:
    AbstractType *type; // a variable has a type
    
    Variable(string, AbstractType *);
    virtual ~Variable();
    virtual void generateCode(string);
    virtual void generateDefinition(string);
    virtual void generateNewStatement();
    virtual bool isArray();
    virtual bool isRecord();
    virtual string toString();
    virtual string className();
    virtual AbstractType *getType();
};

#endif
