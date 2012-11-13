// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This file contains the method definitions of the ArrayType class.

#include "ArrayType.h"

ArrayType::ArrayType(AbstractType *type, list<Range> ranges)
    : AbstractType("", type)
{
    this->ranges = ranges;
}

//Print the array type as an identifier, followed its ranges and type.
string ArrayType::toString(void)
{
    stringstream ss (stringstream::in | stringstream::out);
    ss << identifier << " ";
    list<Range>::iterator it = ranges.begin();
    for (; it != ranges.end(); it++) {
        if (it != ranges.begin())
            ss << ",";
        ss << it->low.str << ".." << it->high.str;
    }
    ss << " " << type->toIdentTypeString();
    return ss.str();
}
