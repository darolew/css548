// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// Defines the Terminal and Range structures.

#ifndef RANGE_H
#define RANGE_H

#include <iostream>
using namespace std;

//Value used to indicate that a constant value has no unary operator.
#define NO_UNARY_OP (0)

//Represents a value, typically a constant value. This structure is somewhat
//misnamed, since a constant with a unary operator is actually two terminals.
typedef struct Terminal {
    string str;    // string containing the constant value
    int token;     // token defining the terminal type (e.g, ynumber, ystring)
    char unaryOp;  // unary operator character (zero for none)
} Terminal;

//Represent a range, as used for arrays and sets. A range has a high value and
//a low value.
typedef struct Range {
    int low;
    int high;
} Range;

#endif
