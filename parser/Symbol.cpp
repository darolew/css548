// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This file contains the method definitions of the Symbol class.

#include <iostream>
#include "Symbol.h"
#include "SymTable.h"
#include "main.h"

extern SymTable symTable;

//The constructor for a Symbol. Simply saves the identifier.
Symbol::Symbol(string id)
{
    this->identifier = id;
}

//For some resons, just defining an empty destructor eliminates many memory
//leaks.
Symbol::~Symbol()
{
}

string Symbol::className() 
{
    return "Symbol";
}

//Insert this symbol into the synbol table. An error is produced if the symbol
//already exists in the current scope. Returns whether the insert succeeded.
bool Symbol::insertInto()
{
    //cout << "inserting " << toString();
    Table *table = symTable.front();
    Symbol *sym = symTable.lookup(table, identifier);
    if (sym) {
        cerr << "error: " << identifier << " already exists in scope\n";
        return false;
    }

    //cout << "\tInsert succeeded.\n";
    table->push_front(this);
    return true;
}

void Symbol::resolve()
{
}

//By default, a symbol is not a type. Overridable.
bool Symbol::isType()
{
    return false;
}

COMPLEXTYPE Symbol::complexType()
{
	return CT_NONE;
}

BASETYPE Symbol::baseType()
{
	return BT_NONE;
}

bool Symbol::isFunction()
{
    return false;
}

bool Symbol::isIoFunction()
{
    return false;
}

bool Symbol::isMemFunction()
{
    return false;
}

bool Symbol::isPointer()
{
    return false;
}

bool Symbol::isProcedure()
{
    return false;
}

bool Symbol::isArray()
{
    return false;
}

bool Symbol::isRecord()
{
    return false;
}

//Returns a string representation of the symbol.
string Symbol::toString()
{
    return identifier + nlindent();
}
