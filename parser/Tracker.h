// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This file contains the definition of the Tracker class. The tracker is used
// to tracking types and perform type checking.

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

typedef struct Frame {
    AbstractType *type;
    string str;
} Frame;

using namespace std;

class Tracker {

private:
   list<Frame> typeStack;
   Frame pop();   
   Frame peek();
   void push(Frame);

public:
    void push(string, AbstractType*);
    void push(string);
    void event_AccessRecordField(string);
    void event_Deref();
    void binaryOp(int);
    bool arrayInContext();
    bool functionInContext();
    void endParameter(int);
    void endArrayDimension(int);
    string arrayIndexOffset(int);
    void debugPrint(string = "");    
    void event_FunctionCall();
    void event_RelationalOp();
    void event_MathOp(int);
};

#endif
