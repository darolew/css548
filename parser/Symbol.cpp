// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// TODO: Write description of this file.

#include <iostream>
#include <stdlib.h>
#include "Symbol.h"
#include "SymTable.h"
#include "main.h"

extern SymTable symTable;

bool Symbol::insertInto()
{
    //cout << "inserting " << toString();
    Table *table = symTable.front();
    Symbol *sym = symTable.lookup(table, identifier);
    if (sym) {
        cerr << "error: " << identifier << " already exists in scope\n";
        return false;
    }

    table->push_front(this);
    //cout << "\tInsert succeeded.\n";
    return true;
}

string Symbol::toString()
{
    return identifier + nlindent();
}
