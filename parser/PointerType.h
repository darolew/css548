// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// PointerType defines a pointer.

#ifndef POINTERTYPE_H
#define POINTERTYPE_H

#include "AbstractType.h"

class PointerType : public AbstractType {

public:
    string pointeeName;

    PointerType(string, string);
    virtual string className();
    virtual bool insertInto();
    virtual void resolve();
    virtual void generateCode(ostream &, string);
    virtual void generateDefinition(ostream &, string);
    virtual string toString();
};

#endif
