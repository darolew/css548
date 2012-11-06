%{
/*
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

/* Macro for releasing memory allocated by strdup() in the lexer.
 * X represents the union of lvals.
 */
#define FREE(X) {free((X).str); (X).str = NULL;}

/* Macro for printing identifiers. Also frees the memory allocated by
 * strdup() in the lexer. 
 */
#define PRINTID(X) {printf("%s ", (X).str); FREE(X)}

/* declarations section */
void yyerror(char const *);
int yylex(); /* needed by g++ */

extern SymTable symTable;

typedef struct {
    PointerType *ptrType;
    string *pointee;
} Ptrinfo;

list<string> idList;
list<Range> rangeList;
list<Ptrinfo> ptrList;
list<Variable*> fieldList;
Function *currFunction;

AbstractType *currType;

%}

/* definition section */

%start  CompilationUnit
%token  yand yarray yassign ybegin ycaret ycase ycolon ycomma yconst ydiv
        ydivide ydo ydot ydotdot ydownto yelse yend yequal yfor yfunction
        ygreater ygreaterequal yif yin yleftbracket yleftparen yless
        ylessequal ymod ymultiply ynil ynot ynotequal ynumber yof yor
        yprocedure yprogram yrecord yrepeat yrightbracket yrightparen
        ysemicolon yset ythen yto ytype yuntil yvar ywhile
        
%token <str> yident ynumber ystring yunknown

%token <tkn> yplus yminus

%type <term> ConstFactor ConstExpression
%type <chr> UnaryOperator


/*
%type   CompilationUnit ProgramModule ProgramParameters IdentList Block
        Declarations ConstantDefBlock ConstantDefList TypeDefBlock
        ConstantDefList TypeDefBlock TypeDefList VariableDeclBlock 
        VariableDeclList ConstantDef TypeDef VariableDecl ConstExpression
        ConstFactor Type ArrayType SubrangeList Subrange RecordType SetType
        PointerType FieldListSequence FieldList StatementSequence Statements
        Assignment ProcedureCall IfStatement ElsePart CaseStatement CaseList
        Case CaseLabelList WhileStatement RepeatStatement ForStatement WhichWay
        DesignatorList Designator DesignatorStuff theDesignatorStuff
        ActualParameters ExpList Expression SimpleExpression
        TermExpr Term Factor FunctionCall Setvalue ElementList Element SubprogDeclList
        ProcedureDecl FunctionDecl ProcedureHeading FunctionHeading FormalParameters
        FormalParamList OneFormalParam UnaryOperator MultOperator AddOperator
        Relation
*/

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
                    }
                      Block
                    {
                        symTable.endScope();
                    }
                      ydot
                    ;
ProgramParameters   : yleftparen IdentList2 yrightparen
                    ;
IdentList2          : yident | IdentList2 ycomma yident
IdentList           :  yident
                    {
                        idList.push_front($1);
                    } 
                    | IdentList ycomma yident
                    {
                        idList.push_front($3);
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
                        while (!ptrList.empty()) {
                            Ptrinfo pi = ptrList.front();
                            pi.ptrType->addType(*pi.pointee);
                            ptrList.pop_front();
                        }
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
                    }
                    ;
TypeDef             : yident yequal NPType
                    {
                        NamedType *td = new NamedType($1, currType);
                        symTable.insert(td);
                    }
                    | PointerTypeDef
                    ;
PointerTypeDef      : yident yequal ycaret yident
                    {
                        Ptrinfo pi;
                        pi.ptrType = new PointerType($1);
                        pi.pointee = new string($4);
                        ptrList.push_front(pi);
                        symTable.insert(pi.ptrType);
                    }
                    ;
VariableDecl        : IdentList ycolon Type
                    {
                        while (!idList.empty()) {
                            string name = idList.front();
                            symTable.insert(new Variable(name, currType));
                            idList.pop_front();
                        } 
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
                        $$ = new Terminal;
                        $$->str = $1;
                        $$->token = ystring;
                        $$->unaryOp = 0;
                    }
                    ;
ConstFactor         : yident 
                    {
                        $$ = new Terminal;
                        $$->str = $1;
                        $$->token = yident;
                        $$->unaryOp = 0;
                    }
                    | ynumber
                    {
                        $$ = new Terminal;
                        $$->str = $1;
                        $$->token = ynumber;
                        $$->unaryOp = 0;
                    }
                    | ynil
                    {
                        $$ = new Terminal;
                        $$->str = "nil";
                        $$->token = ynil;
                        $$->unaryOp = 0;
                    }
                    ;
Type                : yident
                    {
                        currType = symTable.lookupType($1);
                    }
                    | ArrayType 
                    | PointerType 
                    | RecordType 
                    | SetType 
                    ;
NPType              : yident
                    {
                        currType = symTable.lookupType($1);
                    }
                    | ArrayType 
                    | RecordType 
                    | SetType 
                    ;
ArrayType           : yarray yleftbracket Subrange SubrangeList 
                      yrightbracket yof Type
                    {
                        AbstractType *elementType = currType;
                        currType = new ArrayType(elementType, rangeList);
                        rangeList.erase(rangeList.begin(), rangeList.end());
                    }
                    ;
SubrangeList        : /*** empty ***/
                    | SubrangeList ycomma Subrange 
                    ;
Subrange            : ConstFactor ydotdot ConstFactor
                    {
                        Range range;
                        range.low = *$1;
                        range.high = *$3;
                        rangeList.push_front(range);
                    }
                    | ystring ydotdot ystring 
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
                    }
                    ;
FieldListSequence   : FieldList
                    | FieldListSequence ysemicolon FieldList
                    ;
FieldList           : IdentList ycolon Type
                    {
                        while (!idList.empty()) {
                            string id = idList.front();
                            list<Variable*>::iterator it = fieldList.begin();
                            for (; it != fieldList.end(); it++) {
                                Variable *prev = *it;
                                if (id == prev->identifier) {
                                    cerr << "error: " << id << " already exists in record\n";
                                    //exit(1);
                                }
                            }
                            Variable *field = new Variable(id, currType);
                            fieldList.push_front(field);
                            idList.pop_front();
                        } 
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
                    | yident ActualParameters
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
                    ;
WhichWay            : yto | ydownto
                    ;

/***************************  Designator Stuff  ******************************/

Designator          : yident DesignatorStuff
                    ;
DesignatorStuff     : /*** empty ***/
                    | DesignatorStuff theDesignatorStuff
                    ;
theDesignatorStuff  : ydot yident
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
                        symTable.beginScope($2);
                        currFunction->identifier=$2;
                    }
                    | yprocedure yident FormalParameters
                    {
                        symTable.beginScope($2);
                        currFunction->identifier=$2;                        
                    }
                    ;
FunctionHeading     : yfunction yident 
                    {
                        symTable.beginScope($2);
                        currFunction->identifier=$2;
                    }
                    | yfunction yident FormalParameters
                    {
                        symTable.beginScope($2);
                        currFunction->identifier=$2;
                    }
                    ;
FormalParameters    : yleftparen FormalParamList yrightparen 
                    ;
FormalParamList     : OneFormalParam 
                    | FormalParamList ysemicolon OneFormalParam
                    ;
OneFormalParam      : FormalParamFlag IdentList ycolon yident 
                    {
                        while (!idList.empty()) {
                            string name = idList.front();
                            AbstractType *formalType = symTable.lookupType($4);
                            Variable *formalParam = new Variable(name, formalType);
                            currFunction->params.push_front(formalParam);
                            idList.pop_front();
                        } 
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
