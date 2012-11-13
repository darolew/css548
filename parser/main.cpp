// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This file contains the main method. The main method is invoked when the
// program is executed. This program is integrated witha Flex scanner and
// Bison parser. The main method kicks off the parser (which in turn invokes
// the scaneer). This file also contains some global methods for handling
// indentation levels used by the toString() methods, toIdentTypeString()
// methods and the SymTable::printST method.

#include <iostream>
#include <sstream>
#include "SymTable.h"
#include "main.h"

//Create the stack-of-symbol-tables object
//This is a globablly accessible object because it is accessed almost
//everywhere.
SymTable symTable;

//Create the global indentation level. This value is can be set in
//printST, toString(), toIdentTypeString() methods.
unsigned indent;

//Forward declaration of the yyparse method.
int yyparse();

int main()
{
    return yyparse();
}

//Return a string of spaces. The number of spaces correpond to the
//current indentation level.
string indentation()
{
    stringstream ss (stringstream::in | stringstream::out);
    for (unsigned i = 0; i < indent; i++)
        ss << "    ";
    return ss.str();
}

//Return a new line followed by a string of spaces
//(the current indentation level). This string is printed after
//object is printed on the std out stream. It sets up the next
//object to printed at the correct level of indentation.
string nlindent()
{
    return "\n" + indentation();
}
