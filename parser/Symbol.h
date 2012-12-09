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
    virtual void push();
    virtual ~Symbol();
    virtual bool insert();
    virtual void resolve();
    virtual bool isType();
    virtual bool isFunction();
    virtual bool isIoFunction();
    virtual bool isMemFunction();
    virtual bool isPointer();
    virtual bool isProcedure();
    virtual bool isArray();
    virtual bool isRecord();
    virtual bool isSet();
    virtual string className();
    virtual string dump();
    virtual void event_Designator(string);
};

#endif
