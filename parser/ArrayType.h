// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// ArrayType defines an array. For example, the information to the right
// of the equals sign is captured in an ArrayType object:
//
//  int3D = array [1..5,2..3,0..2] of integer;
//
// An array constist of a list of ranges, a type (in this case it is integer)

#ifndef ARRAYTYPE_H
#define ARRAYTYPE_H

#include <list>
#include <sstream>
#include "AbstractType.h"
#include "Range.h"
#include "main.h"
using namespace std; 

class ArrayType : public AbstractType {
    list<Range> ranges;

public:
    ArrayType(AbstractType *, list<Range> );
    virtual string toString();
    
    //Does nothing
    // virtual ~ArrayType()
    // {
    // }
};

#endif
