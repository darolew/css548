#ifndef OFTYPE_H
#define OFTYPE_H

#include "Type.h"
using namespace std;
class SymTable;

class OfType : public Type {

public:
    OfType(string id, string typeName)
        : Type(id), typeName(typeName), type(NULL) {}

    virtual bool insertInto(SymTable &);
		
	Type *type;
	string typeName;
};

#endif
