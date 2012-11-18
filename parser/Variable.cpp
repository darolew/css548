// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This file contains the method definitions of the Variable class.

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

void Variable::generateCode(ostream &out, string ident)
{
    type->generateCode(out, ident);
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
