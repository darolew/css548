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
                        //If a variable is declared with                                                                          
                        //an unknown identifier, as in sterror.p                                                                                        
                        //    aaa: undefinedType;                                                                                                       
                        //currType will be NULL and cannot be resolved                                                                                  
                        if (currType)                                                                                                                   
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
                      Expression {tracker.event_Assignment();}
                    ;
ProcedureCall       : yident
                    {
                        Symbol *sym = symTable.lookup($1);
                        if (!sym || !sym->isFunction())
                            cout << "***ERROR: " << $1 << " is not a function" << endl;
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
                        //Add the function to the type tracker
                        tracker.push($1);

                        //Validate that yident really was a function.
                        //tracker.event_FunctionCall();

                        //TODO: These are partially redundant.
                        Symbol *sym = symTable.lookup($1);
                        if (!sym || !sym->isFunction())
                            cout << "***ERROR: " << $1 << " is not a function" << endl;
                        else if (sym->isIoFunction()) {
                            currIoFunc = (IoFunction*)sym;
                            currIoFunc->generateInit();
                            currIoFunc->generateSep();
                        } else
                            cout << $1;

                        free($1);
                        
                        //Reset the expression count because it is used to 
                        //determine which parameter is being parsed.
                        exprCount.push_front(0);
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
                        AbstractType *type = tracker.peekType();
                        BaseType *bt = dynamic_cast<BaseType*>(type);
                        if (bt && bt->isStringType())    
                            cout << "[0]";
    
                        string postfix = ($6 == yto) ? "++" : "--";
                        cout << postfix << ") {" << nlindent();
                        
                        tracker.pop();
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
                        Variable *var = (Variable*)symTable.lookup($3);
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

                        //If this identifier is being used as an array index
                        //and it is a string, access the first character.
                        if (tracker.arraySecondFromTop()) {
                            AbstractType *type = tracker.peekType();
                            BaseType *bt = dynamic_cast<BaseType*>(type);
                            if (bt && bt->isStringType())
                                cout << "[0]";
                        }

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
                        
                        //Reset the array dimension index
                        exprCount.push_front(0);
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
                        exprCount.pop_front();
                    }
                    ;
ExpList             : ExpAction
                    | ExpList ycomma
                    {
                        //Do not print comma
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
                        //Non-terminal ExpAction is trigged when an expression
                        //is parsed. It triggers semantic actions and 
                        //elminates duplicate code that would otherwise be
                        //in both ExpList productions
                        if (tracker.arraySecondFromTop()) {
                            //Print bounds offset 
                            int dimension = exprCount.front();
                            bool last;
                            tracker.arrayIndexOffset(dimension);                          
                            exprCount.front() += tracker.endArrayDimension(dimension, &last);
                            
                            //Close array access
                            cout << "]";

                            if(last)
                                exprCount.pop_front();

                        }

                        if (tracker.functionCallInProgress() && !currIoFunc) {
                            //Inform the tracker that an expression has been parsed
                            tracker.endParameter(exprCount.front());

                            //Increment the expression count
                            exprCount.front() += 1;
                        }
                    }
                    ;                    

/***************************  Expression Stuff  ******************************/
Expression          : SimpleExpression
                    | SimpleExpression Relation
                    {
                        switch ($2) {
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
                        }
                    }
                      SimpleExpression
                    {
                        //Validate that both expression are booleans and tell
                        //the tracker to update its state
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
                        switch ($2) {
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
                                cout 
                                    << "***ERROR: internal error, unhandled AddOperator "
                                    << $2
                                    << endl;
                                break;
                        }
                    }
                      Term
                    {
                        tracker.event_MathOp($2);
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
                        //Push the type onto the tracker
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
                        
                        AbstractType *type = tracker.peekType();
                        if (!type->isFunction())
                            cout << "***ERROR: expected " << $1 << " to be function\n";
                        
                        //Validate that yident really was a function.
                        //tracker.event_FunctionCall();

                        cout << $1 << flush;
                        free($1);
                        
                        //Reset the expression count because it is used to 
                        //determine which parameter is being parsed.
                        exprCount.push_front(0);
                    }
                      ActualParameters
                    ;
Setvalue            : yleftbracket
                    {
                        if (!tracker.peekType()->isSet())
                            cout << "***ERROR: Assigning set value to non-set" << endl;

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
                        if (!tracker.peekType()->isSet())
                            cout << "***ERROR: Assigning set value to non-set" << endl;

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
                        Terminal *term = $1;
                        
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
                        delete term;
                    }
                    | ConstExpression ydotdot ConstExpression
                    {
                        Terminal *low = $1;
                        Terminal *high = $3;
                        
                        //Literal integers are the only supported set ranges.
                        if (low->token != yinteger || high->token != yinteger)
                            cout << "***ERROR: Set ranges must be literal integers" << endl;

                        //Make sure these values are in-range for this set.
                        const int ilow = atoi(low->str.c_str());
                        const int ihigh = atoi(high->str.c_str());
                        SetType *set = (SetType*)tracker.peekType();
                        if (!set->legalValue(ilow) || !set->legalValue(ihigh))
                            cout << "***ERROR: Out-of-bounds set value" << endl;

                        //Pass the whole range of values into IntSet::makeLiteral().
                        //This will generate pretty ridiculous code for large
                        //set ranges.
                        for (int num = ilow; num <= ihigh; num++) {
                            if (num > ilow)
                                cout << ", ";
                            cout << num;
                        }
                        
                        delete low;
                        delete high;
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
                    | ydivide      { $$ = ydivide; }
                    | ydiv         { $$ = ydiv; }
                    | ymod         { $$ = ymod; }
                    | yand         { $$ = yand; }
                    ;
AddOperator         : yplus     { $$ = yplus; }
                    | yminus    { $$ = yminus; } 
                    | yor       { $$ = yor; }
                    ;
Relation            : yequal        { $$ = yequal; }
                    | ynotequal     { $$ = ynotequal; }
                    | yless         { $$ = yless; }
                    | ygreater      { $$ = ygreater; }
                    | ylessequal    { $$ = ylessequal; }
                    | ygreaterequal { $$ = ygreaterequal; }
                    | yin           { $$ = yin; }
                    ;

%%

/* program section */
void yyerror(const char *s)
{
    fprintf(stderr, "%s\n", s);
}
