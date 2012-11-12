// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// TODO: Write description of this file.
#ifndef ACTIONS_H
#define ACTIONS_H

#include <iostream>
#include <stdio.h>
#include <list>

#include "SymTable.h"
#include "PointerType.h"
#include "ArrayType.h"
#include "RecordType.h"
#include "Variable.h"
#include "Function.h"

/*
#include "Const.h"
#include "SetType.h"
*/

#define NO_UNARY_OP (0)

//The symbol table is a global object declared in main.cpp
//We know global objects are not a good idea, but it was expidient for 
//this project. We did parameterize methods to take the symbol table as an 
//input and did not access the global var directly. 
extern SymTable symTable;

//***Helper structures***
//Ptrinfo is a struct that wraps a pointer definition and the type that 
//it points to. 
typedef struct {
    PointerType *ptrType;
    string *pointee;
} Ptrinfo;

//***Variable declarations for temporary containers***
//These vars are used to collect objects when Yacc evaluates rules. For 
//example, the idList collects identifiers {a, b, c, d} when parsing a 
//declaration like: "var a,b,c,d : integer;"
extern list<string> idList;
extern list<Range> rangeList;
extern list<Ptrinfo> ptrList;
extern list<Variable> fieldList;
extern Function *currFunction;
extern AbstractType *currType;

void assignTypesToPointers(void);
void addPointerToList(string, string);
void insertCurrentVariableDecl(void);
void insertArrayType(void);
Terminal *newTerminal(string, int, char=NO_UNARY_OP);
Terminal initTerminal(string, int, char=NO_UNARY_OP);
void addRange(struct Terminal *, struct Terminal *);
void addField(void);
void addFormalParam(string);
bool isDuplicateField(string);
void beginScope(char *);

#endif
