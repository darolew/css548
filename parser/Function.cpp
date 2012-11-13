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

//The constructor used for a function before its name, return type, or
//parameters are known. Functions objects are always created under those
//conditions, so this is the only constructor.
Function::Function() : Symbol("")
{
    //A NULL value indicates that there is no return type.
    returnType = NULL;
}

//Returns a description of the function in the format:
//
//    functionName returnType
//
//Used only for printST() and debugging.
string Function::toString()
{
    stringstream ss (stringstream::in | stringstream::out);
    ss << identifier;
    if (returnType)
        ss << " " << returnType->toIdentTypeString();
    else
        ss << nlindent();
    return ss.str();
}

//Returns a description of the function in the format:
//
//    functionName returnType
//        formalParam1
//        formalParam2
//        ...
//        formalParamN
//
//Similar to toString(), but also prints the parameters. Used only for
//printST() and debugging.
string Function::toStringLong()
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

//Insert this function into the current scope, to allow for recursion; and
//then insert each of its formal paramaters into the current scope. The
//function is inserted into its parent's scope elsewhere.
bool Function::insertInto()
{
    //Insert the function into its own scope.
    Symbol::insertInto();

    //Insert the formal parameters into the current scope.
    list<Variable>::iterator it = params.begin();
    for (; it!=params.end(); ++it)
        it->insertInto();

    return true;
}

//Add a formal parameter. A wrapper method to keep a private object
//encapsulated.
void Function::addParam(Variable *param)
{
    params.push_back(*param);
}

//Set the return type. A wrapper method to keep private object encapsulated.
void Function::setReturnType(AbstractType *rt)
{
    returnType = rt;
}
