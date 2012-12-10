// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This file contains the method definitions of the AbstractType class.

#include <string>
#include "AbstractType.h"
#include "actions.h"

//This constructor is used for nameless types that do not refer to another
//type. The empty string represents the lack of a name.
AbstractType::AbstractType() : Symbol("")
{
    this->type = NULL;
}

//This constructor is used for named types that do not refer to another
//type. An example is "integer". Also used for pointers, since their
//type is not initially known.
AbstractType::AbstractType(string id) : Symbol(id)
{
    this->type = NULL;
}

//This constructor is used for nameless types that refer to another type.
//An example is an array. The empty string represents the lack of a name.
AbstractType::AbstractType(AbstractType *type) : Symbol("")
{
    this->type = type;
}

//This constructor is used when creating a named type, such as a typedef:
//
//    myInt = Integer;
//
//The alias "myInt" is the first parameter to the constructor and the second
//parameter is a pointer to the "integer" object in the symbol table.
AbstractType::AbstractType(string id, AbstractType *type) : Symbol(id)
{
    this->type = type;
}

AbstractType::~AbstractType()
{
    //If the type has no name, it is not in the symbol table and
    //will not be freed by it.
    //TODO: Segmentation fault going on here
    //if (type && type->identifier == "")
    //    delete type;
}

//For typedefs, the name of the c in C++ is the alais created by the typedef,
//the name is the same in C and Pascal. For these cases, just return the 
//type's identifier. However, the base types have different names in Pascal
//and C. This method is overriden in the BaseType class.
string AbstractType::cTypeName() 
{
    return identifier;
}

//Generate code for this abstract type, e.g., for a typedef definition or a
//variable declaration.
void AbstractType::generateCode(string ident)
{
    //Example 1. typedef
    //  cellptr list
    //
    //Example 2. base type
    //  int count
    cout << cTypeName() << " " << ident;
}

//Generate a definition of this type.
void AbstractType::generateDefinition(string ident)
{
    generateCode(ident);
}

//Return true because this class and all its subclasses are type objects.
bool AbstractType::isType()
{
    return true;
}

//If this AbstractType is a typedef, the type it points to might be an array.
//Otherwise, this is not an array.
bool AbstractType::isArrayType() 
{
    if (type)
        return type->isArrayType();

    return false;
}

//An AbstractType is not a record type, even if it is a typedef.
bool AbstractType::isRecordType() 
{
    return false;
}

//Named types are typedefs, unnamed types are composite types used in variable
//declarations. For example, the statement:
//
//  x : array [1..10] of integer
//
//the "array [1..10] of integer" is a composite type, but has no special name.
bool AbstractType::isNamedType()
{
    return identifier != "";
}

//Return the underlying type of this type. Thus, if this AbstractType is
//actually an integer BaseType, return that; but if this AbstractType is a
//typedef that refers to an integer BaseType, still return integer BaseType.
AbstractType *AbstractType::getType()
{
    //TODO: It might make sense to add a separate class for typedefs. There
    //      was one originally, but it was deleted for being "unnecessary".

    //If this is typedef, we want to return the type which it aliases.
    if (className() == "AbstractType")
        return type->getType();
        
    return this;
}

//Push this type onto the type stack.
void AbstractType::push() 
{
    tracker.push(identifier, this);
}

// <, >, <=, =, et cetera.
bool AbstractType::compatible(AbstractType *otherType, int opToken) 
{
    //In general, arbitrary types are not compatible.
    return false;
}
