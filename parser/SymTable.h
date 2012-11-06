// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// Data structures for managing symbols and scope.
#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <map>
#include <string> 
#include <list>
#include "Symbol.h"
#include "AbstractType.h"
#include "Variable.h"
using namespace std;

class SymTable {
    list<Table*> scopes;
    list<string> scopeNames;

public: 
    //Constructor
    SymTable();

    //Create a new table and push it onto the back of the list.
    void beginScope(string="Unnamed");

    //Pop (and discard) the table on the back of the list.
    void endScope();

    //Insert a symbol into the table. 
    //Return true if insert suceeded. Return false if insert failed.
    //Insert will fail if symbol is already defined in the current scope.
    bool insert(Symbol *);
    // bool insert(Variable *);

    //Search through all the tables in the list, starting with the last one, 
    //and look for the key. 
    Symbol *lookup(string key);
    
    //
    Symbol *lookup(Table *, string);
    
    //
    AbstractType *lookupType(string);

    //Return true if there are no scope objects on the stack.
    bool empty();

    //Return top of the stack
    Table *front();
    
    //Destructor
    ~SymTable();
  
private:
    void assert_stack();
    void printST();
    void printLine(string);
};

#endif

