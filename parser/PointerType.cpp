#include "PointerType.h"
#include "SymTable.h"

//Pointer types are a little special. They are allowed to be 
//declared before before the type type they point to has been
//declared.
//This method skips the the symbol checking in the superclass method.
bool PointerType::insertInto(SymTable &symTable)
{

    return Symbol::insertInto(symTable);
}
