#include "SymTable.h"
#include <stdio.h>
#include <iostream>

SymTable symTable;

extern int yyparse();

int main() {
/*
  Symbol *p;
  SymTable s;
  p = s.lookup("integer");
  cerr << endl << p->identifier << endl;
  p = s.lookup("GARBAGE");
  s.beginScope("blah");
  p = s.lookup("true");
  cerr << endl << p->identifier << endl;
  s.endScope();
  p = s.lookup("char");
  cerr << endl << p->identifier << endl;
  
  return 0;
*/
//THE REAL MAIN METHOD

  int result = yyparse();
  return result;


}

