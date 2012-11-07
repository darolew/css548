%{
/*
 * PHASE 3: SYMBOL TABLE
 * CSS 548; Autumn 2012
 * Aaron Hoffer and Daniel Lewis
 *
 * This is a Yacc/Bison input file. Yacc generates a parserin C code from this
 * definition. 
 */
#include <iostream>
#include <stdio.h> 

#include "ArrayType.h"
#include "SymTable.h"
#include "Variable.h"
#include "Function.h"
#include "Const.h"
#include "NamedType.h"
#include "PointerType.h"
#include "RecordType.h"
#include "SetType.h"

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
list<string> idList;
list<Range> rangeList;
list<Ptrinfo> ptrList;
list<Variable> fieldList;
Function *currFunction = NULL;
AbstractType *currType = NULL;

/* method declarations section */
void yyerror(char const *);
int yylex(); /* needed by g++ */
void assignTypesToPointers(void);
void addPointerToList(string, string);
void insertCurrentVariableDecl(void);
void insertArrayType(void);
Terminal *newTerminal(string, int, char=NO_UNARY_OP);
void addRange(struct Terminal *, struct Terminal *);
void addField(void);
void addFormalParam(string);
bool isDuplicateField(string);
void beginScope(char *);

%}

/* Yacc definition section */

//Tell bison to expect 1 shift/reduce conflict.
%expect 1

//Tell bison to use the Yacc name prefix for generated files.
%file-prefix = "y"

%start  CompilationUnit
%token  yand yarray yassign ybegin ycaret ycase ycolon ycomma yconst ydiv
        ydivide ydo ydot ydotdot ydownto yelse yend yequal yfor yfunction
        ygreater ygreaterequal yif yin yleftbracket yleftparen yless
        ylessequal ymod ymultiply ynil ynot ynotequal yof yor
        yprocedure yprogram yrecord yrepeat yrightbracket yrightparen
        ysemicolon yset ythen yto ytype yunknown yuntil yvar ywhile

//Some tokens have lexemes that must be captured.
//These tokens are declared to use the str field of the union.        
%token <str> yident ynumber ystring 

//Some token values are be captured.
%token <tkn> yplus yminus

//Typed non-terminals. These non-terminals pass their value back to the rule
//from which they were included.
%type <term> ConstFactor ConstExpression
%type <chr> UnaryOperator
 
//The union is used for two reasons. The first is to capture information about
//lexemes from the scanner. The second is to define the data captured in parser
//rules.
%union {
    char *str;
    struct Terminal *term;
    int chr;
};
 
%%
/* rules section */

/**************************  Pascal program **********************************/

CompilationUnit     : ProgramModule
                    ;
ProgramModule       : yprogram yident ProgramParameters ysemicolon
                    {
                        symTable.beginScope($2);
                        free($2);                        
                    }
                      Block
                    {
                        symTable.endScope();
                    }
                      ydot
                    ;
ProgramParameters   : yleftparen IdentList2 yrightparen
                    ;
IdentList2          : yident 
                    {
                        free($1)
                    }
                    | IdentList2 ycomma yident 
                    {
                        free($3)
                    }                        
                    ;
IdentList           :  yident 
                    {
                        idList.push_front($1);
                        free($1);
                    } 
                    | IdentList ycomma yident 
                    {
                        idList.push_front($3);
                        free($3);
                    }
                    ; 

/**************************  Declarations section ***************************/

Block               : Declarations ybegin StatementSequence yend
                    ;
Declarations        : ConstantDefBlock
                      TypeDefBlock PointerCheck
                      VariableDeclBlock
                      SubprogDeclList  
                    ;
PointerCheck        : /*** empty ***/
                    {
                        //See method definition for explanation.
                        assignTypesToPointers();
                    }
                    ;
ConstantDefBlock    : /*** empty ***/
                    | yconst ConstantDefList
                    ;
ConstantDefList     : ConstantDef ysemicolon
                    | ConstantDefList ConstantDef ysemicolon
                    ;
