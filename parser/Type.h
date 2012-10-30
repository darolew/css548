#ifndef TYPE_H
#define TYPE_H

#include "Symbol.h"

class Type : public Symbol {

public:
	Type(string id, int token) : Symbol(id, token) {}

};

#endif
