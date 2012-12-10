%{
/*
 * Phase 4: Code Generation
 * CSS 548; Autumn 2012
 * Aaron Hoffer and Daniel Lewis
 *
 * This is a Yacc/Bison input file. Yacc generates a parser in C code from this
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

//
// Method Declarations
//
void yyerror(char const *);
int yylex();    //Needed by g++

%}

//
// Yacc Definitions
//

//Tell bison to expect 1 shift/reduce conflict.
%expect 1

//Tell bison to use the Yacc name prefix for generated files.
%file-prefix = "y"

%start  CompilationUnit

%token  yand yarray yassign ybegin ycaret ycase ycolon ycomma yconst ydispose
        ydiv ydivide ydo ydot ydotdot ydownto yelse yend yequal yfor yfunction
        ygreater ygreaterequal yif yin yleftbracket yleftparen yless ylessequal
        ymod ymultiply ynew ynil ynot ynotequal yof yor yprocedure yprogram
        yrecord yrepeat yrightbracket yrightparen ysemicolon yset ythen yto
        ytype yunknown yuntil yvar ywhile

//This token is not used by the lexer or parser. It is used as a symbolic 
//constant by the type checking routines.        
%token yboolean;

//Some tokens have lexemes that must be captured. These tokens are declared to
//use the str field of the union.
%token <str> yident yinteger yreal ystring

//Some token values are be captured.
%token <tkn> yplus yminus

//Typed non-terminals. These non-terminals pass their value back to the rule
//from which they were included.
%type <term> ConstFactor ConstExpression
%type <chr> UnaryOperator
%type <tkn> WhichWay MultOperator AddOperator Relation
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
                        //Include standard C++ headers.
                        cout << "#include <iostream>" << endl;
                        cout << "#include <stdlib.h>" << endl;
                        cout << "using namespace std;" << endl;
                        
                        //Include C++ code used to translate sets.
                        cout << "#include \"IntSet.cpp\"" << endl;
                        
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
                        //A "begin" at the global scope indicates the start
                        //of main().
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
                            cout << "}" << endl;
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
                        c->insert();              
                        c->generateDefinition($1);
                        cout << ";" << nlindent();
                        free($1);
                        delete $3;
                    }
                    ;
TypeDef             : yident yequal NPType
                    {
                        AbstractType *td = new AbstractType($1, currType);
                        td->insert();
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
                        declareVariable();
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
                    | yinteger
                    {
                        $$ = newTerminal($1, yinteger);
                        free($1);
                    }
                    | yreal
                    {
                        $$ = newTerminal($1, yreal);
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
                        Terminal low = initTerminal($1, ystring);
                        Terminal high = initTerminal($3, ystring);
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
                        //is a valid type.
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
                    {
                        tracker.event_Assignment();
                    }
                    ;
ProcedureCall       : yident
                    {
                        procedureCallNoParam($1);
                        free($1);
                    }
                    | yident 
                    {
                        procedureCallStart($1);
                        free($1);
                    }
                      ActualParameters
                    {
                        procedureCallEnd();
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
                        cout << "}" << nlindent();
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
                        cout << "}" << nlindent();
                    }
                    ;
CaseStatement       : ycase
                    {
                        cout << "switch (";
                    }
                      Expression yof
                    {
                        indent++;
                        cout << ") {" << nlindent();
                    }
                      CaseList yend
                    {
                        indent--;
                        cout << "}" << nlindent();
                    }
                    ;
CaseList            : Case
                    | CaseList ysemicolon Case
                    ;
Case                : CaseLabelList ycolon
                    {
                        indent++;
                        cout << "{" << nlindent();
                    }
                      Statement
                    {
                        indent--;
                        cout << "break;" << nlindent();
                        cout << "}" << nlindent();
                    }
                    ;
CaseLabelList       : ConstExpression
                    {
                        if ($1->token != yinteger && $1->token != yident) {
                            cout << "***ERROR: Invalid constant value in case ";
                            cout << "statement" << endl;
                        }
                        cout << "case " << $1->str << ":" << nlindent();
                    }
                    | CaseLabelList ycomma ConstExpression
                    {
                        if ($3->token != yinteger && $3->token != yident) {
                            cout << "***ERROR: Invalid constant value in case ";
                            cout << "statement" << endl;
                        }
                        cout << "case " << $3->str << ":" << nlindent();
                    }
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
                        cout << "}" << nlindent();
                    }
                    ;
RepeatStatement     : yrepeat
                    {
                        indent++;
                        cout << "do {" << nlindent();
                    }
                      StatementSequence yuntil
                    {
                        cout << "} while (!(";
                    }
                      Expression
                    {
                        indent--;
                        cout << "));" << nlindent();
                    }
                    ;
ForStatement        : yfor yident yassign
                    {
                        tracker.push($2);                       
                        cout << "for (" << $2 << " = ";
                    }
                      Expression WhichWay
                    {
                        tracker.event_Assignment();
                        string comparator = ($6 == yto) ? " <=" : " >=";
                        cout << "; " << $2 << comparator << " ";
                    }
                      Expression ydo
                    {
                        indent++;
                        cout << "; " << $2;

                        //Special handling for character indices: increment
                        //the string character.
                        if (isStringType())  
                            cout << "[0]";
    
                        string postfix = ($6 == yto) ? "++" : "--";
                        cout << postfix << ") {" << nlindent();
                        
                        tracker.pop();
                    }
                      Statement
                    {
                        indent--;
                        cout << "}" << nlindent();
                        free($2);
                    }
                    ;
WhichWay            : yto       { $$ = yto;     }
                    | ydownto   { $$ = ydownto; }
                    ;
MemoryStatement     : ynew yleftparen yident yrightparen  
                    {
                        generateNew($3);
                        free($3);
                    }
                    | ydispose yleftparen yident yrightparen
                    {
                        generateDelete($3);
                        free($3);
                    }
                    ;
                   
/***************************  Designator Stuff  ******************************/

Designator          : yident 
                    {
                        designatorBegin($1);
                        free($1);
                    }
                      DesignatorStuff
                    ;
DesignatorStuff     : /*** empty ***/
                    | DesignatorStuff theDesignatorStuff
                    ;
theDesignatorStuff  : ydot yident
                    {                 
                        tracker.event_AccessRecordField($2);
                        
                        cout << "." << $2;
                        free($2);
                    } 
                    | yleftbracket 
                    {
                        //Open the array. It is closed in ExpList.
                        cout << "[";
                        
                        //Reset the array dimension index
                        exprCount.push_front(0);
                    }
                      ExpList yrightbracket
                    | ycaret
                    {
                        //Notify the tracker of the pointer dereference
                          tracker.event_Deref();
                        
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
                        exprCount.pop_front();
                    }
                    ;
ExpList             : ExpAction
                    | ExpList ycomma
                    {
                        //Special cases for arrays and I/O functions, which
                        //do not separate expressions with commas.
                        if (tracker.arrayOnTopOfStack())
                            cout << "[";
                        else if (currIoFunc)
                            currIoFunc->generateSep();
                        else
                            cout << ", "; //comma separated list
                    }
                      ExpAction
                    ;
ExpAction           : /*** empty ***/
                    | Expression 
                    {
                        expressionAction();
                    }
                    ;                    

/***************************  Expression Stuff  ******************************/
Expression          : SimpleExpression
                    | SimpleExpression Relation
                    {
                        printRelation($2);
                    }
                      SimpleExpression
                    {
                        //Do type-checking for the relational operator and push
                        //the resulting type (a boolean) onto the type stack.
                        tracker.event_RelationalOp($2);
                    }
                    ;
SimpleExpression    : TermExpr
                    | UnaryOperator 
                    {
                        cout << $1;
                    }
                      TermExpr
                    ;
TermExpr            : Term
                    | TermExpr AddOperator 
                    {
                        printAddOperator($2);
                    }
                      Term
                    {
                        //Do type-checking for the math operator and push the
                        //resulting type onto the type stack.
                        tracker.event_MathOp($2);
                    }
                    ;
Term                : Factor
                    | Term MultOperator
                    {
                        printMultOperator($2);
                    }
                      Factor
                    {
                        //Do type-checking for the math operator and push the
                        //resulting type onto the type stack.
                        tracker.event_MathOp($2);
                    }
                    ;
Factor              : yinteger
                    {
                        //Push the type onto the tracker
                        tracker.push($1, symTable.lookupSIT(yinteger));
                       
                       cout << $1;
                    }
                    | yreal
                    {                       
                        //Push the type onto the tracker
                        tracker.push($1, symTable.lookupSIT(yreal));
                        
                        cout << $1;
                    }
                    | ynil
                    {
                        //Push the type onto the tracker. This is a bit bogus,
                        //since nil is not a type, and (being a keyword), it
                        //does not belong in the symbol table.
                        BaseType *type = symTable.lookupSIT(ynil);
                        tracker.push("", type);
                        
                        //Print "NULL"
                        type->generateCode("");
                    }
                    | ystring
                    {
                        //Push the type onto the tracker
                        tracker.push($1, symTable.lookupSIT(ystring));

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
                        //TODO: Check that Factor is a boolean
                        cout << "!";
                    }
                      Factor
                    | Setvalue
                    | FunctionCall
                    ;
FunctionCall        : yident
                    {
                        functionCallStart($1);
                        free($1);
                    }
                      ActualParameters
                    ;
Setvalue            : yleftbracket
                    {
                        if (!tracker.peekType()->isSet()) {
                            cout << "***ERROR: Assigning set value ";
                            cout << "to non-set" << endl;
                        }

                        //Generate code that will create a set literal.
                        cout << "IntSet::makeLiteral(";
                    }
                      ElementList yrightbracket
                    {
                        cout << ", SETTERM)";
                        tracker.push("SETLIT", tracker.peekType());
                    }
                    | yleftbracket yrightbracket
                    {
                        if (!tracker.peekType()->isSet()) {
                            cout << "***ERROR: Assigning set value ";
                            cout << "to non-set" << endl;
                        }

                        //Generate code that will create an empty set.
                        cout << "IntSet::makeLiteral(SETTERM)";
                        
                        tracker.push("SETLIT", tracker.peekType());
                    }
                    ;
ElementList         : Element
                    | ElementList ycomma
                    {
                        cout << ", ";
                    }
                      Element
                    ;
Element             : ConstExpression
                    {
                        setLiteralAddValue($1);
                        delete $1;
                    }
                    | ConstExpression ydotdot ConstExpression
                    {
                        setLiteralAddRange($1, $3);
                        delete $1;
                        delete $3;
                    }
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
                        endBlock();
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
                    	endBlock();
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
MultOperator        : ymultiply    { $$ = ymultiply; }
                    | ydivide      { $$ = ydivide;   }
                    | ydiv         { $$ = ydiv;      }
                    | ymod         { $$ = ymod;      }
                    | yand         { $$ = yand;      }
                    ;
AddOperator         : yplus     { $$ = yplus;  }
                    | yminus    { $$ = yminus; } 
                    | yor       { $$ = yor;    }
                    ;
Relation            : yequal        { $$ = yequal;        }
                    | ynotequal     { $$ = ynotequal;     }
                    | yless         { $$ = yless;         }
                    | ygreater      { $$ = ygreater;      }
                    | ylessequal    { $$ = ylessequal;    }
                    | ygreaterequal { $$ = ygreaterequal; }
                    | yin           { $$ = yin;           }
                    ;

%%

/* program section */
void yyerror(const char *s)
{
    fprintf(stderr, "%s\n", s);
}