TypeDefBlock        : /*** empty ***/
                    | ytype TypeDefList          
                    ;
TypeDefList         : TypeDef ysemicolon
                    | TypeDefList TypeDef ysemicolon  
                    ;
VariableDeclBlock   : /*** empty ***/
                    | yvar VariableDeclList
                    ;
VariableDeclList    : VariableDecl ysemicolon
                    | VariableDeclList VariableDecl ysemicolon
                    ;
ConstantDef         : yident yequal ConstExpression
                    {
                        symTable.insert(new Const($1, *$3));
                        free($1);
                        delete $3;
                    }
                    ;
TypeDef             : yident yequal NPType
                    {
                        NamedType *td = new NamedType($1, currType);
                        free($1);
                        symTable.insert(td);
                    }
                    | PointerTypeDef
                    ;
PointerTypeDef      : yident yequal ycaret yident
                    {
                        addPointerToList($1, $4);
                        free($1);
                        free($4);
                    }
                    ;
VariableDecl        : IdentList ycolon Type
                    {
                        insertCurrentVariableDecl();
                    }
                    ;

/***************************  Const/Type Stuff  ******************************/

ConstExpression     : UnaryOperator ConstFactor
                    {
                        $$ = $2;
                        $$->unaryOp = $1;                    
                    }
                    | ConstFactor 
                    | ystring 
                    {
                        $$ = newTerminal($1, ystring);
                        free($1);
                    }
                    ;
ConstFactor         : yident 
                    {
                        $$ = newTerminal($1, yident);
                        free($1);
                    }
                    | ynumber
                    {
                        $$ = newTerminal($1, ynumber);
                        free($1);
                        
                    }
                    | ynil
                    {
                        $$ = newTerminal("nil", ynil);
                    }
                    ;
Type                : yident
                    {
                        currType = symTable.lookupType($1);
                        free($1);
                    }
                    | ArrayType  
                    | PointerType 
                    | RecordType 
                    | SetType 
                    ;
NPType              : yident
                    {
                        currType = symTable.lookupType($1);
                        free($1);
                    }
                    | ArrayType 
                    | RecordType 
                    | SetType 
                    ;
ArrayType           : yarray yleftbracket Subrange SubrangeList 
                      yrightbracket yof Type
                    {
                        insertArrayType();
                    }
                    ;
SubrangeList        : /*** empty ***/
                    | SubrangeList ycomma Subrange 
                    ;
Subrange            : ConstFactor ydotdot ConstFactor
                    {
                        addRange($1, $3);
                        delete $1;
                        delete $3;
                    }
                    | ystring ydotdot ystring 
                    {
                        //TODO: handle character subranges [a..z]
                        free($1);
                        free($3);
                    }
                    ;
RecordType          : yrecord FieldListSequence yend
                    {
                        currType = new RecordType(fieldList);
                        fieldList.erase(fieldList.begin(), fieldList.end());
                    }
                    ;
SetType             : yset yof Subrange
                    {
                        currType = new SetType(rangeList.front());
                        rangeList.pop_front();
                    }
                    ;
PointerType         : ycaret yident
                    {
                        currType = symTable.lookupType($2);
                        free($2);
                    }
                    ;
FieldListSequence   : FieldList
                    | FieldListSequence ysemicolon FieldList
                    ;
FieldList           : IdentList ycolon Type
                    {
                      addField();  
                    }
                    ;

/***************************  Statements  ************************************/

StatementSequence   : Statement  
                    | StatementSequence ysemicolon Statement
                    ;
Statement           : Assignment
                    | ProcedureCall
                    | IfStatement
                    | CaseStatement
                    | WhileStatement
                    | RepeatStatement
                    | ForStatement
                    | ybegin StatementSequence yend
                    | /*** empty ***/
                    ;
Assignment          : Designator yassign Expression
                    ;
ProcedureCall       : yident
                    {
                        //Generate code 
                        free($1);
                    }
                    | yident ActualParameters
                    {
                        //Generate code
                        free($1);
                    }

                    ;
