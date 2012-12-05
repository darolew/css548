// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This file contains the method definitions of the SetType class.

#include <list>
#include "SetType.h"
#include "main.h"

//The constructor for the SetType class.
SetType::SetType(Range range) : AbstractType()
{
    this->range = range;
}
