// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// ArrayType defines an array. For example, the information to the right of the
// equals sign is captured in an ArrayType object:
//
//  int3D = array [1..5,2..3,0..2] of integer;
//
// An array consists of a list of ranges (such as 1..5) and the element type
// (such as integer).

#ifndef ARRAYTYPE_H
#define ARRAYTYPE_H

#include <list>
#include "AbstractType.h"
#include "Range.h"
#include "main.h"
using namespace std;

class ArrayType : public AbstractType {
    //A list of the array ranges, e.g., 1..5, 'a'..'z', etc.
    list<Range> ranges;

public:
    ArrayType(AbstractType *, list<Range>);
    virtual void generateCode(string);
    virtual string toString();
};

#endif
