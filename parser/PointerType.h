#ifndef POINTERTYPE_H
#define POINTERTYPE_H

#include "Type.h"

class PointerType : public Type {
public:
    PointerType(string id, Type* pointerType) : Type(id), 
	    pointerType(pointerType)  {}
	
	Type* pointerType;
   	
	virtual bool isType() {
	    return true;
	}
};

#endif
