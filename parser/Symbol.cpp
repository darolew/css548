// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// TODO: Write description of this file.
#include <iostream>
#include <stdlib.h>
#include "Symbol.h"
#include "SymTable.h"

bool Symbol::insertInto(SymTable &symTable)
{
    cerr << "inserting " << toString() << endl;
    Table &table = symTable.front(); 

    bool alreadyExists = table.count(identifier) != 0;
    if (alreadyExists) {
        cerr << "error: " << identifier << " already exists at this scope\n";
        //exit(1);
    }

    table[identifier] = this;
    //cerr << "\tInsert succeeded.\n"; 
    return true;
}

string Symbol::toString()
{
        return identifier;
}
