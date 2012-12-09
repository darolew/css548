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
    Frame peek();
    Frame peek2();
    void push(Frame);

public:
    //Grow type stack
    void push(string, AbstractType*);
    void push(string);

    Frame pop();
    AbstractType *peekType();

    //Getters
    bool arrayOnTopOfStack();
    bool arraySecondFromTop();
    void arrayIndexOffset(int);
    bool functionCallInProgress();

    //Respond to parser events
    void event_AccessRecordField(string);
    void event_Assignment();
    void event_Deref();
    int endArrayDimension(int, bool*);
    void event_FunctionCall();
    void endParameter(int);
    void event_MathOp(int);
    void event_RelationalOp();
    
    //Other
    void debugPrint(string = "");    
};

#endif
