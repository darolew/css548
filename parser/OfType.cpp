#include "OfType.h"
#include "SymTable.h"
#include <stdio.h>

bool OfType::insertInto(SymTable &symTable)
{
     Symbol *sym = symTable.lookup(typeName);
         if(!sym || !sym->isType()) {
		     cerr << "\nerror: " << typeName << " is not a type\n";
             exit(1);
         }
		 
    type = (Type*)sym;

    return Symbol::insertInto(symTable);
}
