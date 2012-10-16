%{
/*
 * CSS 548; Autumn 2012
 * Aaron Hoffer and Daniel Lewis
 *
 * Phase II of the CSS 548 compiler project.
 * This is a Yacc/Bison input file. Yacc generates a parserin C code from this
 * definition. 
 * 
 */

#include <stdio.h>

/* Macro for printing identifiers. Also frees the memory allocated by
 * strdup() in the lexer. 
 */
#define PRINTID(X) {printf("%s ", (X).str); free((X).str); (X).str = NULL;}

/* declarations section */
void yyerror(char const *);


%}

%union {
  char *str;
}

/* definition section */

%start  CompilationUnit
%token  yand yarray yassign ybegin ycaret ycase ycolon ycomma yconst ydispose 
        ydiv ydivide ydo ydot ydotdot ydownto yelse yend yequal yfalse
        yfor yfunction ygreater ygreaterequal yident yif yin yleftbracket
        yleftparen yless ylessequal yminus ymod ymultiply ynew ynil ynot 
        ynotequal ynumber yof yor yplus yprocedure yprogram yread yreadln  
        yrecord yrepeat yrightbracket yrightparen ysemicolon yset ystring
        ythen yto ytrue ytype yuntil yvar ywhile ywrite ywriteln yunknown

%%
/* rules section */

/**************************  Pascal program **********************************/

CompilationUnit    :  ProgramModule        
                   ;
ProgramModule      :  yprogram yident {PRINTID($2);} ProgramParameters ysemicolon Block ydot
                   ;
ProgramParameters  :  yleftparen  IdentList  yrightparen
                   ;
IdentList          :  yident {PRINTID($1);}
                   |  IdentList ycomma yident {PRINTID($3);}
                   ;

/**************************  Declarations section ***************************/

Block              :  Declarations  ybegin  StatementSequence  yend
                   ;
Declarations       :  ConstantDefBlock
                      TypeDefBlock
                      VariableDeclBlock
                      SubprogDeclList  
                   ;
ConstantDefBlock   :  /*** empty ***/
                   |  yconst ConstantDefList
                   ;
ConstantDefList    :  ConstantDef ysemicolon
                   |  ConstantDefList ConstantDef ysemicolon
                   ;
TypeDefBlock       :  /*** empty ***/
                   |  ytype  TypeDefList          
                   ;
TypeDefList        :  TypeDef  ysemicolon
                   |  TypeDefList TypeDef ysemicolon  
                   ;
VariableDeclBlock  :  /*** empty ***/
                   |  yvar VariableDeclList
                   ;
VariableDeclList   :  VariableDecl ysemicolon
                   |  VariableDeclList VariableDecl ysemicolon
                   ;  
ConstantDef        :  yident {PRINTID($1);}  yequal ConstExpression
                   ;
TypeDef            :  yident {PRINTID($1);}  yequal  Type
                   ;
VariableDecl       :  IdentList  ycolon  Type
                   ;

/***************************  Const/Type Stuff  ******************************/

ConstExpression    :  UnaryOperator ConstFactor
                   |  ConstFactor
                   |  ystring
                   ;
ConstFactor        :  yident {PRINTID($1);}
                   |  ynumber
                   |  ytrue
                   |  yfalse
                   |  ynil
                   ;
Type               :  yident {PRINTID($1);}
                   |  ArrayType
                   |  PointerType
                   |  RecordType
                   |  SetType
                   ;
ArrayType          :  yarray yleftbracket Subrange SubrangeList 
                      yrightbracket  yof Type
                   ;
SubrangeList       :  /*** empty ***/
                   |  SubrangeList ycomma Subrange 
                   ;
Subrange           :  ConstFactor ydotdot ConstFactor
                   |  ystring ydotdot  ystring
                   ;
RecordType         :  yrecord  FieldListSequence  yend
                   ;
SetType            :  yset  yof  Subrange
                   ;
PointerType        :  ycaret  yident  {PRINTID($2);}
                   ;
FieldListSequence  :  FieldList  
                   |  FieldListSequence  ysemicolon  FieldList
                   ;
FieldList          :  IdentList  ycolon  Type
                   ;

/***************************  Statements  ************************************/

StatementSequence  :  Statement  
                   |  StatementSequence  ysemicolon  Statement
                   ;
Statement          :  Assignment
                   |  ProcedureCall
                   |  IfStatement
                   |  CaseStatement
                   |  WhileStatement
                   |  RepeatStatement
                   |  ForStatement
                   |  IOStatement
                   |  MemoryStatement
                   |  ybegin StatementSequence yend
                   |  /*** empty ***/
                   ;
Assignment         :  Designator yassign Expression
                   ;
ProcedureCall      :  yident {PRINTID($1);}
                   |  yident {PRINTID($1);} ActualParameters
                   ;
IfStatement        :  yif  Expression  ythen  Statement  ElsePart
                   ;
