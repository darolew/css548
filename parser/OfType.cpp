// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// TODO: Write description of this file.
#include <iostream>
#include <stdlib.h>
#include "OfType.h"
#include "SymTable.h"

bool OfType::insertInto(SymTable &symTable)
{
    Symbol *sym = symTable.lookup(typeName);
    if (!sym || !sym->isType()) {
        cerr << "error: " << typeName << " is not a type\n";
        exit(1);
    }

    type = (Type*)sym;
    return Symbol::insertInto(symTable);
}
