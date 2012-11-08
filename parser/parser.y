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

#include "actions.h"

#include "ArrayType.h"
#include "SymTable.h"
#include "Variable.h"
#include "Function.h"
#include "Const.h"
#include "PointerType.h"
#include "RecordType.h"
#include "SetType.h"

/* method declarations section */
void yyerror(char const *);
int yylex(); /* needed by g++ */

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
                        AbstractType *td = new AbstractType($1, currType);
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
ProcedureDecl       : CreateFunc ProcedureHeading ysemicolon Block 
                    {
                        symTable.endScope();
                    }
                    ;
FunctionDecl        : CreateFunc FunctionHeading ycolon yident ysemicolon 
                    {
                        AbstractType *returnType = symTable.lookupType($4);
                        currFunction->returnType = returnType;
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
ProcedureHeading    : yprocedure yident
                    {
                        beginScope($2);
                        free($2);
                    }
                    | yprocedure yident
                    {
                        beginScope($2);
                        free($2);
                    }
                      FormalParameters
                    ;
FunctionHeading     : yfunction yident 
                    {
                        beginScope($2);
                        free($2);
                    }
                    | yfunction yident
                    {
                        beginScope($2);
                        free($2);
                    }
                      FormalParameters
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
