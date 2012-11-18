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
PointerType::PointerType(string id, string pointeeName) : AbstractType(id)
{
    this->pointeeName = pointeeName;
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

void PointerType::resolve()
{
    type = symTable.lookupType(pointeeName);
}

void PointerType::generateCode(ostream &out, string ident)
{
    if (identifier != "")
        out << identifier << " " << ident;
    else
        out << pointeeName << " *" << ident;
}

void PointerType::generateDefinition(ostream &out, string ident)
{
    AbstractType *pt = type; // initialize to class's type
    
    //If the pointer does not yet have a known pointee type, check the
    //current scope for the pointee type name. This tells us whether
    //the type is a foward reference.
    if (!pt)
        pt = (AbstractType*)symTable.lookup(symTable.front(), pointeeName);
    
    //If the type is known, this is not a forward reference; simply
    //print the pointee name. If the type is not known, this is a
    //foward reference, and the only forward references we support
    //are for records, so output the "struct" keyword.
    if (pt)
        out << pointeeName << " *" << ident;
    else
        out << "struct " << pointeeName << " *" << ident;
}

//Format this object as a string in this format:
//  cellPtr ^ cell
string PointerType::toString()
{
    if (!type)
        return identifier + " ^ <bogus_type>" + nlindent();

    return identifier + " ^ " + type->toIdentTypeString();
}
