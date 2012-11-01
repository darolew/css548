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

 #include "AbstractType.h"
 #include "SymTable.h"
 #include "Variable.h"
 
 #include <iostream>
 
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

/* Include forward declarations so g++ does not complain. */
int yylex();

/* EXTERN */
extern SymTable symTable;

struct IdList {
list<string> l;
};

%}

/* definition section */

%start  CompilationUnit
%token  yand yarray yassign ybegin ycaret ycase ycolon ycomma yconst ydispose 
        ydiv ydivide ydo ydot ydotdot ydownto yelse yend yequal yfalse
        yfor yfunction ygreater ygreaterequal yif yin yleftbracket
        yleftparen yless ylessequal yminus ymod ymultiply ynew ynil ynot 
        ynotequal ynumber yof yor yplus yprocedure yprogram yread yreadln  
        yrecord yrepeat yrightbracket yrightparen ysemicolon yset ystring
        ythen yto ytrue ytype yuntil yvar ywhile ywrite ywriteln yunknown
		
%token <str> yident		
%type <idlistPtr> IdentList
%type <str> Type

/*
%type CompilationUnit ProgramModule ProgramParameters IdentList Block
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
  struct IdList* idlistPtr;
 };
 
%%
/* rules section */

/**************************  Pascal program **********************************/

CompilationUnit:  ProgramModule
                   ;
ProgramModule:  yprogram yident ProgramParameters ysemicolon Block ydot
                   ;
ProgramParameters  :  yleftparen  IdentList  yrightparen
                   ;
IdentList          :  yident { struct IdList* ptr = new IdList(); 
								(ptr->l).push_front($1);
								$$ = ptr;} 
                   |  IdentList ycomma yident { ($1->l).push_front($3); $$=$1; }
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
ConstantDef        :  yident yequal ConstExpression
                   ;
TypeDef            :  yident yequal  Type
                   ;
VariableDecl       :  IdentList  ycolon  Type { 
									/* I don't think I like the object-oriented programming interface for
									 * for this application. Would it make more sense to have an "addVariable"
									 * method on the SymTable class? For example:
									 *
									 *      addVaraible(list<string> ids, string type_name)
									 * 
									 * Let SymTable do the lookup on the type_name. 
									 * 
									 * How do we handle tricky type like array or record? 
									 * For example, to construct an array type we
									 * would need to create a data structure in %union for Subrange and pass it
									 * back to the ArrayType non-terminal when we create the the ArrayType.
									 * then ArrayType gets inserted into the symbol table? Or do we hang ArrayType
									 * off the Variable object and insert the variable into the symbol table? 
									 * Damn. It just gets deeper and deeper....
									 *
									 * As far as I can tell it is a BIG mistake to create a symbol table without first 
									 * createing the abstract syntax tree.
									 */
								 for (list<string>::iterator it = $1->l.begin(); it != $1->l.end(); it++) 
									symTable.insert(new Variable(*it, symTable.lookup($3)));
					}
                   ;

/***************************  Const/Type Stuff  ******************************/

ConstExpression    :  UnaryOperator ConstFactor
                   |  ConstFactor
                   |  ystring 
                   ;
ConstFactor        :  yident 
                   |  ynumber 
                   |  ytrue
                   |  yfalse
                   |  ynil
                   ;
Type               :  yident { symTable.insert(new AbstractType($1)); $$=$1;}
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
                   |  ystring  ydotdot  ystring 
                   ;
RecordType         :  yrecord  FieldListSequence  yend
                   ;
SetType            :  yset  yof  Subrange
                   ;
PointerType        :  ycaret  yident 
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
ProcedureCall      :  yident
                   |  yident ActualParameters
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
ForStatement       :  yfor  yident yassign  Expression  WhichWay  Expression
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
Designator         :  yident DesignatorStuff
                   ;
DesignatorStuff    :  /*** empty ***/
                   |  DesignatorStuff  theDesignatorStuff
                   ;
theDesignatorStuff :  ydot yident
                   |  yleftbracket ExpList yrightbracket 
                   |  ycaret 
                   ;
ActualParameters   :  yleftparen  ExpList  yrightparen
                   ;
ExpList            :  Expression   
                   |  ExpList  ycomma  Expression       
                   ;
MemoryStatement    :  ynew  yleftparen  yident yrightparen
                   |  ydispose yleftparen  yident yrightparen
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
FunctionCall       :  yident ActualParameters
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
FunctionDecl       :  FunctionHeading  ycolon  yident ysemicolon  Block
                   ;
ProcedureHeading   :  yprocedure  yident
                   |  yprocedure  yident FormalParameters
                   ;
FunctionHeading    :  yfunction  yident 
                   |  yfunction  yident FormalParameters
                   ;
FormalParameters   :  yleftparen FormalParamList yrightparen 
                   ;
FormalParamList    :  OneFormalParam 
                   |  FormalParamList ysemicolon OneFormalParam
                   ;
OneFormalParam     :  yvar  IdentList  ycolon  yident
                   |  IdentList  ycolon  yident
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
   /*fprintf(stderr, "%s\n", s);*/
}
