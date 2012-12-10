// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This file contains the method definitions of the Variable class.

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

//Generate code for this variable. Creates a variable with this variable's
//type.
void Variable::generateCode(string ident)
{   
    //If a variable is declared with an unknown identifier, as in sterror.p:
    //    aaa: undefinedType; 
    //type will be NULL.
    if (!type) 
        cout << ident;
    else
        type->generateCode(ident);
}

//Generate the definition of this variable.
//
//TODO: Most places the parameter "ident" is not used. It makes the code a
//      little harder to follow when there is an ident parameter, but it
//      doesn't hurt anything. Consider writing virtual function
//      Symbol::generateCode() that just calls generateCode(identifier)
//      Definitely a low priority.
void Variable::generateDefinition(string ident)
{
    //Ignore parameter "ident"
    generateCode(identifier);
}

//Generate code which initializes this variable (which should be a pointer) to
//a new, dynamically allocated instance of itself.
void Variable::generateNewStatement()
{
    cout << identifier << " = new ";
    
    if(!type->isPointer())
        cout << "***ERROR " << type->identifier << " is not a pointer\n" << endl;

    AbstractType *pointeeType = type->type;
    pointeeType->generateCode("");
}

//The variable is an array if its type is.
bool Variable::isArray()
{
    return type && type->isArrayType();
}

//The variable is a record of its type is.
bool Variable::isRecord()
{
    return type && type->isRecordType();
}

//Return the underlying type of this variable.
AbstractType *Variable::getType() 
{
    return type->getType();
}

//Push this variable's type onto the type stack.
void Variable::push() 
{
    tracker.push(identifier, getType());
}
