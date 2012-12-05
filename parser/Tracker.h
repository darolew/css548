// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This file contains the definition of the Tracker class. 
//The tracker is used to tracking types and type checking.

#ifndef TRACKER_H
#define TRACKER_H

#include <iostream>
#include <list>
#include "ArrayType.h"
#include "Variable.h"
#include "Parameter.h"
#include "Function.h"
#include "PointerType.h"
#include "RecordType.h"
#include "SetType.h"

using namespace std;

class Tracker {
private:
   list<AbstractType *> typeStack;
   list<int> functionParamCountStack;
   AbstractType *pop();   
   AbstractType *peek();
   void push(AbstractType *);

public:
    void push(string);
    void deref();
    void binaryOp(int);
    bool isArrayInContext();
    string arrayIndexOffset(int);
    void debugPrint();
    
};

#endif
