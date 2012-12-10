// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// BaseType represents one of the fundamental Pascal data types:
//     - boolean;
//     - integer;
//     - real;
//     - char (used for strings).
//
// NOTE: As a convenience, this class has (somewhat improperly) been extended
//       to contain other symbols, like "true" and "false".

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
    static BaseType *getMathType(BaseType*, BaseType*, int op);
    virtual string cTypeName();
    virtual void generateCode(string);
    bool isLegalArrayIndexType();
    bool isStringType();
    virtual bool compatible(AbstractType*, int);
    
private:
    bool isBooleanType();
};

#endif
