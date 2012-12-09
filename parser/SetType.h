// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This class represents a Pascal set.

#ifndef SETTYPE_H
#define SETTYPE_H

#include "AbstractType.h"
#include "Range.h"
using namespace std;

class SetType : public AbstractType {

private:
    Range range;  // range of the set (e.g., 0..9)

public:
    SetType(Range);
    void generateCode(string);
    bool legalValue(int);
    bool legalMathOp(int);
    virtual bool relationCompatible(AbstractType*, int);
    virtual bool isSet();
};

#endif
