// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// TODO: Write description of this file.
#ifndef VARIABLE_H
#define VARIABLE_H

#include "AbstractType.h"
using namespace std;

class Variable: public Symbol {
    AbstractType *type;

public:
    Variable(string id, AbstractType *type) : Symbol(id)
    {
        this->type = type;
    }
    
    virtual string toString(void)
    {
    	string s = identifier + " ";
        if (type)
            s += type->toIdentTypeString();
        else
			s += "<unknown_type>" + nlindent();
		return s;
    }
    
    virtual ~Variable() 
    {
    	//If the type has no name, it is not in the symbol table and
    	//will not be freed by it.
    	if (type && type->identifier == "")
    		delete type;
    }
};

#endif
