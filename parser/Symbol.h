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
//typedef map<string, Symbol*> Table;
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
    
    virtual bool insertInto(SymTable &);

    
    //For some resons, just defining the destructor eliminates many memory leaks.
    virtual ~Symbol()
    {
     
    } 
    
};

#endif
