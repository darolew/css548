// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
// 
// This file contains the method definitions of the AbstractType class.

#include "AbstractType.h"
#include <string>

    //This constructor is used when creating a named type, such as a type def:
    //
    //    myName = Integer;
    //The alias, "myName" is the first parameter to the constructor and the 
    //second parameter is a pointer to the "integer" object in the symbol table.
    //
    AbstractType::AbstractType(string id, AbstractType *type) : Symbol(id)
    {
        this->type = type;
    }
    
    //Return true because this class and all its subclasses are type objects.
    //
    bool AbstractType::isType()
    {
        return true;
    } 

    //Print the only the name/alias of the type and no information about 
    //what kind of type it is. For example, if an object represented a type
    //type def:
    //
    //  cellPtr = ^cell;
    //
    // Return "cellPtr"
    //
    string AbstractType::toIdentTypeString()
    {
        if (identifier != "")
            return identifier + nlindent();
        
        return toString();
    }    
    
    //Return the name/alias of the type and details about the type it represents.
    //For example, given the type def:
    //
    //  myint = integer;
    //
    // Return "myint integer"
    //
    string AbstractType::toString()
    {
        if (type)
            return identifier + " " + type->toIdentTypeString();
        return identifier + nlindent();
    }
    
    AbstractType::~AbstractType() 
    {
        //If the type has no name, it is not in the symbol table and
        //will not be freed by it.
        
        //TODO: Seg fault going on here
        //if (type && type->identifier == "")
        //    delete type;
    }
