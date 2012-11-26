// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This file contains the method definitions of the Parameter class.

//#include <cassert>
#include "Parameter.h"

//A constructor for a Variable. Saves the id and type.
Parameter::Parameter(string id, AbstractType *type, bool varflag)
    : Variable(id, type)
{
    this->isReference = varflag;
}

void Parameter::generateDefinition(string ident)
{
    type->generateCode("");
    if(isReference)
        cout << "&";
    cout << identifier;
}
