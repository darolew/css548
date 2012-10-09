#include <stdio.h>

extern int yyparse();

void yyerror(char *s) {
   fprintf(stderr, "%s\n", s);
}


int main() {
   int result = yyparse();
   return result;
}

