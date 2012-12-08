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

//
//TODO: This does not work for nested array accesses. For example:
//          a[i, b[ii, jj, kk], j]
//
// This also does not work for
//  1. nested function calls
//  2. any array access inside a function call
//  3. any function call inside an array access.
//
// The original implementation of tracker includes a stack that stored
// the number of dimension of each array it encountered. The parser signalled
// the tracker every time a dimension was the array was accessed. These numbers
// would be tracked together in one stack frame. When the
// number of acesses == the number of dimension on the the stack, 
// the couting stack is popped, the array type is popped off the type stack, 
// and type of the array was pushed
//
// We would have to do the same thing for the number of parameters in a function.
int exprCount;

%}

/* Yacc definition section */

//Tell bison to expect 1 shift/reduce conflict.
%expect 1

//Tell bison to use the Yacc name prefix for generated files.
%file-prefix = "y"

%start  CompilationUnit

%token  yand yarray yassign ybegin ycaret ycase ycolon ycomma yconst ydispose
        ydiv ydivide ydo ydot ydotdot ydownto yelse yend yequal yfor
        yfunction ygreater ygreaterequal yif yin yleftbracket yleftparen yless
        ylessequal ymod ymultiply ynew ynil ynot ynotequal yof yor yprocedure
        yprogram yrecord yrepeat yrightbracket yrightparen ysemicolon yset
        ythen yto ytype yunknown yuntil yvar ywhile

//This token is not used by the lexer or parser. It is used as a symbolic 
//constant by the type checking routines.        
%token yboolean;

//Some tokens have lexemes that must be captured.
//These tokens are declared to use the str field of the union.
%token <str> yident yinteger yreal ystring

//Some token values are be captured.
%token <tkn> yplus yminus

//Typed non-terminals. These non-terminals pass their value back to the rule
//from which they were included.
%type <term> ConstFactor ConstExpression
%type <chr> UnaryOperator
%type <tkn> WhichWay MultOperator AddOperator
%type <flag> FormalParamFlag
%type <type> Term Factor FunctionCall 

//The union is used for two reasons. The first is to capture information about
//lexemes from the scanner. The second is to define the data captured in parser
//rules.
%union {
    char *str;
    struct Terminal *term;
    int chr;
    int tkn;
    bool flag;
    struct {
        int complex;
        int base;
    } type;
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
                        //Variables must point to a known type. They can be 
                        //resolved immediately.
                        currType->resolve();
                    
                        //Walk the list of variable names being declared. For
                        //example, the declaration "a,b,c : interger;" includes
                        //a list of variables {a, b, c} and their type, integer.
                        //For each one, a new variable object is created, 
                        //assigned a type, and entered into the symbol table. 
                        //The list is emptied as the variables are inserted into
                        //the symbol table.
                        while (!idList.empty()) {
                            string name = idList.front();
                            Variable *var = new Variable(name, currType);
                            var->generateDefinition(name);
                            var->insert();
                            idList.pop_front();
                            cout << ";" << nlindent();
                        }
                    }
                    ;

/***************************  Const/Type Stuff  ******************************/

ConstExpression     : UnaryOperator ConstFactor
                    {
                        //These are used for case statements, sets, and the 
                        //definition of const values.
                   
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
                            cout << "***ERROR: " << $1 << " is not a function (A)" << endl;
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
                            cout << "***ERROR: " << $1 << " is not a function (B)" << endl;
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
CaseStatement       : ycase
                    {
                        cout << "switch (";
                    }
                      Expression yof
                    {
                        indent++;
                        cout << ") { " << nlindent();
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
                        if ($1->token != yinteger && $1->token != yident)
                            cout << "***ERROR: Invalid constant value in case statement\n";
                        cout << "case " << $1->str << ":" << nlindent();
                    }
                    | CaseLabelList ycomma ConstExpression
                    {
                        if ($3->token != yinteger && $3->token != yident)
                            cout << "***ERROR: Invalid constant value in case statement\n";
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
                        $$ = yto; //TODO: Is the assignemnt $$ = yto redundant?
                    }
                    | ydownto
                    {
                        $$ = ydownto;
                    }
                    ;
MemoryStatement     : ynew yleftparen yident yrightparen  
                    {
                        //TODO: Make sure ident exists and is pointer.
                        Variable *var = (Variable*) symTable.lookup($3);
                        var->generateNewStatement();
                    }
                    | ydispose yleftparen yident yrightparen
                    {
                        cout << "delete " << $3;
                    }
                    ;
                   
/***************************  Designator Stuff  ******************************/

Designator          : yident 
                    {
                        cout << $1;

                        //Update the type stack
                        tracker.push($1);

                        //Notify the object that is was just used as a 
                        //designator.
                        Symbol *sym = symTable.lookup($1);
                        if (sym)
                            sym->event_Designator($1);
                    }
                      DesignatorStuff
                    ;
DesignatorStuff     : /*** empty ***/
                    | DesignatorStuff theDesignatorStuff
                    ;
theDesignatorStuff  : ydot yident
                    {                 
                        //ACCESS FIELD IN A RECORD
                        tracker.event_AccessRecordField($2);
                        
                        cout << "." << $2;
                        free($2);
                    } 
                    | yleftbracket 
                    {
                        //ARRAY ACCESS
                        cout << "[";
                        exprCount = 0; //Reset the array dimension index
                    }
                      ExpList yrightbracket
                    {
                        //This is now printed in expression/exp list
                        //cout << "]";
                    }
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
                    }
                    ;
