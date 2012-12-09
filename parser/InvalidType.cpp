// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// ...

#include "InvalidType.h"

InvalidType::InvalidType()
{
}

//Invalid types are incomparable with all types, including themselves.
bool InvalidType::relationCompatible(AbstractType *type, int opToken)
{
	return false;
}
