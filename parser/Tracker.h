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

class Tracker {
private:
   list<AbstractType *> typeStack;
   //list<int> arrayDimStack;
   list<int> functionParamCountStack;
   bool arrayInContext;
   AbstractType *pop();   
   AbstractType *peek();
   void push(AbstractType *);

public:
    Tracker();
    void push(string);
    void deref();
    void binaryOp(int);
    void startArrayAccess();
    void endArrayAccess();
    bool isArrayInContext();
    string arrayIndexOffset(int);
    void endFunctionCall();
    void debugPrint();
    
};

#endif
