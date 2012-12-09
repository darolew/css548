// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// Specialized class for IO functions (e.g., "read", "writeln").
// Just a placeholder class for phase 3.

#ifndef IOFUNCTION_H
#define IOFUNCTION_H

#include "Function.h"
using namespace std;

//TODO: Add methods for code generation
class IoFunction : public Function {

public:
    IoFunction(string id) : Function(id)
    {
    }
       
    virtual bool isIoFunction()
    {
        return true;
    }
    
    void generateInit()
    {
        if (isWrite())
            cout << "cout";
        else if (isRead())
            cout << "cin";
        else
            cout << "***ERROR: unsupported i/o function" << endl;
    }
    
    void generateSep()
    {
        if (isWrite())
            cout << " << ";
        else
            cout << " >> ";
    }
    
    void generateEnd()
    {
        if (identifier == "writeln") {
            generateSep();
            cout << "endl";
        }
    }
    
private:
    bool isWrite()
    {
        return identifier == "write" || identifier == "writeln";
    }
    
    bool isRead()
    {
        return identifier == "read" || identifier == "readln";
    }    
};

#endif
