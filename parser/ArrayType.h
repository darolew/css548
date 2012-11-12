// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// TODO: Write description of this file.
#ifndef ARRAYTYPE_H
#define ARRAYTYPE_H

#include <list>
#include <sstream>
#include "AbstractType.h"
#include "Range.h"
#include "main.h"
using namespace std;

class ArrayType : public AbstractType {
    list<Range> ranges;

public:
    ArrayType(AbstractType *type, list<Range> ranges)
        : AbstractType("", type)
    {
        this->ranges = ranges;
    }
    
    virtual string toString(void)
    {
        stringstream ss (stringstream::in | stringstream::out);
        ss << identifier << " ";
        list<Range>::iterator it = ranges.begin();
        for (; it != ranges.end(); it++) {
        	if (it != ranges.begin())
        		ss << ",";
            ss << it->low.str << ".." << it->high.str;
        }
        ss << " " << type->toIdentTypeString();
        return ss.str();
    }
    
    //Does nothing
    // virtual ~ArrayType()
    // {
    // }
};

#endif
