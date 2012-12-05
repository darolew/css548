// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This file contains the method definitions of the AbstractType class.

#include "AbstractType.h"
#include <string>

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
    //TODO: Seg fault going on here
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

void AbstractType::generateCode(string ident)
{
    //Example 1. typedef
    //  cellptr list
    //
    //Example 2. base type
    //  int count
    cout << cTypeName() << " " << ident;
}

void AbstractType::generateDefinition(string ident)
{
    generateCode(ident);
}

//Return true because this class and all its subclasses are type objects.
bool AbstractType::isType()
{
    return true;
}

bool AbstractType::isArrayType() 
{
    if (type)
        return type->isArrayType();

    return false;
}

bool AbstractType::isRecordType() 
{
    return false;
}


//Named types are typedefs
//Unamed types are composite types used in variable declarations.
//For example, in the statement:
//
//  x : array [1..10] of integer
//
//  the "array [1..10] of integer" is a composite type, 
//but has no special name.
bool AbstractType::isNamedType()
{
    return identifier != "";
}

AbstractType* AbstractType::getType()
{
    //TODO: Add a class for type defs.
    //If this is type def, we want to return the point which it alaises.
    if (className() == "AbstractType")
        return type->getType();
        
    return this;
}
