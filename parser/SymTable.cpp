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

SymTable::SymTable()
{
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
}

//Why is the destructor not called when the object goes out of scope?
SymTable::~SymTable()
{
    //Only the SIT should be left on the stack.
    if (scopes.size() > 1) {
        //TODO: comment that this is an error case
        do {
            delTopScope();
        } while (scopes.size());
    } else
        endScope();
}

//Push a new scope onto the stack.
void SymTable::beginScope(string name)
{
    if (scopes.size() > 0)
        indent++;
    scopeNames.push_front(name);
    scopes.push_front(new Table());
    cout << "\nENTER " << name << endl;
    printLine("-");
}

void SymTable::endScope()
{
    printST();
    if (scopes.size() > 0)
        indent--;
    delTopScope();
    cout << "\nEXIT " << scopeNames.front() << endl;
    printLine("=");
    scopeNames.pop_front();
}

bool SymTable::insert(Symbol *symbol)
{
    assertStack();
    return symbol->insertInto();
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
    if (!sym || !sym->isType())
        cerr << "error: " << key << " is not a type" << endl;
    return (AbstractType*)sym;
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
    string scopeName = scopeNames.front();
    Symbol *sym = lookup(scopeName);
    if (sym) {
        Function *func = (Function*)sym;
        //TODO: comment that this is bad
        indent--;
        cout << indentation();
        cout << func->toStringLong();
        cout << "    ";
        indent++;
    } else
        cout << indentation();
    Table *tbl = front();
    list<Symbol*>::reverse_iterator it = tbl->rbegin();
    for (; it != tbl->rend(); it++)
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
