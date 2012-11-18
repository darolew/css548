// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This file contains the method definitions of the Variable class.

#include <cassert>
#include "Variable.h"

//A constructor for a Variable. Saves the id and type.
Variable::Variable(string id, AbstractType *type) : Symbol(id)
{
    this->type = type;
}

//A destructor for a Variable.
Variable::~Variable()
{
    //If the type has no name, it is not in the symbol table and
    //will not be freed by it.
//    if (type && type->identifier == "")
//        delete type;
}

void Variable::generateCode(string ident)
{
    //Example 1. ident==list
    //  cellptr list
    type->generateCode(ident);
}

//TODO: Most places the parameter "ident" is not used.
//It makes the code a little harder to follow when there is an ident parameter,
//but it doesn't hurt anything. Consider writing virtual function
//Symbol::generateCode() that just calls generateCode(identifier)
//Definitely a low priority.
void Variable::generateDefinition(string ident)
{
    //Ignore parameter "ident"
    generateCode(identifier);
}

//Returns a string representation of a variable.
string Variable::toString()
{
    string s = identifier + " ";
    if (type)
        s += type->toIdentTypeString();
    else
        s += "<unknown_type>" + nlindent(); // should never see this
    return s;
}
