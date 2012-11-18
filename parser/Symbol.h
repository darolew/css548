// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// Class to hold a symbol's attributes.

#ifndef SYMBOL_H
#define SYMBOL_H

#include <iostream>
#include <list>
#include <string>
using namespace std;

class Symbol;
typedef list<Symbol*> Table;
class SymTable;

class Symbol {

public:
    string identifier; // the identifier associated with this symbol

    Symbol(string);
    virtual ~Symbol();
    virtual string className();
    virtual bool insertInto();
    virtual void resolve();
    virtual bool isType();
    virtual string toString();
};

#endif
