// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This file contains defintion of helper methods used in the semantic actions.

#include <strings.h>
#include "actions.h"
#include "y.tab.h"


//Local function declarations
static int terminalToInteger(const Terminal*);
static void validateTerminal(const Terminal*);
static void validateRange(Range);


//The type tracker is a global object for easy access
Tracker tracker;

//Create variables used by the semantic actions to collect objects.
list<string> idList;        // list of identifiers
list<Range> rangeList;      // list of ranges, like for an array
list<PointerType*> ptrList; // list of pointers that need types
list<Variable> fieldList;   // list of fields to add to a record
list<string*> dsgList;      // stack of designator strings
list<int> exprCount;        // stack of expression counts

ArrayType *currArray;       // current array
Function *currFunction;     // current function object
RecordType *currRecord;     // current record type
IoFunction *currIoFunc;     // current I/O function
AbstractType *currType;     // current type being constructed

//Type operation globals
int mathTable[64][64][64];
const int yTokOffset = 258;


//
// ST-Related Actions
//

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
    validateTerminal(low);
    validateTerminal(high);
    Range range;
    range.low = terminalToInteger(low);
    range.high = terminalToInteger(high); 
    validateRange(range);
    rangeList.push_back(range);
}

//Convert a terminal to an integer. Returns -1 on error.
static int terminalToInteger(const Terminal *t)
{
    if (t->token == yinteger)
        return atoi(t->str.c_str());
    
    if (t->token == ystring) {
        if (t->str.size() > 1)
            return -1;
        
        return t->str[0];
    }
    
    if (t->token == yident) {
        Symbol *s = symTable.lookup(t->str);
        Const *c = dynamic_cast<Const*>(s);
        if (!c)
            return -1;

        return terminalToInteger(&c->term);
    }
    
    return -1;
}

//Prints an error if a character terminal is more than one character long,
//rendering it illegal for ranges, sets, and array dimensions.
static void validateTerminal(const Terminal *term)
{
    //Array bounds must be single characters
    if (term->token == ystring && term->str.size() > 1) {
        cout << "\n***ERROR invalid character array bounds '" 
            << term->str << "'" << endl;
    }
}    

