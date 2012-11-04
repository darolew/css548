// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// Class to hold a symbol's attributes.
#ifndef SYMBOL_H
#define SYMBOL_H

#include <iostream>
#include <map>
#include <string> 
using namespace std;

class Symbol;
typedef map<string, Symbol*> Table;
class SymTable;

class Symbol {
public:
    string identifier;

    Symbol(string id)
    {
        this->identifier = id;
    }
    
    virtual string toString()
    {
        return identifier;
    }
    
    virtual bool isType() 
    {
        return false;
    }
    
    virtual bool insertInto(SymTable &);
};

#endif
