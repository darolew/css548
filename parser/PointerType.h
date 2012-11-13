// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// PointerType defines a pointer.

#ifndef POINTERTYPE_H
#define POINTERTYPE_H

#include "AbstractType.h"

class PointerType : public AbstractType {

public:
    PointerType(string);
    virtual bool insertInto();
    void addType(string);
    virtual string toString();
};

#endif
