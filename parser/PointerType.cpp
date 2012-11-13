// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This file contains method definitions for the PointerType class.

#include "PointerType.h"
#include "SymTable.h"

extern SymTable symTable;

//Constructor for a pointer with a name, which is the typical case for
//a pointer typedef. Note that the type it points to is not provided;
//since forward references are legal, the type must be filled in later.
PointerType::PointerType(string id) : AbstractType(id)
{
}

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
    //lookupType() handles error checking.
    type = symTable.lookupType(typeName);
}

//Format this object as a string in this format:
//  cellPtr ^ cell
string PointerType::toString()
{
    return identifier + " ^ " + type->toIdentTypeString();
}
