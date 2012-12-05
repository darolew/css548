// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This file contains the method definitions of the SymTable class.

#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include "SymTable.h"
#include "AbstractType.h"
#include "BaseType.h"
#include "Function.h"
#include "MemFunction.h"
#include "IoFunction.h"
#include "main.h"

//
//Public SymTable methods
//

//Constructor for the symbol table.
SymTable::SymTable()
{
    //Print the SIT, program scope without indentation
    indentThreshold=1;

    //Start standard identifier.
    beginScope("Standard Identifier Table");

    insert(new BaseType("integer", "int"));
    insert(new BaseType("boolean", "bool"));
    insert(new BaseType("real", "double"));
    insert(new BaseType("char", "string")); //TODO: Using char as an array index.

    insert(new IoFunction("write"));
    insert(new IoFunction("writeln"));
    insert(new IoFunction("read"));
    insert(new IoFunction("readln"));

    insert(new MemFunction("new"));
    insert(new MemFunction("dispose"));
    
    
    //TODO: true and false are not types, they are values.
    //But it is convenient to put them in the symbol table
    insert(new BaseType("true", "true"));
    insert(new BaseType("false", "false"));
}

//Destructor for the symbol table.
//TODO: Why is the destructor not called when the object goes out of scope?
SymTable::~SymTable()
{
    //In the happy case, only the SIT is left on the stack, so we call
    //endScope() to clean it up. However, in error cases (e.g., if the
    //program contained a syntax error) this destructor might be called
    //when other scopes still exist. In that case, simply delete all
    //the remaining scopes.
    if (size() > 1) {
        //Error case, delete remaining scopes.
        do {
            delTopScope();
        } while (size());
    } else {
        //Happy case, cleanup the SIT scope.
        endScope();
    }
}

//Push a new scope onto the stack.
void SymTable::beginScope(string name)
{
    if (scopes.size() > indentThreshold)
        indent++;

    scopes.push_front(new Table());
    scopeNames.push_front(name);

#ifdef PRINTST
    //Print the new scope banner.
    cout << "\nENTER " << name << endl;
    printLine("-");
#endif
}

//Pop and discard the current scope.
//Pop and discard the current scope.
void SymTable::endScope()
{
#ifdef PRINTST
    //Print the symbols that are about to be destroyed.
    printST();
#endif

    if (indent > 0)
        indent--;

#ifdef PRINTST
    //Print the end scope banner.
    cout << "\nEXIT " << scopeNames.front() << endl;
    printLine("=");
#endif

    //Destroy the scope.
    delTopScope();
    scopeNames.pop_front();
}

//Insert a symbol into the symbol table for the current scope. The insert
//will fail if the symbol indentifier already exists for the current
//scope. Returns whether the insert succeeded.
bool SymTable::insert(Symbol *symbol)
{
    assertStack();
    return symbol->insertInto();
}

//Drill down through the list of scopes, front to back, and look for the
//symbol. Return the first Symbol if it is found, otherwise return null.
Symbol *SymTable::lookup(string key)
{
    //Start at the current scope, and proceed upward thence.
    list<Table*>::iterator ti = scopes.begin();
    for (; ti != scopes.end(); ti++) {
        Symbol *sym = lookup(*ti, key);
        if (sym)
            return sym;
    }
    return NULL;
}

//Look for a symbol in a given scope. Return the Symbol if it is found,
//otherwise return null.
Symbol *SymTable::lookup(Table *tbl, string key)
{
    //All symbols are stored in an ordered list, so unfortunately we must resort
    //to a linear search, which could be slow if the symbol table is very large.
    //It is possible to optimize this (e.g., with a second sorted list) but it
    //is probably not worth the time for this assignment.
    list<Symbol*>::iterator si = tbl->begin();
    for (; si != tbl->end(); si++) {
        Symbol *sym = *si;
        if (key == sym->identifier)
            return sym;
    }
    return NULL;
}

//Look for a symbol that represents a type. If there is no such symbol, or if
//the symbol found is not a type, this prints an error and returns null.
AbstractType *SymTable::lookupType(string key)
{
    Symbol *sym = lookup(key);
    if (!sym || !sym->isType()) {
        cout << "***ERROR: " << key << " is not a type" << endl;
        sym = NULL;
    }
    return (AbstractType*)sym;
}

//Returns the top of the symbol table stack; a convenience wrapper.
Table *SymTable::front()
{
    assertStack();
    return scopes.front();
}

//
Table *SymTable::SIT()
{
    assertStack();
    return scopes.back();
}

//TODO:comment
int SymTable::size()
{
    return (int)scopes.size();
}

//
//Private SymTable methods
//

//Print out a line of a given character, specified by divider.
void SymTable::printLine(string divider)
{
    for (int i=0; i<75; ++i)
        cout << divider;
    cout << endl;
}

//Remove the scope on top of the stack and reclaim memory.
void SymTable::delTopScope()
{
    Table *tbl = front();
    scopes.pop_front();
    delTable(tbl);
}

//Reclaim memory from a symbol table.
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

//Returns whether there are any scope objects on the stack.
bool SymTable::empty()
{
    return scopes.empty();
}

//Assert that the stack is not empty; used for debugging.
void SymTable::assertStack()
{
    assert(!empty());
}