ElsePart           :  /*** empty ***/
                   |  yelse  Statement  
                   ;
CaseStatement      :  ycase  Expression  yof  CaseList  yend
                   ;
CaseList           :  Case
                   |  CaseList  ysemicolon  Case  
                   ;
Case               :  CaseLabelList  ycolon  Statement
                   ;
CaseLabelList      :  ConstExpression  
                   |  CaseLabelList  ycomma  ConstExpression   
                   ;
WhileStatement     :  ywhile  Expression  ydo  Statement  
                   ;
RepeatStatement    :  yrepeat  StatementSequence  yuntil  Expression
                   ;
ForStatement       :  yfor  yident  {PRINTID($2);}  yassign  Expression  WhichWay  Expression
                            ydo  Statement
                   ;
WhichWay           :  yto  |  ydownto
                   ;
IOStatement        :  yread  yleftparen  DesignatorList  yrightparen
                   |  yreadln  
                   |  yreadln  yleftparen DesignatorList  yrightparen 
                   |  ywrite  yleftparen  ExpList  yrightparen
                   |  ywriteln  
                   |  ywriteln  yleftparen  ExpList  yrightparen 
                   ;

/***************************  Designator Stuff  ******************************/

DesignatorList     :  Designator  
                   |  DesignatorList  ycomma  Designator 
                   ;
Designator         :  yident {PRINTID($1);} DesignatorStuff
                   ;
DesignatorStuff    :  /*** empty ***/
                   |  DesignatorStuff  theDesignatorStuff
                   ;
theDesignatorStuff :  ydot yident {PRINTID($2);}
                   |  yleftbracket ExpList yrightbracket 
                   |  ycaret 
                   ;
ActualParameters   :  yleftparen  ExpList  yrightparen
                   ;
ExpList            :  Expression   
                   |  ExpList  ycomma  Expression       
                   ;
MemoryStatement    :  ynew  yleftparen  yident  {PRINTID($3);} yrightparen
                   |  ydispose yleftparen  yident  {PRINTID($3);} yrightparen
                   ;

/***************************  Expression Stuff  ******************************/

Expression         :  SimpleExpression  
                   |  SimpleExpression  Relation  SimpleExpression 
                   ;
SimpleExpression   :  TermExpr
                   |  UnaryOperator  TermExpr
                   ;
TermExpr           :  Term  
                   |  TermExpr  AddOperator  Term
                   ;
Term               :  Factor  
                   |  Term  MultOperator  Factor
                   ;
Factor             :  ynumber
                   |  ytrue
                   |  yfalse
                   |  ynil
                   |  ystring
                   |  Designator
                   |  yleftparen  Expression  yrightparen
                   |  ynot Factor
                   |  Setvalue
                   |  FunctionCall
                   ;
FunctionCall       :  yident {PRINTID($1);} ActualParameters
                   ;
Setvalue           :  yleftbracket ElementList  yrightbracket
                   |  yleftbracket yrightbracket
                   ;
ElementList        :  Element  
                   |  ElementList  ycomma  Element
                   ;
Element            :  ConstExpression  
                   |  ConstExpression  ydotdot  ConstExpression 
                   ;

/***************************  Subprogram Stuff  ******************************/

SubprogDeclList    :  /*** empty ***/
                   |  SubprogDeclList ProcedureDecl ysemicolon  
                   |  SubprogDeclList FunctionDecl ysemicolon
                   ;
ProcedureDecl      :  ProcedureHeading  ysemicolon  Block 
                   ;
FunctionDecl       :  FunctionHeading  ycolon  yident {PRINTID($3);} ysemicolon  Block
                   ;
ProcedureHeading   :  yprocedure  yident {PRINTID($2);}
                   |  yprocedure  yident {PRINTID($2);} FormalParameters
                   ;
FunctionHeading    :  yfunction  yident {PRINTID($2);}
                   |  yfunction  yident {PRINTID($2);} FormalParameters
                   ;
FormalParameters   :  yleftparen FormalParamList yrightparen 
                   ;
FormalParamList    :  OneFormalParam 
                   |  FormalParamList ysemicolon OneFormalParam
                   ;
OneFormalParam     :  yvar  IdentList  ycolon  yident {PRINTID($4);}
                   |  IdentList  ycolon  yident {PRINTID($3);}
                   ;

/***************************  More Operators  ********************************/

UnaryOperator      :  yplus | yminus
                   ;
MultOperator       :  ymultiply | ydivide | ydiv | ymod | yand 
                   ;
AddOperator        :  yplus | yminus | yor
                   ;
Relation           :  yequal  | ynotequal | yless | ygreater 
                   |  ylessequal | ygreaterequal | yin
                   ;

%%

/* program section */
void yyerror(const char *s) {
   fprintf(stderr, "%s\n", s);
}

int main() {
   int result = yyparse();
   return result;
}
