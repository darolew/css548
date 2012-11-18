// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This file contains the method definitions of the ArrayType class.

#include <sstream>
#include "ArrayType.h"
#include "main.h"
#include "y.tab.h"

//The constructor for an ArrayType. The passed-in type is the type of the
//array elements, while the ranges list represents the array dimensions.
ArrayType::ArrayType(AbstractType *type, list<Range> ranges)
    : AbstractType(type)
{
    this->ranges = ranges;
}

//Return the array type as an identifier, followed its ranges and type.
//Used only for printST() and debugging.
string ArrayType::toString()
{
    stringstream ss (stringstream::in | stringstream::out);
    ss << identifier << " ";
    list<Range>::iterator it = ranges.begin();
    for (; it != ranges.end(); it++) {
        if (it != ranges.begin())
            ss << ",";
        ss << it->low.str << ".." << it->high.str;
    }
    if (type)
        ss << " " << type->toIdentTypeString();
    else
        ss << " <bogus_type>" << nlindent();
    return ss.str();
}

//TODO:
//  - Support character ranges
//  - Support constant ranges
//  - Make sure both ranges are the same type
//  - Make sure low <= high
void ArrayType::generateCode(ostream &out, string ident)
{
    type->generateCode(out, ident);
    list<Range>::iterator it = ranges.begin();
    for (; it != ranges.end(); it++) {
        Range r = *it;
        if (r.low.token != ynumber || r.high.token != ynumber) {
            cerr << "error -- unsupported array type" << endl;
            return;
        }
        
        int low, high, size;
        low = atoi(r.low.str.c_str());
        high = atoi(r.high.str.c_str());
        size = (high - low + 1);
        out << "[" << size << "]";
    }
}
