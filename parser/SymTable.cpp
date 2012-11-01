#include <cstdlib>
#include "SymTable.h"
#include "BaseType.h"
#include <iostream>


//Push a new scope onto the stack.
void SymTable::beginScope(string name) 
{
  scopeNames.push_front(name);
  scopes.push_front(new Table());
  cerr << "\nENTER " << name << endl;
  printLine("-");

}

void  SymTable::endScope() 
{
  assert_stack();
    
  Table *temp = scopes.front();
  scopes.pop_front();
  delete temp;
  cerr << "\nEXIT " << scopeNames.front() << endl;
  printLine("=");
  scopeNames.pop_front();
}

bool  SymTable::insert(Symbol *symbol) 
{
  assert_stack();
	cerr << "inserting " << symbol->toString();
	string key = symbol->identifier;
	Table *current = scopes.front(); 

	bool alreadyExists = current->count(key) != 0;
	if (alreadyExists) {
		cerr << "\n\tInsert failed. Symbol name already exists in current scope.\n";
		return false;
	}

	(*current)[key] = symbol;
	cerr << "\t Insert succeeded.\n"; 
	return true;
}

//Drill down through the list of scopes, front to back, and look
//for the symbol. Return the first Symbol if it is found, otherwise
//return null.
Symbol *SymTable::lookup(string key) 
{
  for (list<Table*>::iterator it = scopes.begin();
    it != scopes.end(); it++) 
  {
    Table *next = *it;
    if (next->count(key)) 
      return (*next)[key];
  }
  return NULL;
}

SymTable::SymTable() 
{
	//Start standard identifier.
	beginScope("Standard Identifier Table");
	
	//TODO: When generating code, "#include <stdbool.h>" so
	// C recognizes true/false
	insert(new BaseType("true", 0, "true"));
	insert(new BaseType("false", 0, "false"));
	insert(new BaseType("boolean", 0, "bool"));

	insert(new BaseType("integer", 0, "int"));
	insert(new BaseType("real", 0, "double"));
	
	//TODO: I assume char is a value type of a single char.
	//How do we represents string literals?
	insert(new BaseType("char", 0, "char"));

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

//When the SymTable class goes out of scope (if it was on the stack)
//or deleted (if it was on the heap), the list objects will be deleted
//because they were created on the stack when the SymTable constructor
//was called. 
//When an STL class like list is deleted, it will call the desctructor
//on every object still in the list. No explicit cleanup is needed.
//At least, that is what the C++ spec says. :-)
SymTable::~SymTable() 
{
  //Get rid of the SIT.
  assert_stack();
  endScope();
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

void SymTable::printST() 
{

}

void SymTable::printLine(string divider)  
{
  for(int i=0; i<75; ++i)
    cerr << divider;
  cerr << endl;
}
