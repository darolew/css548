// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This file contains the method definitions of the BaseType class.

#include "BaseType.h"
#include "y.tab.h"

using namespace std;

BaseType::BaseType(string id, string cName, int token) : AbstractType(id)
{
    this->cName = cName;
    this->token = token; 
}

string BaseType::cTypeName() 
{
    return cName;
}

void BaseType::generateCode(string ident)
{
    cout << cName << " " << ident;
}

//Return true is the other type is a base type and it represents the same
//kind of thing -- integer, real, boolean, char.
bool BaseType::compatible(BaseType *otherType)
{
      return AbstractType::compatible(otherType) && token == otherType->token;
}

bool BaseType::isLegalArrayIndexType()
{
        //Only integers can be used as array indexes
        return token == yinteger;
}
