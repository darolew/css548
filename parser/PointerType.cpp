// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This file contains method definitions for the PointerType class.

#include "PointerType.h"
#include "SymTable.h"
#include "main.h"
#include "actions.h"

//Constructor for a pointer with a name, which is the typical case for
//a pointer typedef. Note that the type it points to is not provided;
//since forward references are legal, the type must be filled in later.
PointerType::PointerType(string id, string pointeeName) : AbstractType(id)
{
    this->pointeeName = pointeeName;
}

//Pointer types are special: they are allowed to be declared before
//the type they point to has been declared. Thus, this method skips
//the symbol checking of the superclass method.
bool PointerType::insert()
{
    return Symbol::insert();
}

void PointerType::resolve()
{
    type = symTable.lookupType(pointeeName);
}

void PointerType::generateCode(string varName)
{
    //If this is typdef whose type is a pointer, like "cellptr", print the type
    //name and then the name of a variable (if there is one). No asterisk is
    //needed because the asterisk was part of the typedef.
    if (isNamedType()) {
        cout << identifier << " ";

        //Do not print a space if the parameter varName is an empty string.
        //varName will be an empty parameter when this type is used outside
        //the context of a variable declaration, such as the type of a paramter
        //in a function declaration, like "void cleanup(cellptr* &list)" where
        //cellptr is the PointerType object's identifier.
        if (!varName.empty())
            cout << varName;

        return;
    }

    //If the pointee type object exists, ask it to print its type name 
    //and then print the pointer symbol (*) and finally the name of 
    //a variable. This is used in declaring variables that point to other
    //types. For example:
    //
    // int *temp2                                                                                                                                                             
    // bool *testarray[3][6]
    //
    if (type) 
        type->generateCode("");
    else 
        cout << cPointeeName();
        
    cout << "*" << varName;
}

void PointerType::generateDefinition(string ident)
{
//TODO: This code does not appear to be used
/*
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
    if (pt) {
    //TODO: we are not hitting this branch in our test case
        pt->generateCode("");
        cout << " *" << ident; 
    } else
        cout << "struct " << pointeeName << " *" << ident;
*/
    
    //Only the last line of the commented-out section (above) was being used.
    //Here it is:
    cout << "struct " << pointeeName << " *" << ident;
}

bool PointerType::isPointer()
{
    return true;
}

//Account for base type whose names are diffferent in Pascal and C
string PointerType::cPointeeName()
{
    //Check if the pointee's name is in the SIT.
    Symbol *sym = symTable.lookup(symTable.SIT(), pointeeName);
    AbstractType *pt = (AbstractType*)sym;    
    if (pt) {
        //The pointee is in the SIT. Return its C name. For example, if the
        //type is "integer", the proper C name is not "integer", but "int".
        return pt->cTypeName();
    } else {
        //The pointee is not in the SIT. Return the pointee name stored in this
        //object. This is only encountered when there is a pointer to a typedef
        //alias. For example:
        //
        //  typedef double footype;                                                                                                                                                                                  
        //  footype *temp3;     
        //
        return pointeeName;
    }
}

bool PointerType::relationCompatible(AbstractType *otherType) 
{
    //pointer to pointer comparison
    bool pointerToPointer = compatible(otherType);

    //pointer and null comparison
    bool pointerToNull = otherType->relationCompatible(this);
   
    return pointerToPointer || pointerToNull;
}
