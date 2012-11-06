// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// TODO: Write description of this file.
#include <iostream>
#include <stdlib.h>
#include "AbstractType.h"
#include "BaseType.h"
#include "SymTable.h"

//Push a new scope onto the stack.
void SymTable::beginScope(string name) 
{
    scopeNames.push_front(name);
    scopes.push_front(new Table());
    cerr << "\nENTER " << name << endl;
    printLine("-"); 
}

void SymTable::endScope() 
{ 
    assert_stack();
    delTopScope();
    printST();
    cerr << "\nEXIT " << scopeNames.front() << endl;
    printLine("=");
    scopeNames.pop_front();
}

bool SymTable::insert(Symbol *symbol) 
{
    assert_stack();
    return symbol->insertInto(*this);
}

//Drill down through the list of scopes, front to back, and look
//for the symbol. Return the first Symbol if it is found, otherwise
//return null.
Symbol *SymTable::lookup(string key) 
{
    //This is a linear search, which is slow and bad.
    list<Table*>::iterator ti = scopes.begin();
    for (; ti != scopes.end(); ti++) {
        Symbol *sym = lookup(*ti, key);
        if (sym)
            return sym;
    }
    return NULL;
}

Symbol *SymTable::lookup(Table *tbl, string key)
{
    list<Symbol*>::iterator si = tbl->begin();
    for (; si != tbl->end(); si++) {
        Symbol *sym = *si;
        if (key == sym->identifier)
            return sym;
    }
    return NULL;
}

//
AbstractType *SymTable::lookupType(string key)
{
    Symbol *sym = lookup(key);
    if (!sym || !sym->isType()) {
        cerr << "error: " << key << " is not a type" << endl;
        exit(1);
    }
    return (AbstractType*)sym;
}

SymTable::SymTable() 
{
    //Start standard identifier.
    beginScope("Standard Identifier Table");
    
    insert(new BaseType("boolean", "bool"));
    insert(new BaseType("integer", "int"));
    insert(new BaseType("real", "double"));
    
    //TODO: Using char as an array index.
    insert(new BaseType("char", "string"));
}

//When the SymTable class goes out of scope (if it was on the stack)
//or deleted (if it was on the heap), the list objects will be deleted
//because they were created on the stack when the SymTable constructor
//was called. 
//When an STL class like list is deleted, it will call the desctructor
//on every object still in the list. No explicit cleanup is needed.
SymTable::~SymTable() 
{
    cleanUp();
}

bool SymTable::empty() 
{
    return scopes.empty();
}

//Prevent seg faults.
void SymTable::assert_stack() 
{
    if (empty()) {
        cerr << "\nFATAL ERROR SymTable::assert_stack\n\n";
        exit(EXIT_FAILURE);
    }
}

void SymTable::printST() 
{
    // ... 
}

void SymTable::printLine(string divider)  
{
    for(int i=0; i<75; ++i)
        cerr << divider;
    cerr << endl;
}

//Convenience wrapper
Table *SymTable::front() 
{
    return scopes.front();
}

//Reclaim memory from a symbol table
void SymTable::delTable(Table *tbl) 
{
    list<Symbol*>::iterator si = tbl->begin();
    for (; si != tbl->end(); si++) {
        Symbol *sym = *si;
        if (sym)
            delete sym;
    }
}  

//Remove the scope on top of the stack and reclaim memory.
void SymTable::delTopScope() 
{
    Table *tbl = front();
    delTable(tbl);
    scopes.pop_front();
    delete tbl;
}

void SymTable::cleanUp()
{
    //Get rid of the SIT.
    assert_stack();
    
    //TODO: Assert that the stack has exactly one symbol table on it.
    endScope();
}
