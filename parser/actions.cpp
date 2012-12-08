// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This file contains defintion of helper methods used in the semantic actions.

#include <strings.h>
#include "actions.h"
#include "y.tab.h"

//The type tracker is a global object for easy access
Tracker tracker;

//Create variables used by the semantic actions to collect objects.
list<string> idList;        // list of identifiers
list<Range> rangeList;      // list of ranges, like for an array
list<PointerType*> ptrList; // list of pointers that need types
list<Variable> fieldList;   // list of fields to add to a record
list<string*> dsgList;      // stack of designator strings

ArrayType* currArray;       // current array
Function *currFunction;     // current function object
RecordType* currRecord;     // current record type
IoFunction *currIoFunc;     //
AbstractType *currType;     // current type being constructed

//Type operations
int mathTable[64][64][64];
const int offset = 258;

//This method iterates through the list of pointers declared in a just-parsed
//typedef block; it is invoked when exiting the typedef block, and it assigns
//a points-to-type to each pointer. This post facto way of adding the type
//allows pointers to point to types that have not yet been defined.
void assignTypesToPointers()
{
    while (!ptrList.empty()) {
        PointerType *ptrType = ptrList.front();
        ptrType->resolve();
        ptrList.pop_front();
    }
}

//When a pointer type is defined, a new object for it is created and inserted
//into the symbol table. The type of the pointer is saved locally as a string,
//along with its pointer object. That string will be used as a key to lookup
//the type object at the end of the typedef block.
PointerType *addPointerToList(string nameOfPointer, string nameOfPointee)
{
    PointerType *ptrType = new PointerType(nameOfPointer, nameOfPointee);
    ptrList.push_front(ptrType);
    ptrType->insert();
    return ptrType;
}

//Convience method for creating Terminal struct.
Terminal *newTerminal(string lexeme, int token, char unaryOperatorChar)
{
    Terminal *terminal = new Terminal;
    *terminal = initTerminal(lexeme, token, unaryOperatorChar);
    return terminal;
}

//Convience method for initializing Terminal struct.
Terminal initTerminal(string lexeme, int token, char unaryOperatorChar)
{
    Terminal terminal;
    terminal.str = lexeme;
    terminal.token = token;
    terminal.unaryOp = unaryOperatorChar;
    return terminal;
}

//Create a new array type and add it to the symbol table. Uses the ranges built
//up in the rangeList. Empty the range list when the insert is complete.
void insertArrayType()
{
    AbstractType *elementType = currType;
    currType = new ArrayType(elementType, rangeList);
    rangeList.erase(rangeList.begin(), rangeList.end());
}

//Create a range object, set its members, and push it on a list.
void addRange(const Terminal *low, const Terminal *high)
{
    Range range;
    range.low = *low;
    range.high = *high;
    rangeList.push_back(range);
}

//Remove an identifer and turn it into a variable as part of a record's fields.
void addField()
{   
    while (!idList.empty()) {
        string id = idList.front();
        if (isDuplicateField(id)) {
            cout << "***ERROR: " << id << " already exists in record\n";
        } else {
            Variable field(id, currType);
            fieldList.push_front(field);
        }
        idList.pop_front();
    }
}

//Check for duplicated field names in a record.
bool isDuplicateField(string id)
{
    list<Variable>::iterator it = fieldList.begin();
    for (; it != fieldList.end(); it++) {
        Variable &prev = *it;
        if (id == prev.identifier)
            return true;
    }
    return false;
}

//Create a formal method parameter from a list of identifiers. Add the
//parameter to the object for the current function.
void addFormalParam(string typeName, bool varflag)
{
    while (!idList.empty()) {
        string name = idList.front();
        AbstractType *formalType = symTable.lookupType(typeName);
        Parameter *formalParam = new Parameter(name, formalType, varflag);
        currFunction->addParam(formalParam);
        idList.pop_front();
    }
}

//Begin a scope for a new function.
void beginScope(const char *name)
{
    currFunction->identifier = name;
    currFunction->insert();
    symTable.beginScope(name);
}

void initMathTable() 
{
    //Zero out the table
    bzero(mathTable, 64 * 64 * 64 * sizeof(int));

    // Forward slash (/) is ydivide
    int yreal_ = yreal-offset;
    int yinteger_ = yinteger-offset;
    int yplus_ = yplus-offset;
    int yminus_ = yminus-offset;
    int ymultiply_ = ymultiply-offset;
    int ydivide_ = ydivide-offset;
    int ydiv_ = ydiv-offset;
    int ymod_ = ymod-offset;
    
    mathTable[yreal_][yreal_][yplus_] = yreal;
    mathTable[yreal_][yinteger_][yplus_] = yreal;
    mathTable[yinteger_][yreal_][yplus_] = yreal;
    mathTable[yinteger_][yinteger_][yplus_] = yinteger;

    mathTable[yreal_][yreal_][yminus_] = yreal; 
    mathTable[yreal_][yinteger_][yminus_] = yreal;
    mathTable[yinteger_][yreal_][yminus_] = yreal;
    mathTable[yinteger_][yinteger_][yminus_];

    mathTable[yreal_][yreal_][ymultiply_] = yreal;
    mathTable[yreal_][yinteger_][ymultiply_] = yreal;
    mathTable[yinteger_][yreal_][ymultiply_] = yreal;
    mathTable[yinteger_][yinteger_][ymultiply_] = yinteger;

    mathTable[yreal_][yreal_][ydivide_] = yreal;
    mathTable[yreal_][yinteger_][ydivide_] = yreal;
    mathTable[yinteger_][yreal_][ydivide_] = yreal;
    mathTable[yinteger_][yinteger_][ydivide_] = yreal;

    mathTable[yinteger_][yinteger_][ydiv_] = yinteger;

    mathTable[yinteger_][yinteger_][ymod_] = yinteger;
}
