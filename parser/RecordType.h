// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// TODO: Write description of this file.
#ifndef RECORDTYPE_H
#define RECORDTYPE_H

#include <list>
#include "AbstractType.h"
#include "main.h"

class RecordType : public AbstractType {
    list<Variable> fields;

public:
    RecordType(list<Variable> fields) : AbstractType("")
    {
        list<Variable>::iterator it = fields.begin();
        for (; it != fields.end(); it++)
	        this->fields.push_front(*it);
    }
    
    virtual string toString()
    {
        stringstream ss (stringstream::in | stringstream::out);
        ss << nlindent();
        list<Variable>::iterator it = fields.begin();
        for (; it != fields.end(); it++)
			ss << "    " << it->toString();
        return ss.str();
    }
    
};

#endif
