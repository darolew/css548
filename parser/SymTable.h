#ifndef SYMTABLE
#define SYMTABLE

/* 
 * Data structures for managing symbols and scope 
 * Aaron Hoffer and Daniel Lewis
 * CSS 548, Autumn 2012
 */

#include <map>
#include <string> 
#include <list>
#include "Symbol.h"

using namespace std;

class SymTable {

typedef map<string, Symbol*> Table;

private:
list<Table*> scopes;

public:
//Constructor
SymTable();

//Create a new table and push it onto the back of the list.
void beginScope();

//Pop (and discard) the table on the back of the list.
void endScope();

//Insert a pair into the table that is on the back of the list.
//Return true if insert suceeded. Return false if insert failed.
//Insert will fail if symbol is already defined in the current scope.
bool insert(string key, Symbol *value);

//Convenience method. Uses the symbols identifier as the key.
bool insert(Symbol *value);

//Search through all the tables in the list, starting with the last one, 
//and look for the key. 
Symbol *lookup(string key);

};

#endif

