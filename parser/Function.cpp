// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This file contains the method definition of the Function type class.
#include <iostream>
#include <sstream>
#include <list>
#include "Function.h"
#include "main.h" 
using namespace std;

//Print a description of the function on std out in the format:
//
//    functionName returnType
// 
string Function::toString(void)
{
    stringstream ss (stringstream::in | stringstream::out);
    ss << identifier;
    if (returnType)
        ss << " " << returnType->toIdentTypeString();
        else
        ss << nlindent();
        return ss.str();
}

//Print a description of the function on std out in the format:
//
//    functionName returnType
//      formalParam1
//      formalParam2...
//
string Function::toStringLong(void)
{
    stringstream ss (stringstream::in | stringstream::out);
    ss << toString();
    list<Variable>::iterator it = params.begin();
    for (; it != params.end(); it++) {
        ss << "    " << it->toString();
    }
    ss << nlindent();
    return ss.str();
}

//Insert this function into the current scope.
//Then insert each of its formal paramaters into the current scope.   
bool Function::insertInto() 
{
    //TODO: Should the function itself be inserted into it's parent's scope
    //It seems like we are already inserting it into its parent's scope somewhere.
    Symbol::insertInto();
    list<Variable>::iterator it = params.begin();
    for (; it!=params.end(); ++it)
        it->insertInto();
    return true;
}

//Wrapper method to keep private object encapsulated.    
void Function::addParam(Variable *param)
{
    params.push_back(*param);
}

//Wrapper method to keep private object encapsulated.    
void Function::setReturnType(AbstractType *rt)
{
    returnType = rt;
}
