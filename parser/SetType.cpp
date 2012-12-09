// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This file contains the method definitions of the SetType class.

#include "SetType.h"
#include "BaseType.h"
#include "y.tab.h"

//The constructor for the SetType class.
SetType::SetType(Range range) : AbstractType()
{
    this->range = range;
}

//Generate code for a set, assumed to be an integer set.
void SetType::generateCode(string ident)
{
    cout << "IntSet " << ident;
}

//Return whether an integer value is legal, given the defined range of this
//set type.
bool SetType::legalValue(int value)
{
    return value >= range.low && value <= range.high;
}

//Set types have overloaded operators for +, -, and *.
bool SetType::legalMathOp(int opToken)
{
    return opToken == yplus || opToken == yminus || opToken == ymultiply;
}

//Sets can be compared with or assigned to other sets. The "in" operator is
//legal with integers.
bool SetType::relationCompatible(AbstractType *otherType, int opToken)
{
    if (opToken == ylessequal || opToken == yassign)
        return otherType->isSet();
    
    if (opToken == yin) {
        BaseType *bt = dynamic_cast<BaseType*>(otherType);
        return bt && bt->cTypeName() == "int";
    }
    
    return false;
}

//A set is a set.
bool SetType::isSet()
{
    return true;
}
