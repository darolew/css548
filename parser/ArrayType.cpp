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
    for (; it != ranges.end(); it++) {
        this->ranges.push_back(*it);
    }
}

//Zero-based counting
string ArrayType::offsetForDim(int dim)
{
//TODO: This needs to be tested for character array bounds
//TODO: Refactor this mess.
    //Validate input.
    if (dim < 0 || dim > (numDimensions()-1)) {
        cout << "***ERROR: invalid arrary access for dim " << dim << endl;
        return "ERROR";
    }
    
    //Examples:
    //  If the low bound is 5, return -(5)
    //  If the low bound is -5, return -(-5)
    //  If the low bound is +5, return -(+5)
    //  If the low bound is 'a', return -(0)
    Terminal low = ranges[dim].low;
    string expr = "-(";
    if (low.unaryOp)
        expr += low.unaryOp; //Account for unary operator
    
    if (low.token == ystring) {
        int lowBound = low.str[0] - 'a'; //Account for dimensions indexed by chars
        char str[256];
        sprintf(str, "%d", lowBound);
        expr += str;
    }
    else {
        expr += low.str;
    }
    expr += ")";
    return expr;
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
        if (r.low.token != yinteger || r.high.token != yinteger) {
            cout << "***ERROR: unsupported array type" << endl;
            return;
        }
        
        int low, high, size;
        low = atoi(r.low.str.c_str());
        high = atoi(r.high.str.c_str());
        size = (high - low + 1);
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
