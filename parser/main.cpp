// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// TODO: Write description of this file.
#include <iostream>
#include <sstream>
#include "SymTable.h"
#include "main.h"

SymTable symTable;
unsigned indent;

int yyparse();

int main()
{
    return yyparse();
}

string indentation()
{
	stringstream ss (stringstream::in | stringstream::out);
	for (unsigned i = 0; i < indent; i++)
		ss << "    ";
	return ss.str();
}

string nlindent()
{
	return "\n" + indentation();
}

