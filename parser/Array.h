// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// TODO: Write description of this file.
#ifndef ARRAY_H
#define ARRAY_H

#include <list>
#include <sstream>
#include "OfType.h"
using namespace std;

typedef struct Terminal {
    string str;
    int token;
} Terminal;

typedef struct Range {
    Terminal low;
    Terminal high;
} Range;

class Array : public OfType {
    list<Range> ranges;

public:
    Array(string ident, list<Range> ranges, string typeName)
        : OfType(ident, typeName)
    {
        this->ranges = ranges;
    }
    
    virtual string toString(void)
    {
        stringstream ss (stringstream::in | stringstream::out);
        ss << identifier << "[";
        list<Range>::iterator it = ranges.begin();
        for (; it != ranges.end(); it++)
            ss << it->low.str << ".." << it->high.str << ",";
        ss << "] of " << type->toString();
        return ss.str();
    }
};

#endif
