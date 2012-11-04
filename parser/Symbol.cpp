// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// TODO: Write description of this file.
#include "Symbol.h"
#include "SymTable.h"

bool Symbol::insertInto(SymTable &symTable)
{
    cerr << "inserting " << toString() << endl;
    Table &table = symTable.front(); 

    bool alreadyExists = table.count(identifier) != 0;
    if (alreadyExists) {
        //cerr << "\n\tInsert failed. Symbol name already exists in current scope.\n";
        return false;
    }

    table[identifier] = this;
    //cerr << "\tInsert succeeded.\n"; 
    return true;
}
