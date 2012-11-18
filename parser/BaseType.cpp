// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This file contains the method definitions of the BaseType class.

#include "BaseType.h"
using namespace std;

BaseType::BaseType(string id, string cName) : AbstractType(id)
{
    this->cName = cName;
}
    
string BaseType::className() 
{
    return "BaseType";
}

void BaseType::generateCode(ostream &out, string ident)
{
    out << cName << " " << ident;
}
