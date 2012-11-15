// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// BaseType represents one of the fundamental Pascal data types:
//     - boolean;
//     - integer;
//     - real;
//     - char (used for strings).
//
// This class is too small to deserve its own cpp file.

#ifndef BASETYPE_H
#define BASETYPE_H

#include "AbstractType.h"
using namespace std;

class BaseType : public AbstractType {
    //The name of the equivalent type in C++, e.g., "int" for integer.
    string cName;

public:
    BaseType(string id, string cName) : AbstractType(id)
    {
        this->cName = cName;
    }
    
    virtual string className() 
    {
        return "BaseType";
    }
};

#endif
