// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This file contains the method definitions of the InvalidType class.

#include "InvalidType.h"

//Construct an InvalidType (nothing to be done).
InvalidType::InvalidType()
{
}

//Invalid types are incompatible with all types, including themselves. That is
//the only reason this class exists; if there is an error in the code, we want
//to push a type onto the type stack that is invalid for everything.
bool InvalidType::compatible(AbstractType *type, int opToken)
{
    return false;
}
