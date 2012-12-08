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
#include "y.tab.h"

//
//Public SymTable methods
//

//Constructor for the symbol table.
SymTable::SymTable()
{
    //Print the SIT, program scope without indentation
    indentThreshold = 1;

    //Start standard identifier.
    beginScope("Standard Identifier Table");

    //Base types
    insert(new BaseType("integer", "int", yinteger));
    insert(new BaseType("real", "double", yreal));
    insert(new BaseType("char", "string", ystring)); //TODO: Using char as an array index.

    //Tracking booleans by token is convenient
    insert(new BaseType("boolean", "bool", yboolean));
    //insert(new BaseType("boolean", "bool", yident);
    
    //None of these are types. They are in the symbol table purely for
    //convenience -- it is a kluge to store them in the BaseType class.
    insert(new BaseType("true", "true", yident));
    insert(new BaseType("false", "false", yident));
    insert(new BaseType("nil", "NULL", ynil));
    
    //Keyword functions
    insert(new IoFunction("write"));
    insert(new IoFunction("writeln"));
    insert(new IoFunction("read"));
    insert(new IoFunction("readln"));

    insert(new MemFunction("new"));
    insert(new MemFunction("dispose"));
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
}

//Pop and discard the current scope.
//Pop and discard the current scope.
void SymTable::endScope()
{
    if (indent > 0)
        indent--;

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
    return symbol->insert();
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

//Lookup a base type in the SIT
BaseType *SymTable::lookupSIT(int token) 
{
    //This is a little kludgy
    string id;
    //
    //TODO: Make this less brittle. If new base types are 
    //added into the SIT, this method must be modified.
    //
    if (token == yinteger)
        id = "integer";
    else if (token == yreal)
        id = "real";
    else if (token == yboolean)
        id = "boolean";
    else if (token == ystring)
        id = "char";
    else if (token == ynil)
        id = "nil";
    
    Symbol *sym = lookup(SIT(), id); 
    return dynamic_cast<BaseType *>(sym);
}
