// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// TODO: Write description of this file.
#include <iostream>
#include <stdlib.h>
#include <cassert>
#include "AbstractType.h"
#include "BaseType.h"
#include "SymTable.h"

//Push a new scope onto the stack.
void SymTable::beginScope(string name) 
{
    scopeNames.push_front(name);
    scopes.push_front(new Table());
    cout << "\nENTER " << name << endl;
    printLine("-"); 
}

void SymTable::endScope() 
{
    printST();
    delTopScope();
    cout << "\nEXIT " << scopeNames.front() << endl;
    printLine("=");
    scopeNames.pop_front();
}

bool SymTable::insert(Symbol *symbol) 
{
    assertStack();
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
        cout << "error: " << key << " is not a type" << endl;
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

//Why is the destructor not called when the object goes out of scope?
SymTable::~SymTable() 
{
    //Only the SIT should be left on the stack.
    assert(scopes.size() == 1);
    endScope();
}

bool SymTable::empty() 
{
    return scopes.empty();
}

//Prevent seg faults.
void SymTable::assertStack() 
{
    assert(!empty());
}

void SymTable::printST() 
{
    Table *tbl = front();
    list<Symbol*>::iterator it = tbl->begin();
    for (; it != tbl->end(); it++)
        cout << (*it)->toString();
}

void SymTable::printLine(string divider)  
{
    for (int i=0; i<75; ++i)
        cout << divider;
    cout << endl;
}

//Convenience wrapper
Table *SymTable::front() 
{
    assertStack();
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
    
    delete tbl;
}  

//Remove the scope on top of the stack and reclaim memory.
void SymTable::delTopScope() 
{
    assertStack();
    Table *tbl = front();
    scopes.pop_front();
    delTable(tbl);   
}
