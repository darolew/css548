// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This file contains definitions and funtion prototypes used in the semantic
// actions.

#ifndef ACTIONS_H
#define ACTIONS_H

#include <iostream>
#include <cstdio>
#include <list>

#include "SymTable.h"
#include "Const.h"
#include "PointerType.h"
#include "ArrayType.h"
#include "RecordType.h"
#include "Variable.h"
#include "Parameter.h"
#include "Function.h"
#include "IoFunction.h"
#include "Range.h"
#include "Tracker.h"

//Debug print macro
#define ERR(X)              \
    (cout << "\n***ERROR: " \
          << (X)            \
          << " | "          \
          << __FILE__       \
          << " line "       \
          << __LINE__       \
          << endl)

//The symbol table is a global object declared in main.cpp.
extern SymTable symTable;
extern Tracker tracker;

//***Variable declarations for temporary containers***
//These vars are used to collect objects when Yacc evaluates rules. For
//example, the idList collects identifiers {a, b, c, d} when parsing a
//declaration like: "var a,b,c,d : integer;"
extern list<string> idList;        // list of identifiers
extern list<Range> rangeList;      // list of ranges, like for an array
extern list<PointerType*> ptrList; // list of pointers that need types
extern list<Variable> fieldList;   // list of fields to add to a record
extern list<string*> dsgList;      // stack of designator strings
extern list<int> exprCount;        // stack of expression counts

extern ArrayType *currArray;       // current array type 
extern Function *currFunction;     // current function object
extern RecordType *currRecord;     // current record type
extern IoFunction *currIoFunc;     // current I/O function
extern AbstractType *currType;     // current type being constructed

//Type operations globals
extern int mathTable[64][64][64];
extern const int yTokOffset;

//
// ST-Related Actions
//
void assignTypesToPointers();
PointerType *addPointerToList(string, string);
void insertArrayType();
Terminal *newTerminal(string, int, char=NO_UNARY_OP);
Terminal initTerminal(string, int, char=NO_UNARY_OP);
void addRange(const Terminal *, const Terminal *);
void addField();
void addFormalParam(string, bool);
bool isDuplicateField(string);
void beginScope(const char *);
void declareVariable();

//
// Code Generation Actions
//
void initMathTable();
void procedureCallNoParam(string);
void procedureCallStart(string);
void procedureCallEnd();
void functionCallStart(string);
bool isStringType();
void generateNew(string);
void generateDelete(string);
void designatorBegin(string);
void expressionAction();
void printRelation(int);
void printAddOperator(int);
void printMultOperator(int);
void setLiteralAddValue(const Terminal*);
void setLiteralAddRange(const Terminal*, const Terminal*);
void endBlock();

#endif
