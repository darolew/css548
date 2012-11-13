// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// TODO: Write description of this file.

#ifndef SETTYPE_H
#define SETTYPE_H

#include <list>
#include <sstream>
#include "AbstractType.h"
#include "Range.h"
#include "main.h"
using namespace std;

class SetType : public AbstractType {
    Range range;

public:
    SetType(Range range) : AbstractType("")
    {
        this->range = range;
    }

    virtual string toString(void)
    {
        stringstream ss (stringstream::in | stringstream::out);
        ss << identifier << " ";
        ss << range.low.str << ".." << range.high.str << nlindent();
        return ss.str();
    }
};

#endif
