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

//Resolve a type. Do nothing, since symbols have no type. Overridable.
//
//TODO: Why is this even here? Shouldn't the overridable method start in
//      AbstractType?
//
void Symbol::resolve()
{
}

//There are a number of overridable methods that query the attributes of a
//symbol. By default, a Symbol is none of these things.
bool Symbol::isType()       { return false; }
bool Symbol::isFunction()   { return false; }
bool Symbol::isIoFunction() { return false; }
bool Symbol::isPointer()    { return false; }
bool Symbol::isProcedure()  { return false; }
bool Symbol::isArray()      { return false; }
bool Symbol::isRecord()     { return false; }
bool Symbol::isSet()        { return false; }

//Return the name of this class. Used for printing debug information.
string Symbol::className()
{
    //Use C++ RTTI to implement this functionality
    char *cstr = const_cast<char *>(typeid(*this).name());

    //Blow past the letters in the name to reach the numbers.
    while (*cstr > '9' || *cstr < '0')
        cstr++;

    //Blow past the number in the name to reach the valuable characters.
    while (*cstr <= '9' && *cstr >= '0')
        cstr++;

    return string(cstr);
}

//Push a symbol onto the type stack. This prints an error, since a Symbol has
//no type, and push() should only be used on objects that override this method.
//
//TODO: Refactor to eliminate this. It is dumb for Symbol to define type stack
//      methods.
//
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

//Debugging information
string Symbol::dump() 
{
    return className() + ":" + (identifier.empty() ? "NO_NAME" : identifier);
}

//Only functions and procedures need to do something special when a designator
//is parsed.
void Symbol::event_Designator(string designator) 
{
    //Do nothing
}
