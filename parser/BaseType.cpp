// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This file contains the method definitions of the BaseType class.

#include "BaseType.h"
#include "y.tab.h"
#include "actions.h"

using namespace std;

//Construct a BaseType.
BaseType::BaseType(string id, string cName, int token) : AbstractType(id)
{
    this->cName = cName;
    this->token = token; 
}

//Return the C++ equivalent name of the base type.
string BaseType::cTypeName() 
{
    return cName;
}

//Generate code (like a variable declaration) for this type.
void BaseType::generateCode(string ident)
{
    cout << cName << " " << ident;
}

//Returns whether this BaseType could legally be used to index an array.
bool BaseType::isLegalArrayIndexType()
{
    //Only integers and characters can be used as array indexes
    return token == yinteger || token == ystring;
}

//Returns whether this BaseType is a string.
bool BaseType::isStringType()
{
    //Only integers and characters can be used as array indexes
    return token == ystring;
}

//Type checking for BaseType.
bool BaseType::compatible(AbstractType *otherType, int opToken) 
{
    //Pointers and null can be compared
    if (token == ynil)
        return otherType->isPointer();
        
    if (opToken == yin) {
        SetType *st = dynamic_cast<SetType*>(otherType);
        return token == yinteger && st != NULL;
    }

    BaseType *type = dynamic_cast<BaseType *>(otherType);
    if (!type)
        return false;
        
    //Permitted comparisons
    //  chars and chars
    if (token == ystring && type->token == ystring)
        return true;
        
    //  booleans and booleans
    if (isBooleanType() && type->isBooleanType())
        return true;

    //  integers and integers
    //  reals and reals
    //   integers and reals can be compared
    bool number1 = token == yinteger || token == yreal;
    bool number2 = type->token == yinteger || type->token == yreal;
    return number1 && number2;
}

//Returns the resulting type from a math operation on the given operands.
BaseType *BaseType::getMathType(BaseType *left, BaseType *right, int op)
{
    int l = left->token - yTokOffset;
    int r = right->token - yTokOffset;

    int result = mathTable[l][r][op-yTokOffset];
    
    //Invalid operation
    if (!result)
        return NULL;

    //Go find the correct instance of base type.
    return symTable.lookupSIT(result);
}

//Returns whether this BaseType is a boolean.
bool BaseType::isBooleanType()
{
	string id = identifier; // shorten line
	return id == "boolean" || id == "true" || id == "false";
}
