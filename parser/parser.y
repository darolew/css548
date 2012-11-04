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

#include "Array.h"
#include "SymTable.h"
#include "Variable.h"
#include "Const.h"
#include "TypeDef.h"
#include "PointerType.h" 

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

list<string> idList;
list<Range> rangeList;
list<PointerType*> ptrList;

%}

/* definition section */

%start  CompilationUnit
%token  yand yarray yassign ybegin ycaret ycase ycolon ycomma yconst ydiv
        ydivide ydo ydot ydotdot ydownto yelse yend yequal yfor yfunction
        ygreater ygreaterequal yif yin yleftbracket yleftparen yless
        ylessequal yminus ymod ymultiply ynot ynotequal ynumber yof yor
        yplus yprocedure yprogram yrecord yrepeat yrightbracket yrightparen
        ysemicolon yset ythen yto ytype yuntil yvar ywhile
        
%token <str> yident ynumber ynil ydispose ynew yread yreadln ystring ytrue yfalse ywrite ywriteln yunknown

/* %type <str> Type */
%type <term> ConstFactor ConstExpression

/*
%type   CompilationUnit ProgramModule ProgramParameters IdentList Block
        Declarations ConstantDefBlock ConstantDefList TypeDefBlock
        ConstantDefList TypeDefBlock TypeDefList VariableDeclBlock 
        VariableDeclList ConstantDef TypeDef VariableDecl ConstExpression
        ConstFactor Type ArrayType SubrangeList Subrange RecordType SetType
        PointerType FieldListSequence FieldList StatementSequence Statements
        Assignment ProcedureCall IfStatement ElsePart CaseStatement CaseList
        Case CaseLabelList WhileStatement RepeatStatement ForStatement WhichWay
        IOStatement DesignatorList Designator DesignatorStuff theDesignatorStuff
        ActualParameters ExpList MemoryStatement Expression SimpleExpression
        TermExpr Term Factor FunctionCall Setvalue ElementList Element SubprogDeclList
        ProcedureDecl FunctionDecl ProcedureHeading FunctionHeading FormalParameters
        FormalParamList OneFormalParam UnaryOperator MultOperator AddOperator
        Relation
*/

%union {
    char *str;
    struct Terminal *term;
};
 
%%
/* rules section */

/**************************  Pascal program **********************************/

CompilationUnit     : ProgramModule
                    ;
ProgramModule       : yprogram yident ProgramParameters ysemicolon Block ydot
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

Block               : Declarations PointerCheck ybegin StatementSequence yend
                    ;
Declarations        : ConstantDefBlock
                      TypeDefBlock
                      VariableDeclBlock
                      SubprogDeclList  
                    ;
PointerCheck        : /*** empty ***/
					{
						while (!ptrList.empty()) {
							PointerType *ptrType = ptrList.front();
							ptrType->addType();
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
                        symTable.insert(new Const($1, $3->str));
                    }
                    ;
BasicTypeDef        : yident yequal yident 
                    {
                        symTable.insert(new TypeDef($1, $3));
                    }
ArrayTypeDef        : yident yequal yarray yleftbracket Subrange SubrangeList
                      yrightbracket yof yident
                    {
                        symTable.insert(new Array($1, rangeList, $9));
                        rangeList.erase(rangeList.begin(), rangeList.end());
                    }
PointerTypeDef      : yident yequal ycaret yident
                    {
                    	PointerType *ptrType = new PointerType($1, $4);
                    	ptrList.push_front(ptrType);
                    	symTable.insert(ptrType);
                    }
                    ;
RecordTypeDef       :
                    ;
SetTypeDef          :
                    ;
TypeDef             : BasicTypeDef
                    | ArrayTypeDef
                    | PointerTypeDef
                    ;
/*VariableDecl        : IdentList ycolon Type*/ /* TODO: Support complicated types */
VariableDecl        : IdentList ycolon yident
                    {
                        while (!idList.empty()) {
                            symTable.insert(new Variable(idList.front(), $3));
                            idList.pop_front();
                        } 
                    }
                    ;

/***************************  Const/Type Stuff  ******************************/

ConstExpression     : UnaryOperator ConstFactor /* TODO: Capture unary operator */
                    | ConstFactor 
                    | ystring 
                    {
                        $$ = new Terminal;
                        $$->str = $1;
                        $$->token = ystring;
                    }
                    ;
ConstFactor         : yident 
                    {
                        $$ = new Terminal;
                        $$->str = $1;
                        $$->token = yident;
                    }
                    | ynumber
                    {
                        $$ = new Terminal;
                        $$->str = $1;
                        $$->token = ynumber;
                    }
                    | ynil
                    {
                        $$ = new Terminal;
                        $$->str = $1;
                        $$->token = ynil;
                    }
                    ;
Type                : yident
                    | ArrayType 
                    | PointerType 
                    | RecordType 
                    | SetType 
                    ;
ArrayType           : yarray yleftbracket Subrange SubrangeList 
                      yrightbracket yof Type
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
                    ;
SetType             : yset yof Subrange
                    ;
PointerType         : ycaret yident 
                    ;
FieldListSequence   : FieldList  
                    | FieldListSequence ysemicolon FieldList
                    ;
FieldList           : IdentList ycolon Type { idList.erase(idList.begin(), idList.end()); }
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
                    | IOStatement
                    | MemoryStatement
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
IOStatement         : yread yleftparen DesignatorList yrightparen
                    | yreadln  
                    | yreadln yleftparen DesignatorList yrightparen 
                    | ywrite yleftparen ExpList yrightparen
                    | ywriteln  
                    | ywriteln yleftparen ExpList yrightparen 
                    ;

/***************************  Designator Stuff  ******************************/

DesignatorList      : Designator
                    | DesignatorList ycomma Designator 
                    ;
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
MemoryStatement     : ynew yleftparen yident yrightparen
                    | ydispose yleftparen yident yrightparen
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
                    | ytrue
                    | yfalse
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

SubprogDeclList     :  /*** empty ***/
                    | SubprogDeclList ProcedureDecl ysemicolon  
                    | SubprogDeclList FunctionDecl ysemicolon
                    ;
ProcedureDecl       : ProcedureHeading ysemicolon Block 
                    ;
FunctionDecl        : FunctionHeading ycolon yident ysemicolon Block
                    ;
ProcedureHeading    : yprocedure yident
                    | yprocedure yident FormalParameters
                    ;
FunctionHeading     : yfunction yident 
                    | yfunction yident FormalParameters
                    ;
FormalParameters    : yleftparen FormalParamList yrightparen 
                    ;
FormalParamList     : OneFormalParam 
                    | FormalParamList ysemicolon OneFormalParam
                    ;
OneFormalParam      : yvar IdentList ycolon yident { idList.erase(idList.begin(), idList.end()); }
                    | IdentList ycolon yident { idList.erase(idList.begin(), idList.end()); }
                    ;

/***************************  More Operators  ********************************/

UnaryOperator       : yplus | yminus
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
