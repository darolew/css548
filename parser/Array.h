#ifndef ARRAY_H
#define ARRAY_H

#include <list>
#include <string.h>
#include <sstream>
#include "AbstractType.h"

using namespace std;

typedef struct Terminal {
    string str;
    int token;
} Terminal;

typedef struct {
    Terminal low;
    Terminal high;
} Range;

class Array : public AbstractType {

public:
	Array(string ident, list<Range> ranges, AbstractType *elementType)
	    : AbstractType(ident)
	{
	    this->ranges = ranges;
	    this->elementType = elementType;
	}
	
	virtual string toString(void)
	{
	    stringstream ss (stringstream::in | stringstream::out);
	    ss << identifier << "[";
        for (list<Range>::iterator it = ranges.begin(); it != ranges.end(); it++)
            ss << it->low.str << ".." << it->high.str << ",";
        ss << "] of " << elementType->toString();
	    return ss.str();
	}

private:
    list<Range> ranges;
    AbstractType *elementType;
};

#endif
