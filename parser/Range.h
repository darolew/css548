// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// TODO: Write description of this file.
// TODO: Rename this header.
#ifndef RANGE_H
#define RANGE_H

typedef struct Terminal {
    string str;
    int token;
    char unaryOp;
} Terminal;

typedef struct Range {
    Terminal low;
    Terminal high;
} Range;

#endif
