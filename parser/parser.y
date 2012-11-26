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
#include "Parameter.h"
#include "Function.h"
#include "Const.h"
#include "PointerType.h"
#include "RecordType.h"
#include "SetType.h"
#include "IoFunction.h"

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
//TODO: fix line lengths
%token  yand yarray yassign ybegin ycaret ycase ycolon ycomma yconst ydispose ydiv
        ydivide ydo ydot ydotdot ydownto yelse yend yequal yfor yfunction
        ygreater ygreaterequal yif yin yleftbracket yleftparen yless
        ylessequal ymod ymultiply ynew ynil ynot ynotequal yof yor yprocedure
        yprogram yrecord yrepeat yrightbracket yrightparen ysemicolon yset
        ythen yto ytype yunknown yuntil yvar ywhile

//Some tokens have lexemes that must be captured.
//These tokens are declared to use the str field of the union.
%token <str> yident ynumber ystring

//Some token values are be captured.
%token <tkn> yplus yminus

//Typed non-terminals. These non-terminals pass their value back to the rule
//from which they were included.
%type <term> ConstFactor ConstExpression
%type <chr> UnaryOperator
%type <tkn> WhichWay
%type <flag> FormalParamFlag

//The union is used for two reasons. The first is to capture information about
//lexemes from the scanner. The second is to define the data captured in parser
//rules.
%union {
    char *str;
    struct Terminal *term;
    int chr;
    int tkn;
    bool flag;
};

%%
/* rules section */

/**************************  Pascal program **********************************/

CompilationUnit     :
                    {
                        cout << "#include <iostream>" << endl;
                        cout << "#include <stdlib.h>" << endl;
                        cout << "using namespace std;" << endl;
                        cout << endl << endl;
                    }
                      ProgramModule
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
ProgramParameters   : yleftparen IdentList yrightparen
                    {
                        //We don't care about the program parameters.
                        idList.clear();
                    }
                    ;
                    ;
IdentList           :  yident
                    {
                        idList.push_back($1);
                        free($1);
                    }
                    | IdentList ycomma yident
                    {
                        idList.push_back($3);
                        free($3);
                    }
                    ;

/**************************  Declarations section ***************************/

Block               : Declarations
                    {
                        if (symTable.size() == 2) {
                            indent++;
                            cout << "int main()" << endl;
                            cout << "{" << nlindent();
                        }
                    }
                      ybegin StatementSequence yend
                    {
                        if (symTable.size() == 2) {
                            indent--;
                            cout << unindent() << "}" << endl;
                        }
                    }
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
                    {
                        cout << nlindent();
                    }
                    ;
ConstantDefList     : ConstantDef ysemicolon
                    | ConstantDefList ConstantDef ysemicolon
                    ;
TypeDefBlock        : /*** empty ***/
                    | ytype TypeDefList
                    {
                        cout << nlindent();
                    }
                    ;
TypeDefList         : TypeDef ysemicolon
                    {
                        cout << ";" << endl;
                    }
                    | TypeDefList TypeDef ysemicolon
                    {
                        cout << ";" << nlindent();
                    }
                    ;
VariableDeclBlock   : /*** empty ***/
                    | yvar VariableDeclList
                    {
                        cout << nlindent();
                    }
                    ;
VariableDeclList    : VariableDecl ysemicolon
                    | VariableDeclList VariableDecl ysemicolon
                    ;
ConstantDef         : yident yequal ConstExpression
                    {
                        Const *c = new Const($1, *$3);
                        symTable.insert(c);
                        c->generateDefinition($1);
                        cout << ";" << nlindent();
                        free($1);
                        delete $3;
                    }
                    ;
TypeDef             : yident yequal NPType
                    {
                        AbstractType *td = new AbstractType($1, currType);
                        symTable.insert(td);
                        cout << "typedef ";
                        currType->generateDefinition($1);
                        free($1);
                    }
                    | PointerTypeDef
                    ;
PointerTypeDef      : yident yequal ycaret yident
                    {
                        PointerType *ptrType = addPointerToList($1, $4);
                        cout << "typedef ";
                        ptrType->generateDefinition($1);
                        free($1);
                        free($4);
                    }
                    ;
