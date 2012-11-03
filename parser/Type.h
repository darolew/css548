#ifndef ABSTRACTTYPE_H
#define ABSTRACTTYPE_H

#include "Symbol.h"

class Type : public Symbol {
public:
    Type(string id) : Symbol(id) {}
    Type(string id, int token) : Symbol(id, token) {}
   	virtual bool isType() {
	    return true;
	}

};

#endif
