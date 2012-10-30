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


bool SymTable::insert(Symbol *value) 
{
	return insert(value->identifier, value);
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
	
	//TODO: When generating code, "#include <stdbool.h>" so
	// C recognizes true/false
	insert(new BaseType("true", ytrue, "true"));
	insert(new BaseType("false", yfalse, "false"));
	insert(new BaseType("boolean", yunknown, "bool"));

	insert(new BaseType("integer", ynumber, "int"));
	insert(new BaseType("real", ynumber, "double"));
	insert(new BaseType("string", ystring, "char *"));

	//Silly tests to make sure constructors work.
	new Const("myConst", "5");
	string typeName = "integer";
	Symbol *sym = lookup(typeName);
	if (sym != NULL) {
		Variable *v = (Variable *) new Variable("x", (Type *) sym);
		cerr << "Created " << v->identifier << " of type " << v->type->identifier << endl;
	}
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
