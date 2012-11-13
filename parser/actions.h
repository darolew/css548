// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This file contains definitions and funtion prototypes used in the semantic
// actions.

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
#include "Range.h"

//The symbol table is a global object declared in main.cpp.
extern SymTable symTable;

//Ptrinfo is a struct that wraps a pointer definition and the type that
//it points to.
typedef struct Ptrinfo {
    PointerType *ptrType;
    string *pointee;
} Ptrinfo;

//***Variable declarations for temporary containers***
//These vars are used to collect objects when Yacc evaluates rules. For
//example, the idList collects identifiers {a, b, c, d} when parsing a
//declaration like: "var a,b,c,d : integer;"
extern list<string> idList;       // list of identifiers
extern list<Range> rangeList;     // list of ranges, like for an array
extern list<Ptrinfo> ptrList;     // list of pointers that need types
extern list<Variable> fieldList;  // list of fields to add to a record
extern Function *currFunction;    // current function object
extern AbstractType *currType;    // current type being constructed

//Method prototypes. For details about each method, see comments in actions.cpp
void assignTypesToPointers();
void addPointerToList(string, string);
void insertCurrentVariableDecl();
void insertArrayType();
Terminal *newTerminal(string, int, char=NO_UNARY_OP);
Terminal initTerminal(string, int, char=NO_UNARY_OP);
void addRange(const Terminal *, const Terminal *);
void addField();
void addFormalParam(string);
bool isDuplicateField(string);
void beginScope(const char *);

#endif
