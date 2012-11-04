// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// TODO: Write description of this file.
#ifndef OFTYPE_H
#define OFTYPE_H

#include "Type.h"
using namespace std;
class SymTable;

class OfType : public Type {
    string typeName;
    
public:
    Type *type;

    OfType(string id, string typeName) : Type(id)
    {
        this->typeName = typeName;
        this->type = NULL;
    }

    virtual bool insertInto(SymTable &);
};

#endif
