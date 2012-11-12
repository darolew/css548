// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// TODO: Write description of this file.
#ifndef POINTERTYPE_H
#define POINTERTYPE_H

#include "AbstractType.h"

class PointerType : public AbstractType {

public:
    PointerType(string id) : AbstractType(id) {}
    virtual bool insertInto();
    void addType(string typeName);
    virtual string toString();
};

#endif
