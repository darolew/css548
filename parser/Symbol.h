// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// Class to hold a symbol's attributes.

#ifndef SYMBOL_H
#define SYMBOL_H

#include <iostream>
#include <list>
#include <string>
using namespace std;

typedef enum {
    CT_NONE,
    CT_POINTER,
    CT_ARRAY,
    CT_FUNCTION,
    CT_PROCEDURE,
    CT_RECORD
} COMPLEXTYPE;

typedef enum {
    BT_NONE,
    BT_INTEGER,
    BT_REAL,
    BT_BOOLEAN,
    BT_CHARACTER
} BASETYPE;

class Symbol;
typedef list<Symbol*> Table;
class SymTable;

class Symbol {

public:
    string identifier; // the identifier associated with this symbol

    Symbol(string);
    virtual void push();
    virtual ~Symbol();
    virtual bool insert();
    virtual void resolve();
    virtual COMPLEXTYPE complexType();
    virtual BASETYPE baseType();
    virtual bool isType();
    virtual bool isFunction();
    virtual bool isIoFunction();
    virtual bool isMemFunction();
    virtual bool isPointer();
    virtual bool isProcedure();
    virtual bool isArray();
    virtual bool isRecord();
    virtual string className();
};

#endif