ExpList             : ExpAction
                    | ExpList ycomma
                    {    
                        if (currIoFunc)
                            currIoFunc->generateSep();
                        else
                            cout << ", "; //comma separated list
                    }
                      ExpAction
                    ;
ExpAction           : /*** empty ***/
                    | Expression 
                    {
                        //This non-terminal exists to catch anytime an 
                        //expression is parser. It prevents duplicate 
                        //code in the two production of ExpList 
                         if (tracker.arrayInContext()) {
                            //Print offset 
                            cout << tracker.arrayIndexOffset(exprCount);
                            tracker.endArrayDimension(exprCount);

                            //Increment the expression count
                            exprCount++;
                            
                            //Close array access
                            cout << "]";
                        }
                        
                        if (tracker.functionInContext()) {
                            //Inform the tracker that an expression has been parsed
                            tracker.endParameter(exprCount);

                            //Increment the expression count
                            exprCount++;
                        }
                    }
                    ;                    

/***************************  Expression Stuff  ******************************/
Expression          : SimpleExpression
                    | SimpleExpression Relation SimpleExpression
                    {
                        //Validate that both expression are booleans and tell
                        //the tracker to update its state
                        tracker.event_RelationalOp();
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
                    | TermExpr AddOperator Term 
                    {
                        tracker.event_MathOp($2)
                    }
                    ;
Term                : Factor
                    | Term MultOperator
                    {
                        switch ($2) {
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
                                cout << "***ERROR: internal error, unhandled MultOperator\n";
                                break;
                        }
                    }
                      Factor
                    {
                        //Type checking
                        tracker.event_MathOp($2);
                    
                        $$.complex = CT_NONE;

                           //
                           //TODO: This was commented-out without explanation.
                           //      What was the reason?
                           //
                        // switch ($2) {
                            // case yand:
                                // if ($1.base != BT_BOOLEAN || $4.base != BT_BOOLEAN)
                                    // cout << "***ERROR: && expected boolean\n";
                                // $$.base = BT_BOOLEAN;
                                // break;
                            
                            // case ymultiply:
                                // if ($1.base == BT_INTEGER && $4.base == BT_INTEGER) {
                                    // $$.base = BT_INTEGER;
                                    // break;
                                // }
 
                                // /* fall-through */
                                
                            // case ydivide:
                                // if ($1.base != BT_INTEGER && $1.base != BT_REAL) {
                                    // cout << "***ERROR: / or * expected number\n";
                                    // break;
                                // }
                                // if ($4.base != BT_INTEGER && $4.base != BT_REAL) {
                                    // cout << "***ERROR: / or * expected number\n";
                                    // break;
                                // }
                                // $$.base = BT_REAL;
                                // break;

                            // case ydiv:
                            // case ymod:
                                // if ($1.base != BT_INTEGER || $4.base != BT_INTEGER)
                                    // cout << "***ERROR: div or mod expected integer\n";
                                // $$.base = BT_INTEGER;
                                // break;
                            
                            // default:
                                // cout << "***ERROR: Internal error, unhandled MultOperator\n";
                                // break;
                        // }
                    }
                    ;
Factor              : yinteger
                    {
                        //Push the type onto the tracker
                        tracker.push($1, symTable.lookupSIT(yinteger));
                        
                        $$.complex = CT_NONE;
                        $$.base = BT_INTEGER;
                       
                       cout << $1;
                    }
                    | yreal
                    {                       
                        //Push the type onto the tracker
                        tracker.push($1, symTable.lookupSIT(yreal));
                        
                        $$.complex = BT_NONE;
                        $$.base = BT_REAL;
                        
                        cout << $1;
                    }
                    | ynil
                    {
                        //Push the type onto the tracker
                        BaseType *type = symTable.lookupSIT(ynil);
                        tracker.push("", type);

                        $$.complex = CT_POINTER;
                        $$.base = BT_NONE;
                        
                        //Print "NULL"
                        type->generateCode("");
                    }
                    | ystring
                    {
                        //Push the type onto the tracker
                        tracker.push($1, symTable.lookupSIT(ystring));
                    
                        $$.complex = CT_NONE;
                        $$.base = BT_CHARACTER;
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
                        cout << "!"; //TODO: typechecking -- must be bool
                    }
                    Factor
                    | Setvalue
                    | FunctionCall
                    ;
FunctionCall        : yident
                    {
                        //Add the function to the type tracker
                        tracker.push($1);
                         
                        //Validate that yident really was a function.
                        tracker.event_FunctionCall();

                        cout << $1;
                        free($1);
                        
                        //Reset the expression count because it is used to 
                        //determine which parameter is being parsed.
                        exprCount = 0;
                        
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
                        currFunction->endFunction();
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
                        currFunction->endFunction();
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
MultOperator        : ymultiply    { $$ = ymultiply; }
                    | ydivide     { $$ = ydivide; }
                    | ydiv         { $$ = ydiv; }
                    | ymod         { $$ = ymod; }
                    | yand        { $$ = yand; }
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
