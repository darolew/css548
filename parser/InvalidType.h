// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// ...

#ifndef INVALIDTYPE_H
#define INVALIDTYPE_H

#include "AbstractType.h"
using namespace std;

class InvalidType : public AbstractType {

public:
    InvalidType();
    virtual bool compatible(AbstractType*, int);
};

#endif
