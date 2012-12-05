// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This class represents a Pascal set.

#ifndef SETTYPE_H
#define SETTYPE_H

#include <list>
#include <sstream>
#include "AbstractType.h"
#include "Range.h"
#include "main.h"
using namespace std;

class SetType : public AbstractType {

private:
    Range range;  // range of the set (e.g., 0..9)

public:
    SetType(Range);
};

#endif
