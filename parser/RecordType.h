// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// TODO: Write description of this file.
#ifndef RECORDTYPE_H
#define RECORDTYPE_H

#include <list>
#include "AbstractType.h"

class RecordType : public AbstractType {
    list<Variable*> fields;

public:
    RecordType(list<Variable*> fields) : AbstractType("")
    {
        this->fields = fields;
    }
    
    virtual string toString(void)
    {
        stringstream ss (stringstream::in | stringstream::out);
        ss << identifier << " = record" << endl;
        list<Variable*>::iterator it = fields.begin();
        for (; it != fields.end(); it++) {
            Variable *var = *it;
            ss << "    " << var->toString() << endl;
        }
        ss << "end" << endl;
        return ss.str();   
    }
};

#endif
