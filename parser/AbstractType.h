// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// TODO: Write description of this file.
#ifndef ABSTRACTTYPE_H
#define ABSTRACTTYPE_H

#include "Symbol.h"
#include "main.h"

class AbstractType : public Symbol {

public:
    AbstractType *type;

    AbstractType(string id) : Symbol(id) {}

    AbstractType(string id, AbstractType *type) : Symbol(id)
    {
        this->type = type;
    }
    
    virtual bool isType() 
    {
        return true;
    }

    virtual string toIdentTypeString()
    {
        if (identifier != "")
            return identifier + nlindent();
        
        return toString();
    }    
    
    virtual string toString(void)
    {
        if (type)
            return identifier + " " + type->toIdentTypeString();
        return identifier + nlindent();
    }
    
    //This desctructor did not help anything.
    // virtual ~AbstractType()
    // {
        // if (type)
            // delete type;
    // }
	virtual ~AbstractType() 
    {
    	//If the type has no name, it is not in the symbol table and
    	//will not be freed by it.
    	if (type && type->identifier == "")
    		delete type;
    }
};

#endif