IfStatement         : yif Expression ythen Statement ElsePart
                    ;
ElsePart            : /*** empty ***/
                    | yelse Statement   
                    ;
CaseStatement       : ycase Expression yof CaseList yend
                    ;
CaseList            : Case
                    | CaseList ysemicolon Case  
                    ;
Case                : CaseLabelList ycolon Statement
                    ;
CaseLabelList       : ConstExpression  
                    | CaseLabelList ycomma ConstExpression   
                    ;
WhileStatement      : ywhile Expression ydo Statement  
                    ;
RepeatStatement     : yrepeat StatementSequence yuntil Expression
                    ;
ForStatement        : yfor yident yassign Expression WhichWay Expression
                      ydo Statement
                    {
                        //Generate code
                        free($2);
                    }
                    ;
WhichWay            : yto | ydownto
                    ;

/***************************  Designator Stuff  ******************************/

Designator          : yident DesignatorStuff
                    {
                        //Do designator stuff here
                        free($1);
                    }                        
                    ;
DesignatorStuff     : /*** empty ***/
                    | DesignatorStuff theDesignatorStuff
                    ;
theDesignatorStuff  : ydot yident
                    {
                        //Do the the designator stuff here
                        free($2);
                    }
                    | yleftbracket ExpList yrightbracket 
                    | ycaret 
                    ;
ActualParameters    : yleftparen ExpList yrightparen
                    ;
ExpList             : Expression   
                    | ExpList ycomma Expression     
                    ;

/***************************  Expression Stuff  ******************************/
Expression          : SimpleExpression  
                    | SimpleExpression Relation SimpleExpression 
                    ;
SimpleExpression    : TermExpr
                    | UnaryOperator TermExpr
                    ;
TermExpr            : Term  
                    | TermExpr AddOperator Term
                    ;
Term                : Factor  
                    | Term MultOperator Factor
                    ;
Factor              : ynumber
                    | ynil
                    | ystring 
                    | Designator
                    | yleftparen Expression yrightparen
                    | ynot Factor
                    | Setvalue
                    | FunctionCall
                    ;
FunctionCall        : yident ActualParameters
                    {
                        //Generate code
                        free($1);
                    }
                    ;
Setvalue            : yleftbracket ElementList yrightbracket
                    | yleftbracket yrightbracket
                    ;
ElementList         : Element  
                    | ElementList ycomma Element
                    ;
Element             : ConstExpression  
                    | ConstExpression ydotdot ConstExpression 
                    ;

/***************************  Subprogram Stuff  ******************************/

SubprogDeclList     : /*** empty ***/
                    | SubprogDeclList ProcedureDecl ysemicolon  
                    | SubprogDeclList FunctionDecl ysemicolon
                    ;
ProcedureDecl       : CreateFunc ProcedureHeading ysemicolon
                    {
                        symTable.insert(currFunction);
                    }
                      Block 
                    {
                        symTable.endScope();
                    }
                    ;
CreateFunc          : /*** empty ***/
                    {
                        currFunction = new Function();
                    }
                    ; 
FunctionDecl        : CreateFunc FunctionHeading ycolon yident ysemicolon 
                    {
                        AbstractType *returnType = symTable.lookupType($4);
                        currFunction->returnType = returnType;
                        symTable.insert(currFunction);  
                    }
                      Block
                    {
                        symTable.endScope();
                    }
                    ;
ProcedureHeading    : yprocedure yident
                    {
                        beginScope($2);
                        free($2);
                    }
                    | yprocedure yident FormalParameters
                    {
                        beginScope($2);
                        free($2);
                    }
                    ;
FunctionHeading     : yfunction yident 
                    {
                        beginScope($2);
                        free($2);
                    }
                    | yfunction yident FormalParameters
                    {
                        beginScope($2);
                        free($2);
                    }
                    ;
FormalParameters    : yleftparen FormalParamList yrightparen 
                    ;
FormalParamList     : OneFormalParam 
                    | FormalParamList ysemicolon OneFormalParam
                    ;
