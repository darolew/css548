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
bool PointerType::insertInto()
{
    return Symbol::insertInto();
}

//Add the type to which the PointerType points. This must be done
//after all the types for the scope have been declared.
void PointerType::addType(string typeName)
{
    type = symTable.lookupType(typeName);
}

string PointerType::toString(void)
{
    return identifier + " ^ " + type->toIdentTypeString();
}
    
