// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This file contains the definition of the AbstractType class.
// AbstractType is a sublcass Symbol and the base class of other type classes.
// The most imporant different this class and its parent class is that any
// Abstract Type can be associated with another AbstractType. For example,
// an array type be an integer array, pointer type must be associated with the
// type to which it points, and so on.

// TODO: This class is no longer abstract because we create instances of it.
// Maybe a better name would be Type or AnyType?
#ifndef ABSTRACTTYPE_H
#define ABSTRACTTYPE_H

#include "Symbol.h"
#include "main.h"

class AbstractType : public Symbol {

public:
    //Types can point to other types.
    AbstractType *type;

    //See the .cpp file for descriptions of these methods.
    AbstractType(string id) : Symbol(id) {}
    AbstractType(string, AbstractType*);
    virtual bool isType();
    virtual string toIdentTypeString();
    virtual string toString();
    virtual ~AbstractType();
};

#endif
