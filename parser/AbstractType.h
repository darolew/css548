#ifndef ABSTRACTTYPE_H
#define ABSTRACTTYPE_H

#include "Symbol.h"

class AbstractType : public Symbol {

public:
  AbstractType(string id) : Symbol(id) {}
  AbstractType(string id, int token) : Symbol(id, token) {}

};

#endif
