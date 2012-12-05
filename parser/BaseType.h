// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// BaseType represents one of the fundamental Pascal data types:
//     - boolean;
//     - integer;
//     - real;
//     - char (used for strings).

#ifndef BASETYPE_H
#define BASETYPE_H

#include "AbstractType.h"
using namespace std;

class BaseType : public AbstractType {
    //The name of the equivalent type in C++, e.g., "int" for integer.
    string cName;
    int token;

public:
    BaseType(string, string, int);
    virtual string cTypeName();
    virtual void generateCode(string);
};

#endif
