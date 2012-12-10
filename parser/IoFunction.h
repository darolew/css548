// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// Specialized class for IO functions (e.g., "read", "writeln").

#ifndef IOFUNCTION_H
#define IOFUNCTION_H

#include "Function.h"
using namespace std;

//
//TODO: Move the code into a *.cpp file.
//
class IoFunction : public Function {

public:
    IoFunction(string id) : Function(id)
    {
    }
    
    //This function is an I/O function.
    virtual bool isIoFunction()
    {
        return true;
    }
    
    //Generate the initial code portion for this I/O function.
    void generateInit()
    {
        if (isWrite())
            cout << "cout";
        else if (isRead())
            cout << "cin";
        else
            cout << "***ERROR: unsupported i/o function" << endl;
    }
    
    //Generate a separator to be used between parameters to this I/O
    //function.
    void generateSep()
    {
        if (isWrite())
            cout << " << ";
        else
            cout << " >> ";
    }
    
    //Generate the terminating code portion for this I/O function.
    void generateEnd()
    {
        if (identifier == "writeln") {
            generateSep();
            cout << "endl";
        }
    }
    
private:
    //Returns whether this I/O function is a write.
    bool isWrite()
    {
        return identifier == "write" || identifier == "writeln";
    }
    
    //Returns whether this I/O function is a read.
    bool isRead()
    {
        return identifier == "read" || identifier == "readln";
    }    
};

#endif
