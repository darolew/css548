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

//Print an offset that will convert the array index into a zero-based index.
//For example, if the array starts at one, substract one from all indices.
void ArrayType::offsetForDim(int dim)
{
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

//Generate the definition of an array. The ranges used by Pascal are converted
//into a raw size, for the array will be zero-based.
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

//An ArrayType is an array.
bool ArrayType::isArrayType() 
{
    return true;
}

//We have the element type in an incomplete form (the ID is known, but whether
//that is a valid type is not), so do a lookup and resolve it.
void ArrayType::resolve()
{
    type->resolve();
}

//Returns the number of dimensions in this array.
int ArrayType::numDimensions() 
{
    return ranges.size();
}

//Check that two array types are compatible. This is mostly used for parameter
//type checking.
bool ArrayType::compatible(AbstractType *otherType, int opToken) 
{
    //This implementation is cute, but it hardly counts as real type checking
    //for arrays.
    //
    //TODO:
    //  - This does not check that the element type is the same!
    //  - This does not check that the dimensions/ranges are the same!
    //  - This does not return false for operators that are illegal for
    //    arrays, like '<='!
    return className() == otherType->className();
    //&&  getType()->compatible(otherType->getType(), opToken);
}
