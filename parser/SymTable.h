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

typedef map<string, Symbol> table;

private:
list<table> scopes;

public:
//Create a new table and push it onto the back of the list.
void beginScope();

//Pop (and discard) the table on the back of the list.
void endScope();

//Insert a pair into the table that is on the back of the list.
void insert(string key, Symbol value);

//Search through all the tables in the list, starting with the last one, 
//and look for the key. 
string lookup(string key);

};

#endif

