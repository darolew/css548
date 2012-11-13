// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// Specialized class for memory functions; viz., new and dispose.
// Just a placeholder class for phase 3.

#ifndef MEMFUNCTION_H
#define MEMFUNCTION_H

#include "Symbol.h"
using namespace std;

//TODO: Add methods for code generation
class MemFunction : public Symbol {

public:
    MemFunction(string id) : Symbol(id)
    {
    }
};

#endif
