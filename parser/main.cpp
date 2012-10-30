#include <stdio.h>
#include "SymTable.h"

#include <iostream>


extern int yyparse();

int main() {

  Symbol *p;
  SymTable s;
  p = s.lookup("integer");
  cerr << endl << p->identifier << endl;
  s.beginScope();
  p = s.lookup("true");
  cerr << endl << p->identifier << endl;
  s.endScope();
  p = s.lookup("char");
  cerr << endl << p->identifier << endl;
  
  return 0;

/*
  int result = yyparse();
  return result;
*/

}

