// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This file contains the method definitions of the BaseType class.

#include "BaseType.h"
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
