// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// Specialized class for IO functions.
// Not used for anything important in phase 3.

#ifndef IOFUNCTION_H
#define IOFUNCTION_H

#include "Symbol.h" 
using namespace std;

//TODO: Add methods for code generation
class IoFunction : public Symbol {

public:
    IoFunction(string id) : Symbol(id)
    {
    }
};

#endif
