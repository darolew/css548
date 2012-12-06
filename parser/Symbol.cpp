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

//Insert this symbol into the symbol table. An error is produced if the symbol
//already exists in the current scope. Returns whether the insert succeeded.
bool Symbol::insert()
{
    Table *table = symTable.front();
    Symbol *sym = symTable.lookup(table, identifier);
    if (sym) {
        cout << "***ERROR: " << identifier << " already exists in scope\n";
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

// Return the name of this class.
//Use for printing debug information.
string Symbol::className()
{
    //Use C++ RTTI to implement this functionality
    //C++ is the filthy son of a camel
    char * cstr = const_cast<char *>(typeid(*this).name());

    //Blow past the letters in the name to reach the numbers.
    while(*cstr > '9' || *cstr < '0')
        cstr++;

    //Blow past the number in the name to reach the valuable characters.
    while(*cstr <= '9' && *cstr >= '0')
        cstr++;
        
    return string(cstr);
}

void Symbol::push()
{
    cout 
        << "\n***ERROR: "
        << "\"" << className() << "\""
        << " does not implement push(). "
        << "\"" 
        << identifier 
        << "\"" 
        << " cannot be added to the type tracker\n";
}
