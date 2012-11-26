// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This file contains the main method. The main method is invoked when the
// program is executed. This program is integrated with a Flex scanner and
// Yacc parser. The main method kicks off the parser (which in turn invokes
// the scanner). This file also contains some global methods for handling
// indentation levels used by the toString() methods, toIdentTypeString()
// methods and the SymTable::printST() method.

#include <iostream>
#include <sstream>
#include "SymTable.h"
#include "main.h"

//Create the stack-of-symbol-tables object. This is a globally accessible
//object because it is accessed almost everywhere.
SymTable symTable;

//Create the global indentation level. This value is used to control the
//indentation in the printST(), toString(), toIdentTypeString() methods,
//and is modified when the program and functions come into scope and
//leave scope.
unsigned indent;

//Forward declaration of the yyparse() method.
int yyparse();

int main()
{
    return yyparse();
}

//Return a string of spaces. The number of spaces correponds to the current
//indentation level.
string indentation()
{
    stringstream ss (stringstream::in | stringstream::out);
    for (unsigned i = 0; i < indent; i++)
        ss << "    ";
    return ss.str();
}

//Return a newline followed by a string of spaces for the current indentation
//level. This is used when printing newlines during printST(), so that the next
//object is setup to be indented correctly. Doing it this way is a bit strange,
//but the alternative -- having each object generate all of its own
//indentation -- is not viable, since when the object is printed it does not
//always know whether the line is empty at that point. (Sometimes two objects
//are printed on the same line.) However, we *do* always know when newlines
//are actually generated, so the indentation must be done at that time.
string nlindent()
{
    return "\n" + indentation();
}

//Formatting hack...
string unindent()
{
    //The "\b" is backspace and we use four spaces for indentation.
    //return "\b\b\b\b";
    return "";
}

