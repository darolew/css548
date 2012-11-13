// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This file contains the method definitions of the Const class

#include "Const.h"
#include "main.h"
#include "Range.h"

//The name of the constant and the value of the constant are passed to the
//constructor.
Const::Const(string id, Terminal term) : Symbol(id)
{
    this->term = term;
}

//Print the const and its value. For example:
//
//    const a = -5;
//
//is represented as "a -5". Used for printST() and debugging.
string Const::toString()
{
    string s = identifier + " ";
    if (term.unaryOp != NO_UNARY_OP)
        s += term.unaryOp;
    s += term.str + nlindent();
    return s;
}