VariableDecl        : IdentList ycolon Type
                    {
                        //Variables must point to a known type. They can be 
                        //resolved immediately.
                        currType->resolve();
                    
                        /*Walk the list of variable names being declared. For
                        example, the declaration "a,b,c : interger;" includes
                        a list of variables {a, b, c} and their type, integer.
                        For each one, a new variable object is created, 
                        assigned a type, and entered into the symbol table. 
                        The list is emptied as the variables are inserted into
                        the symbol table.*/
                        while (!idList.empty()) {
                            string name = idList.front();
                            Variable *var = new Variable(name, currType);
                            var->generateDefinition(name);
                            symTable.insert(var);
                            idList.pop_front();
                            cout << ";" << nlindent();
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
                        Terminal low, high;
                        low = initTerminal($1, ystring);
                        high = initTerminal($3, ystring);
                        addRange(&low, &high);
                        free($1);
                        free($3);
                    }
                    ;
RecordType          : yrecord FieldListSequence yend
                    {
                        currType = new RecordType(fieldList);
                        fieldList.clear();
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
                        //Create a pointer instance -- we know the name of what
                        //it points at, but we do not know if that identifier
                        //exists in the symbol table.
                        currType = new PointerType("", $2);
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
                	{
                    	cout << ";" << nlindent();
                    }
                    | ProcedureCall
                	{
                    	cout << ";" << nlindent();
                    }
                    | IfStatement
                    | CaseStatement
                    | WhileStatement
                    | RepeatStatement
                    | ForStatement
                    | MemoryStatement
                	{
                    	cout << ";" << nlindent();
                    }
                    | ybegin StatementSequence yend
                    | /*** empty ***/
                    ;
Assignment          : Designator yassign 
					{
						cout << " = "; 
					}
					Expression
                    ;
ProcedureCall       : yident
                    {
                        Symbol *sym = symTable.lookup($1);
                        if (!sym || !sym->isFunction())
                            cerr << "***ERROR: " << $1 << " is not a function" << endl;
                        else if (sym->isIoFunction()) {
                            IoFunction *iofunc = (IoFunction*)sym;
                            iofunc->generateInit();
                            iofunc->generateEnd();                                                        
                        } else {
                            currIoFunc = NULL;
                            cout << $1 << "()";
                        }
                        
                        free($1);
                    }
                    | yident 
                    {
                        Symbol *sym = symTable.lookup($1);
                        if (!sym || !sym->isFunction())
                            cerr << "***ERROR: " << $1 << " is not a function" << endl;
                        else if (sym->isIoFunction()) {
                            currIoFunc = (IoFunction*)sym;
                            currIoFunc->generateInit();
                            currIoFunc->generateSep();
                        } else
                            cout << $1;

                        free($1);
                    }
                      ActualParameters
                    {
                        if (currIoFunc)
                            currIoFunc->generateEnd();
                            
                        currIoFunc = NULL;
                    }
                    ;
IfStatement         : yif
                    {
                        cout << "if (";
                    }
                      Expression
                    {
                        indent++;
                        cout << ") {" << nlindent();
                    }
                      ythen Statement
                    {
                        indent--;
                        cout << unindent() << "} " << nlindent();
                    }
                      ElsePart
                    ;
ElsePart            : /*** empty ***/
                    | yelse
                    {
                        indent++;
                        cout << "else {" << nlindent();
                    }
                      Statement
                    {
                        indent--;
                        cout << unindent() << "} " << nlindent();
                    }
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
WhileStatement      : ywhile
                    {
                        cout << "while (";
                    }
                      Expression ydo
                    {
                        indent++;
                        cout << ") {" << nlindent();
                    }
                      Statement
                    {
                        indent--;
                        cout << unindent() << "}" << nlindent();
                    }
                    ;
RepeatStatement     : yrepeat
                    {
                        indent++;
                        cout << "do {" << nlindent();
                    }
                      StatementSequence yuntil
                    {
                        cout << unindent() << "} while (";
                    }
                      Expression
                    {
                        indent--;
                        cout << ");" << nlindent();
                    }
                    ;
ForStatement        : yfor yident yassign
                    {
                        cout << "for (" << $2 << " = ";
                    }
                      Expression WhichWay
                    {
                        string comparator = ($6 == yto) ? " <=" : " >=";
                        cout << "; " << $2 << comparator << " ";
                    }
                      Expression ydo
                    {
                        indent++;
                        string postfix = ($6 == yto) ? "++" : "--";
                        cout << "; " << $2 << postfix << ") {" << nlindent();
                    }
                      Statement
                    {
                        indent--;
                        cout << unindent() << "}" << nlindent();
                        free($2);
                    }
                    ;
WhichWay            : yto
                    {
                        $$ = yto;
                    }
                    | ydownto
                    {
                        $$ = ydownto;
                    }
                    ;
MemoryStatement     : ynew yleftparen yident yrightparen  
                    {
                        //TODO: Make sure ident exists and is pointer.
                        //Using malloc() is easier than using new since it is
                        //not necessary to lookup the pointed-to type.
                        cout << $3 << " = malloc(sizeof(*" << $3 << "))";
                    }
                    | ydispose yleftparen yident yrightparen
                    {
                        cout << "free(" << $3 << ")";
                    }
                    ;
                   
/***************************  Designator Stuff  ******************************/

Designator          : yident 
					{
						cout << $1;
					}
					  DesignatorStuff
                    ;
DesignatorStuff     : /*** empty ***/
                    | DesignatorStuff theDesignatorStuff
                    ;
theDesignatorStuff  : ydot yident /*Record field access*/
                    {
                        cout << "." << $2;
                        free($2);
                    }
                    | yleftbracket 
                    {
                    	cout << "[";
                    }
                      ExpList yrightbracket /*Array element access*/
                    {
                    	cout << "]";
                    }
                    | ycaret
                    {
                        //In Pascal, the pointer deference is on the right
                        //side. In C, using "*" to deference would have to
                        //go on the left side; to make translation easier,
                        //dereference with "[0]" which, like Pascal, goes
                        //on the right. Though this is bad style, since
                        //the pointer is not an array, it is correct.
                        cout << "[0]";
                    }
                    ;
ActualParameters    : yleftparen 
					{
					    if (!currIoFunc)
    						cout << "(";
					}
					  ExpList yrightparen
					{
						if (!currIoFunc)
    						cout << ")";
					}
                    ;
ExpList             : Expression
                    | ExpList ycomma
                    {
                        if (currIoFunc)
                            currIoFunc->generateSep();
                        else
                        	cout << ", ";
                    }
                      Expression
                    ;

/***************************  Expression Stuff  ******************************/
Expression          : SimpleExpression
                    | SimpleExpression Relation SimpleExpression
                    ;
SimpleExpression    : TermExpr
                    | UnaryOperator 
                    {
                    	cout << $1;
                    }
                    TermExpr
                    ;
TermExpr            : Term
                    | TermExpr AddOperator Term
                    ;
Term                : Factor
                    | Term MultOperator Factor
                    ;
Factor              : ynumber
					{
						cout << $1;
					}
                    | ynil
                    {
                    	cout << "NULL";
                    }
                    | ystring
                    {
                    	cout << "\"" << $1 << "\"";
                    }
                    | Designator
                    | yleftparen 
                    {
                    	cout << "(";
                    }
                      Expression yrightparen
                    {
                    	cout << ")";
                    }
                    | ynot 
                    {
                    	cout << "!";
                    }
                    Factor
                    | Setvalue
                    | FunctionCall
                    ;
FunctionCall        : yident
					{
						cout << $1;
						free($1);
					}
 					  ActualParameters
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
                        currFunction->generateDefinition("");
                    }
                    Block
                    {
                        cout << "\n}\n\n";
                        symTable.endScope();
                    }
                    ;
FunctionDecl        : CreateFunc FunctionHeading ycolon yident ysemicolon
                    {
                        AbstractType *returnType = symTable.lookupType($4);
                        currFunction->setReturnType(returnType);
                        currFunction->generateDefinition("");
                    }
                    Block
                    {
                        cout << "\n}\n\n";
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
                        addFormalParam($4, $1);
                        free($4);
                    }
                    ;
FormalParamFlag     : /*** nothing ***/
                    {
                        $$ = false;
                    }
                    | yvar
                    {
                        $$ = true;
                    }
                    ;

/***************************  More Operators  ********************************/

UnaryOperator       : yplus     { $$ = '+'; }
                    | yminus    { $$ = '-'; }
                    ;
MultOperator        : ymultiply
					{
						//TODO: type checking and coercion
						cout << " * ";
					} 
					| ydivide 
					{
						//TODO: type checking and coersion
						cout << " / ";
					}
					| ydiv 
					{ //With the exception of Div and Mod, which accept only integer expressions as operands,
					  //all operators accept real and integer expressions as operands. 
					  cout << " / ";
					}
					| ymod 
					{
						cout << " % ";
					}
					| yand
					{
						//TODO:Boolean operators can only have boolean type 
						//operands, and the resulting type is always boolean.
						cout << " && ";
					}
	
                    ;
AddOperator         : yplus 
					{
						cout << " + ";
					}	
					| yminus
					{
						cout << " - ";
					}
					| yor
					{
						cout << " || ";
					}
                    ;
Relation            : yequal        { cout << " == "; }
                    | ynotequal     { cout << " != "; }
                    | yless         { cout << " < ";  }
                    | ygreater      { cout << " > ";  }
                    | ylessequal    { cout << " <= "; }
                    | ygreaterequal { cout << " >= "; }
                    | yin
                    ;

%%

/* program section */
void yyerror(const char *s)
{
    fprintf(stderr, "%s\n", s);
}
