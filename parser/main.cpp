#include <stdio.h>
#include "SymTable.h"

extern int yyparse();

int main() {
   int result = yyparse();
   return result;
}

