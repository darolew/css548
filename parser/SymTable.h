// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// Data structures for managing symbols and scope.

#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <string>
#include <list>
#include "Symbol.h"
#include "AbstractType.h"
#include "BaseType.h"
#include "Variable.h"
using namespace std;

class SymTable {

private:
    //An ordered list of the current scopes in the program. The first scope
    //is special, as it is the SIT; beneath that is the global scope. Other
    //scopes are transient, created as functions and subfunctions are seen
    //and destroyed when they end.
    list<Table*> scopes;

    //For debugging purposes, each scope in the above list has a name.
    list<string> scopeNames;
    
public:
    //See the .cpp file for descriptions of these methods.
    SymTable();
    virtual ~SymTable();
    void beginScope(string="Unnamed");
    void endScope();
    bool insert(Symbol*);
    Symbol *lookup(string key);
    Symbol *lookup(Table*, string);
    BaseType* lookupSIT(int);
    AbstractType *lookupType(string);
    Table *front();
    Table *SIT();
    int size();

private:
    int indentThreshold;
    void printST();
    void printLine(string);
    void delTopScope();
    void delTable(Table*);
    bool empty();
    void assertStack();
};

#endif
