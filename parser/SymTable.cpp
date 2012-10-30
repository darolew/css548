#include "SymTable.h"

//Push a new scope onto the stack.
void SymTable::beginScope() 
{
	scopes.push_front(new Table());
}

void  SymTable::endScope() 
{
	Table *temp = scopes.front();
	scopes.pop_front();
	delete temp;
}

bool  SymTable::insert(string key, Symbol *value) 
{
	Table *current = scopes.front(); 

	bool alreadyExists = current->count(key) != 0;
	if (alreadyExists)
		return false;

	(*current)[key] = value;
	return true;
}

//Drill down through the list of scopes, front to back, and look
//for the symbol. Return the first Symbol if it is found, otherwise
//return null.
Symbol *SymTable::lookup(string key) 
{
	for (list<Table*>::iterator it = scopes.begin(); it != scopes.end(); it++) {
		Table *next = *it;
		if (next->count(key)) 
			return (*next)[key];
	}
	return NULL;
}

SymTable::SymTable() 
{
	//Start standard identifier.
	beginScope();
	
	//Standard identifiers.
	insert("false", new Symbol("false", new Type(...)));
	insert("true", NULL);
	insert("integer", NULL);
	insert("boolean", NULL);
	insert("real", NULL);
	insert("char", NULL);
	insert("write", NULL);
	insert("writeln", NULL);
	insert("read", NULL);
	insert("readln", NULL);
	insert("new", NULL);
	insert("dispose", NULL);
}
