#ifndef SYMBOL_
#define SYMBOL_

#include <map>
#include <string> 
#include <iostream>

using namespace std;
class Symbol;
typedef map<string, Symbol*> Table;
class SymTable;

/* Class to hold a symbol's attributes
 *
 */

 
class Symbol {
public:
	Symbol(string id) : identifier(id) {}
	
	virtual string toString()
	{
		return identifier;
	}
	
	virtual bool isType() 
	{
	    return false;
	}
	
	virtual bool insertInto(SymTable &);
	
	string identifier;
};

#endif
