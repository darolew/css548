// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// TODO: Write description of this file.
#ifndef POINTERTYPE_H
#define POINTERTYPE_H

#include "OfType.h"

class PointerType : public OfType {

public:
    PointerType(string id, string typeName) : OfType(id, typeName) {}
    virtual bool insertInto(SymTable &);
};

#endif
