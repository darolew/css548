// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This file contains the method definition of the Function type class.

#include <iostream>
#include <sstream>
#include <list>
#include "Function.h"
#include "SymTable.h"
#include "main.h"
using namespace std;

extern SymTable symTable;

//The constructor used for a function before its name, return type, or
//parameters are known. Functions objects are always created under those
//conditions, so this is the only constructor.
Function::Function() : Symbol("")
{
    //A NULL value indicates that there is no return type.
    returnType = NULL;
}

//TODO: Comment
void Function::generateDefinition(string ident)
{
    if(returnType)
        cout << returnType->cTypeName();
    else
        cout << "void";
        
    cout << " " << identifier << "(";
    
    //Parameters
    list<Parameter*>::iterator it = params.begin();
    for (; it != params.end(); it++) {
        if (it != params.begin())
            cout << ", ";
        Parameter *param = *it;
        param->generateDefinition("");
    }
 
    cout 
        //Close off function parameter list
        << ")" 
        //Go to the next line but do not indent, otherwise the curly 
        //brace will be indented as well.
        << endl 
        //Open the function body with a curly brace
        << "{" 
        //Add the new line and indentation
        << nlindent();
       
    //If this is a function (and not a procedure), create a variable to 
    //capture the return type.
    if (returnType) {
        cout
            << returnType->cTypeName() 
            << " " << identifier 
            << "_;" << nlindent();
    }
}

//Insert this function into the current scope, to allow for recursion; and
//then insert each of its formal paramaters into the current scope. The
//function is inserted into its parent's scope elsewhere.
bool Function::insertInto()
{
    //Insert the function into its own scope.
    Symbol::insertInto();
    return true;
}

//Add a formal parameter. A wrapper method to keep a private object
//encapsulated.
void Function::addParam(Parameter *param)
{
    params.push_back(param);
    symTable.insert(param);
}

//Set the return type. A wrapper method to keep private object encapsulated.
void Function::setReturnType(AbstractType *rt)
{
    returnType = rt;
}

//TODO: Change the name of isFunction to something more generic like 
//isSubRoutine
bool Function::isFunction()
{
    return true;
}

bool Function::isProcedure()
{
    return !returnType;
}

void Function::endFunction() 
{
    if (returnType)
        cout << "return " << identifier << "_;" << nlindent();

    cout << "\n}\n\n";
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
    list<Parameter*>::iterator it = params.begin();
    for (; it != params.end(); it++) {
        Parameter *param = *it;
        ss << "    " << param->toString();
    }
    ss << nlindent();
    return ss.str();
}
