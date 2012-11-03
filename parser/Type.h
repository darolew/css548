#ifndef TYPE_H
#define TYPE_H

#include "Symbol.h"

class Type : public Symbol {
public:
    Type(string id) : Symbol(id) {}
	
   	virtual bool isType() {
	    return true;
	}

};

#endif
