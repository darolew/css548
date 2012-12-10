// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This file contains the method definitions of the Parameter class.

#include "Parameter.h"

//A constructor for a Parameter. Saves the id and type.
Parameter::Parameter(string id, AbstractType *type, bool varflag)
    : Variable(id, type)
{
    this->isReference = varflag;
}

//Generate the code for this parameter, as it will exist in the function
//definition.
void Parameter::generateDefinition(string ident)
{
    //If a variable is declared with an unknown identifier, as in sterror.p:
    //    aaa: undefinedType; 
    //currType will be NULL and cannot be resolved.
    if (!type)
        cout << "<UNDEFINED TYPE>" << identifier;
    else {
        type->generateCode("");
        if(isReference)
            cout << "&";
        cout << identifier;
    }
}
