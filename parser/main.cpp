#include "SymTable.h"
#include <stdio.h>
#include <iostream>

SymTable symTable;

extern int yyparse();

int main()
{
    return yyparse();
}
