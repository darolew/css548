#include <cstdlib>
#include "SymTable.h"
#include "BaseType.h"
#include "Variable.h"
#include "Const.h"
#include <iostream>


//Push a new scope onto the stack.
void SymTable::beginScope() 
{
	scopes.push_front(new Table());
}

void  SymTable::endScope() 
{
  assert_stack();
    
	Table *temp = scopes.front();
	scopes.pop_front();
	delete temp;
}

bool  SymTable::insert(string key, Symbol *value) 
{
  assert_stack();

	Table *current = scopes.front(); 

	bool alreadyExists = current->count(key) != 0;
	if (alreadyExists)
		return false;

	(*current)[key] = value;
	return true;
}


bool SymTable::insert(Symbol *value) 
{
	return insert(value->identifier, value);
}

//Drill down through the list of scopes, front to back, and look
//for the symbol. Return the first Symbol if it is found, otherwise
//return null.
Symbol *SymTable::lookup(string key) 
{

  assert_stack();

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
	
	//TODO: When generating code, "#include <stdbool.h>" so
	// C recognizes true/false
	insert(new BaseType("true", ytrue, "true"));
	insert(new BaseType("false", yfalse, "false"));
	insert(new BaseType("boolean", yunknown, "bool"));

	insert(new BaseType("integer", ynumber, "int"));
	insert(new BaseType("real", ynumber, "double"));
	
	//TODO: I assume char is a value type of a single char.
	//How do we represents string literals?
	insert(new BaseType("char", ystring, "char"));

/*
	insert("write", ywrite, "TODO:writefunction");
	insert("writeln", ywrite, "TODO:writefunction");
	insert("write", ywrite, "TODO:writefunction");
	insert("write", ywrite, "TODO:writefunction");
	insert("writeln", NULL);
	insert("read", NULL);
	insert("readln", NULL);
	insert("new", NULL);
	insert("dispose", NULL);
*/
}


bool SymTable::empty() 
{
  return scopes.empty();
}

//Prevent seg faults.
void SymTable::assert_stack() 
{
  if (empty()) {
    cerr << "\nFATAL ERROR SymTable::assert_stack\n\n";
    exit(EXIT_FAILURE);
  }
}


