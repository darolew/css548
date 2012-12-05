// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This file contains the method definitions of the Variable class.

#include <cassert>
#include "Variable.h"
#include "PointerType.h"
#include "actions.h"

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

void Variable::generateNewStatement()
{
    cout << identifier << " = new ";
    
    if(!type->isPointer())
        cout << "***ERROR " << type->identifier << " is not a pointer\n" << endl;

    AbstractType *pointeeType = type->type;
    pointeeType->generateCode("");
}

bool Variable::isArray()
{
    if (type)
        return type->isArrayType();
    else return false;
}

bool Variable::isRecord()
{
    if (type)
        return type->isRecordType();
    else return false;
}

AbstractType *Variable::getType() 
{
    return type->getType();
}

void Variable::push() 
{
    tracker.push(identifier, getType());
}
