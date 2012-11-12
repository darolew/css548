// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// TODO: Write description of this file.
#include <iostream>
#include <sstream>
#include <list>
#include "Function.h"
#include "main.h"
using namespace std;

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
   
bool Function::insertInto() 
{
	Symbol::insertInto();
    list<Variable>::iterator it = params.begin();
    for (; it!=params.end(); ++it)
        it->insertInto();
    return true;
}
    
void Function::addParam(Variable *param)
{
	params.push_back(*param);
}
    
void Function::setReturnType(AbstractType *rt)
{
	returnType = rt;
}
