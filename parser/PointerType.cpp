// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// TODO: Write description of this file.
#include "PointerType.h"
#include "SymTable.h"

extern SymTable symTable;

//Pointer types are special: they are allowed to be declared before
//the type they point to has been declared. Thus, this method skips
//the symbol checking of the superclass method.
bool PointerType::insertInto(SymTable &symTable)
{
    return Symbol::insertInto(symTable);
}

//
void PointerType::addType()
{
	Symbol *sym = symTable.lookup(typeName);
    if (!sym || !sym->isType()) {
        cerr << "error: " << typeName << " is not a type\n";
        exit(1);
    }

	type = (Type*)sym;
}
