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
    virtual bool insert();
    virtual void resolve();
    virtual void generateCode(string);
    virtual void generateDefinition(string);
    string cPointeeName();
    virtual bool isPointer();
    virtual bool compatible(AbstractType*, int);
};

#endif
