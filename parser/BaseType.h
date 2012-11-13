// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// BaseType represents one of the fundamental data types boolean; integer;
// real; char/string.
//
// This class is too small to deserve it's own cpp file.
 
#ifndef BASETYPE_H
#define BASETYPE_H

#include "AbstractType.h"
using namespace std;

class BaseType : public AbstractType {
    string cName;
 
public:
    BaseType(string ident, string cName) : AbstractType(ident)
    {
        this->cName = cName;
    }
};

#endif