OneFormalParam      : FormalParamFlag IdentList ycolon yident 
                    {
                        addFormalParam($4);
                        free($4);
                    }
                    ;
FormalParamFlag     : /*** nothing ***/
                    | yvar
                    ;

/***************************  More Operators  ********************************/

UnaryOperator       : yplus { $$ = '+'; } | yminus { $$ = '-'; }
                    ;
MultOperator        : ymultiply | ydivide | ydiv | ymod | yand 
                    ;
AddOperator         : yplus | yminus | yor
                    ;
Relation            : yequal | ynotequal | yless | ygreater 
                    | ylessequal | ygreaterequal | yin
                    ;

%%

/* program section */
void yyerror(const char *s)
{
    fprintf(stderr, "%s\n", s);
}

//This method walks through the lists of pointers declared in the source code.
//It assigns a pointed-to-type to each pointer. This method is invoked when
//exiting the type def block in the source code. It allows pointers to point
//to types that have not yet been defined.
void assignTypesToPointers(void)
{
    while (!ptrList.empty()) {
        Ptrinfo pi = ptrList.front();
        pi.ptrType->addType(*pi.pointee);
        
        //The string pointee was copied in the previous statement. 
        //Free the memory.
        delete pi.pointee;
        
        ptrList.pop_front();
    }
}
//When a pointer type is defined, a new object for it and inserted into the 
//symbol table. The type of the pointer is saved locally as a string, along
//with its pointer object. That string will be used as a key to lookup the 
//type object at the end of the type def block.
void addPointerToList(string nameOfPointer, string nameOfPointee)
{
    Ptrinfo pi;
    pi.ptrType = new PointerType(nameOfPointer);
    pi.pointee = new string(nameOfPointee);
    ptrList.push_front(pi);
    symTable.insert(pi.ptrType);
}

//Walk the list of variable names being declared. For example, the declaration
//"a,b,c : interger;" includes a list of variables {a, b, c} and their type, 
//integer. For each one, a new variable object is created, assigned a type,
//and entered into the symbol table. The list is emptied as the varaibles are
//inserted into the symbol table.
 void insertCurrentVariableDecl(void) 
 {
    while (!idList.empty()) {
        string name = idList.front();
        symTable.insert(new Variable(name, currType));
        idList.pop_front();
    }
}                        
//Convience method for creating Terminal struct.
Terminal *newTerminal(string lexeme, int token, char unaryOperatorChar)
{
    Terminal *terminal = new Terminal;
    terminal->str = lexeme;
    terminal->token = token;
    terminal->unaryOp = unaryOperatorChar;
    return terminal;
}

//Create a new array type and add it to the symbol table.
//Use the ranges build up in the rangeList. Empty the range
//list when the insert is complete.
void insertArrayType()
{
    AbstractType *elementType = currType;
    currType = new ArrayType(elementType, rangeList);
    rangeList.erase(rangeList.begin(), rangeList.end());
}

//Create a range object, set its members, and push it on a list.
void addRange(struct Terminal *low, struct Terminal *high)
{
    Range range;
    range.low = *low;
    range.high = *high;
    rangeList.push_front(range);
}
 
//Remove an identifer and turn it into a varaible as part of a record's fields.
void addField()
{
    while (!idList.empty()) {
        string id = idList.front();
        if (isDuplicateField(id)) {
            cerr << "error: " << id << " already exists in record\n";
            //exit(1);
        }
        Variable field(id, currType);
        fieldList.push_front(field);
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

//Create a formal method parameter from a list of identifiers.
//Add the parameter to the object for the current function.
void addFormalParam(string typeName) 
{
    while (!idList.empty()) {
        string name = idList.front();
        AbstractType *formalType = symTable.lookupType(typeName);
        Variable *formalParam = new Variable(name, formalType);
        currFunction->params.push_front(formalParam);
        idList.pop_front();
    }
}

//Not much to say about this one.
void beginScope(char *name) 
{
    symTable.beginScope(name);
    currFunction->identifier = name;
}