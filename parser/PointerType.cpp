// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This file contains method definitions for the PointerType class.

#include "PointerType.h"
#include "SymTable.h"
#include "main.h"

extern SymTable symTable;

//Constructor for a pointer with a name, which is the typical case for
//a pointer typedef. Note that the type it points to is not provided;
//since forward references are legal, the type must be filled in later.
PointerType::PointerType(string id) : AbstractType(id)
{
}

string PointerType::className() 
{
    return "PointerType";
}

//Pointer types are special: they are allowed to be declared before
//the type they point to has been declared. Thus, this method skips
//the symbol checking of the superclass method.
bool PointerType::insertInto()
{
    return Symbol::insertInto();
}

//Format this object as a string in this format:
//  cellPtr ^ cell
string PointerType::toString()
{
    if (!type)
        return identifier + " ^ <bogus_type>" + nlindent();

    return identifier + " ^ " + type->toIdentTypeString();
}
