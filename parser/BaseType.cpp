// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This file contains the method definitions of the BaseType class.

#include "BaseType.h"
#include "y.tab.h"
#include "actions.h"

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

bool BaseType::relationCompatible(AbstractType *otherType) 
{
    //Pointers and null can be compared
    if (token == ynil) 
        return otherType->isPointer();

    BaseType *type = dynamic_cast<BaseType *>(otherType);
    if (!type)
        return false;
        
    //Permitted comparisons
    //  chars and chars
    if (token == ystring && type->token == ystring)
        return true;
        
    //  booleans and booleans
    bool left = identifier == "false"  || identifier == "true";
    bool right = type->identifier == "false" || type->identifier == "true";
    if (left && right)
        return true;

    //  integers and integers
    //  reals and reals
    //   integers and reals can be compared
    bool number1 = token == yinteger || token == yreal;
    bool number2 = type->token == yinteger || type->token == yreal;
    return number1 == number2;
}

BaseType *BaseType::getMathType(BaseType *left, BaseType *right, int op)
{
    int l = left->token - offset;
    int r = right->token - offset;
     
    int result = mathTable[l][r][op-offset];
    
    //Invalid operation
    if (!result)
        return NULL;

    //Go find the correct instance of base type.
    return symTable.lookupSIT(result);
}
