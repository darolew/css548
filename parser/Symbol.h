// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// Class to hold a symbol's attributes.

#ifndef SYMBOL_H
#define SYMBOL_H

#include <iostream>
#include <list>
#include <string>
#include "main.h"
using namespace std;

class Symbol;
typedef list<Symbol*> Table;
class SymTable;

class Symbol {
public:
    string identifier;

    Symbol(string id)
    {
        this->identifier = id;
    }

    virtual string toString();

    virtual bool isType()
    {
        return false;
    }

    virtual bool insertInto();

    //For some resons, just defining the destructor eliminates many memory leaks.
    virtual ~Symbol()
    {

    }

};

#endif
