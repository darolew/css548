#include <stdio.h>

extern int yyparse();

int main() {
   int result = yyparse();
   return result;
}

