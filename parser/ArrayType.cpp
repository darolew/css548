// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This file contains the method definitions of the ArrayType class.

#include <sstream> 
#include <stdio.h>
#include "ArrayType.h"
#include "main.h"
#include "y.tab.h"

//The constructor for an ArrayType. The passed-in type is the type of the
//array elements, while the ranges list represents the array dimensions.
ArrayType::ArrayType(AbstractType *type, list<Range> ranges)
    : AbstractType(type)
{
    list<Range>::iterator it = ranges.begin();
    for (; it != ranges.end(); it++)
        this->ranges.push_back(*it);
}

//Zero-based counting
void ArrayType::offsetForDim(int dim)
{
//TODO: This needs to be tested for character array bounds
//TODO: Refactor this mess.
    //Validate input.
    if (dim < 0 || dim > numDimensions()-1) {
        if (type->getType()->isArrayType()) {
            ArrayType *a = (ArrayType*)type->getType();
            a->offsetForDim(dim - (numDimensions()-1));
            return;
        }
        cout << "***ERROR: invalid array access for dim " << dim << endl;
        return;
    }
    
    cout << "-(" << ranges[dim].low << ")";
}

//TODO:
//  - Support character ranges
//  - Support constant ranges
//  - Make sure both ranges are the same type
//  - Make sure low <= high
void ArrayType::generateCode(string ident)
{
    type->generateCode(ident);
    vector<Range>::iterator it = ranges.begin();
    for (; it != ranges.end(); it++) {
        Range r = *it;
        int size = (r.high - r.low + 1);
        cout << "[" << size << "]";
    }
}

bool ArrayType::isArrayType() 
{
    return true;
}

void ArrayType::resolve()
{
    type->resolve();
}

int ArrayType::numDimensions() 
{
    return ranges.size();
}

bool ArrayType::relationCompatible(AbstractType *otherType, int opToken) 
{
    //TODO: Check dimensions and ranges.
    //NOTE: Arrays are (probably) not assignment compatible in Pascal.
    //This method will return true, so it should not be used to assignment
    //compatiblity.
    
    return className() == className();
    //&&  getType()->relationCompatible(otherType->getType(), opToken);
}