//Prints an error if a range does not encompass at least one value.
static void validateRange(Range r) 
{
    if(r.low >= r.high)
        cout << "\n***ERROR: array bounds must be ascending" << endl;
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
        Variable *var = new Variable(name, formalType);
        currFunction->addParam(formalParam);
        var->insert();
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

void declareVariable()
{
    //Variables must point to a known type. They can be resolved immediately.
    //If a variable is declared with an unknown identifier, as in sterror.p:                                                                                       
    //    aaa: undefinedType;                                                                                                       
    //currType will be NULL and cannot be resolved                                                                                  
    if (currType)                                                                                                                   
        currType->resolve();    

    //Walk the list of variable names being declared. For example, the
    //declaration "a,b,c : interger;" includes a list of variables {a, b, c}
    //and their type, integer. For each one, a new variable object is created,
    //assigned a type, and entered into the symbol table. The list is emptied
    //as the variables are inserted into the symbol table.
    while (!idList.empty()) {
        string name = idList.front();
        Variable *var = new Variable(name, currType);
        var->generateDefinition(name);
        var->insert();
        idList.pop_front();
        cout << ";" << nlindent();
    }
}

//
// Code Generation Actions
//

//Initializes a table that indicates the results of math operations. Given
//the type of the operands and the operation, the mathTable returns the
//resulting type.
void initMathTable() 
{
    //Zero out the table
    bzero(mathTable, sizeof mathTable);

    // Forward slash (/) is ydivide
    int yreal_      = yreal - yTokOffset;
    int yinteger_   = yinteger - yTokOffset;
    int yplus_      = yplus - yTokOffset;
    int yminus_     = yminus - yTokOffset;
    int ymultiply_  = ymultiply - yTokOffset;
    int ydivide_    = ydivide - yTokOffset;
    int ydiv_       = ydiv - yTokOffset;
    int ymod_       = ymod - yTokOffset;
    
    mathTable[yreal_][yreal_][yplus_] = yreal;
    mathTable[yreal_][yinteger_][yplus_] = yreal;
    mathTable[yinteger_][yreal_][yplus_] = yreal;
    mathTable[yinteger_][yinteger_][yplus_] = yinteger;

    mathTable[yreal_][yreal_][yminus_] = yreal; 
    mathTable[yreal_][yinteger_][yminus_] = yreal;
    mathTable[yinteger_][yreal_][yminus_] = yreal;
    mathTable[yinteger_][yinteger_][yminus_] = yinteger;

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

//Generate code and perform error checking for a procedure call that takes no
//parameters. Note this includes writeln(), read(), etc.
void procedureCallNoParam(string ident)
{
    Symbol *sym = symTable.lookup(ident);
    if (!sym || !sym->isFunction())
        cout << "***ERROR: " << ident << " is not a procedure" << endl;
    else if (sym->isIoFunction()) {
        //Generate the I/O statement. Probably "cout << endl".
        IoFunction *iofunc = (IoFunction*)sym;
        iofunc->generateInit();
        iofunc->generateEnd();                                                        
    } else {
        //Append parentheses to the name of the procedure.
        cout << ident << "()";
        currIoFunc = NULL;
    }
}

//Generate code and perform error checking for the start of a procedure call.
//Note this includes writeln(), read(), etc.
void procedureCallStart(string ident)
{
    //Add the procedure to the type tracker
    tracker.push(ident);

    //TODO: These checks are partially redundant.
    Symbol *sym = symTable.lookup(ident);
    if (!sym || !sym->isFunction())
        cout << "***ERROR: " << ident << " is not a procedure" << endl;
    else if (sym->isIoFunction()) {
        currIoFunc = (IoFunction*)sym;
        currIoFunc->generateInit();
        currIoFunc->generateSep();
    } else
        cout << ident;
    
    //Reset the expression count because it is used to 
    //determine which parameter is being parsed.
    exprCount.push_front(0);
}

//Generate code and perform error checking for the end of a procedure call.
//Note this includes writeln(), read(), etc.
void procedureCallEnd()
{
    if (currIoFunc) {
        currIoFunc->generateEnd();
        currIoFunc = NULL;
        
        //Since I/O functions have an indefinite number of parameters, the
        //function must be manually removed from the type stack.
        tracker.pop();
    }
}

//Generate code and perform error checking for the start of a function call.
void functionCallStart(string ident)
{
    //Add the function to the type tracker
    tracker.push(ident);
    
    //Better be a function...
    AbstractType *type = tracker.peekType();
    if (!type->isFunction())
        cout << "***ERROR: expected " << ident << " to be function\n";

    cout << ident << flush;
    
    //Reset the expression count because it is used to determine which
    //parameter is being parsed.
    exprCount.push_front(0);
}

//Returns whether the type at the top of the type stack is a string (char)
//type.
bool isStringType()
{
    AbstractType *type = tracker.peekType();
    BaseType *bt = dynamic_cast<BaseType*>(type);
    return bt && bt->isStringType();
}

//Generate a new statement to dynamically allocate memory.
void generateNew(string ident)
{
    Symbol *sym = symTable.lookup(ident);
    Variable *var = dynamic_cast<Variable*>(sym);
    if (!sym)
        cout << "***ERROR: undefined identifier " << ident << endl;
    else if (!var)
        cout << "***ERROR: " << ident << " is not a variable" << endl;
    else {
        //generateNewStatement() checks whether the variable's type is a
        //pointer.
        var->generateNewStatement();
    }
}

//Generate a delete statement to dynamically free memory.
void generateDelete(string ident)
{
    Symbol *sym = symTable.lookup(ident);
    Variable *var = dynamic_cast<Variable*>(sym);
    if (!sym)
        cout << "***ERROR: undefined identifier " << ident << endl;
    else if (!var)
        cout << "***ERROR: " << ident << " is not a variable" << endl;
    else if (!var->getType()->isPointer())
        cout << "***ERROR: " << ident << " is not a pointer" << endl;
    
    cout << "delete " << ident;
}

//Generate code for the start of a designator.
void designatorBegin(string ident)
{
    cout << ident;

    //Update the type stack
    tracker.push(ident);

    //If this identifier is being used as an array index and it is a string,
    //access the first character.
    if (tracker.arraySecondFromTop() && isStringType())
        cout << "[0]";

    //Notify the object that is was just used as a designator.
    Symbol *sym = symTable.lookup(ident);
    if (sym)
        sym->event_Designator(ident);
}

//Generate code for the end of an expression.
void expressionAction()
{
    //Non-terminal ExpAction is trigged when an expression is parsed. It
    //triggers semantic actions and elminates duplicate code that would
    //otherwise be in both ExpList productions
    
    //If the expression is being used to index into an array...
    bool indexingArray = tracker.arraySecondFromTop();
    if (indexingArray) {
        //Print code to translate the index into a zero-based array.
        int dimension = exprCount.front();
        tracker.arrayIndexOffset(dimension);
        
        //Update the expression count, i.e., the dimension index of the
        //current array. This may be incremented, or it may be reset if
        //the next dimension is another array.
        bool last;
        exprCount.front() += tracker.endArrayDimension(dimension, &last);
        
        //Close array access
        cout << "]";

        //If this array index was the last dimension for the array, pop the
        //expression count for the array dimensions off the stack.
        if (last)
            exprCount.pop_front();
    }

    //If we were not indexing an array, or if a non-array type is on the top
    //of the type stack...
    if (!indexingArray || !tracker.arrayOnTopOfStack()) {
        //If a function call is in progress...
        if (tracker.functionCallInProgress()) {
            //Inform the tracker that a parameter expression has been parsed.
            tracker.endParameter(exprCount.front());

            //Increment the expression count. Do not do this for I/O functions,
            //whose parameters are special.
            if (!currIoFunc)
                exprCount.front() += 1;
        }
    }
}

//Print the C++ equivalent of the given Pascal relation operator.
void printRelation(int opToken)
{
    switch (opToken) {
        case yequal:
            cout << " == ";
            break;
        case ynotequal:
            cout << " != ";
            break;
        case yless:
            cout << " < ";
            break;
        case ygreater:
            cout << " > ";
            break;
        case ylessequal:
            cout << " <= ";
            break;
        case ygreaterequal:
            cout << " >= ";
            break;
        case yin:
            cout << " % ";  // Overloaded for sets
            break;
        default:
            cout << "***ERROR: unhandled opToken=" << opToken << " (internal)";
            cout << endl;
            break;
    }
}

//Print the C++ equivalent of the given Pascal "add" operator.
void printAddOperator(int opToken)
{
    switch (opToken) {
        case yplus:
            cout << " + ";
            break;
        case yminus:
            cout << " - ";
            break;                            
        case yor:
            cout << " || ";
            break;                        
        default:
            cout << "***ERROR: unhandled opToken=" << opToken << " (internal)";
            cout << endl;
            break;
    }
}

//Print the C++ equivalent of the given Pascal "mult" operator.
void printMultOperator(int opToken)
{
    switch (opToken) {
        case yand:
            cout << " && ";
            break;
        case ymultiply:
            cout << " * ";
            break;                            
        case ydivide:
        case ydiv:
            cout << " / ";
            break;
        case ymod:
            cout << " % ";
            break;                        
        default:
            cout << "***ERROR: unhandled opToken=" << opToken << " (internal)";
            cout << endl;
            break;
    }
}

//Add an integer value to the current set literal.
void setLiteralAddValue(const Terminal *term)
{
    //Literal integers are the only supported set value.
    if (term->token != yinteger)
        cout << "***ERROR: Set values must be integers" << endl;

    //Make sure this value is in-range for this set.
    const int val = atoi(term->str.c_str());
    SetType *set = (SetType*)tracker.peekType();
    if (!set->legalValue(val))
        cout << "***ERROR: Out-of-bounds set value" << endl;
    
    //Pass this value into IntSet::makeLiteral().
    cout << val;
}

//Add a range of integer values to the current set literal.
void setLiteralAddRange(const Terminal *low, const Terminal *high)
{
    //Literal integers are the only supported set ranges.
    if (low->token != yinteger || high->token != yinteger)
        cout << "***ERROR: Set ranges must be literal integers" << endl;

    //Make sure these values are in-range for this set.
    const int ilow = atoi(low->str.c_str());
    const int ihigh = atoi(high->str.c_str());
    SetType *set = (SetType*)tracker.peekType();
    if (!set->legalValue(ilow) || !set->legalValue(ihigh))
        cout << "***ERROR: Out-of-bounds set value" << endl;

    //Pass the whole range of values into IntSet::makeLiteral(). This will
    //generate pretty ridiculous code for large set ranges.
    for (int num = ilow; num <= ihigh; num++) {
        if (num > ilow)
            cout << ", ";
        cout << num;
    }
}

//Terminate a block.
void endBlock()
{
    //If currFunction is NULL, then we terminated a function within this
    //function, meaning there was a nested function. We do not support nested
    //functions, so print an error.
    if (currFunction)
        currFunction->endFunction();
    else
        cout << "***ERROR: Nested functions not supported\n";
        
    symTable.endScope();
    currFunction = NULL;
}
