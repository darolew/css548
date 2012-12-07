// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This file contains the method definitions of the Const class

#include "Const.h"
#include "main.h"
#include "Range.h"
#include "BaseType.h"
#include "y.tab.h"
#include "actions.h"

//The name of the constant and the value of the constant are passed to the
//constructor.
Const::Const(string id, Terminal term) : Symbol(id)
{
    this->term = term;    
}

void Const::generateDefinition(string ident)
{
    switch (term.token) {
    case yinteger:
        cout << "const int";
        break;
    case yreal:
        cout << "const double";
        break;

        //yboolean is not scanned or parsed. It is used to simplify type checking
    case yboolean:
        cout << "const bool";
        break;
    case ynil:
        cout << "ERROR: unsupported use of nil in constant" << endl;
        return;
    default:
        cout << "ERROR: internal error: invalid token " << term.token << endl;
        return;
    }
    
    cout << " " << identifier << " = ";
    if(term.unaryOp)
        cout << term.unaryOp;
    cout << term.str;
}

//Push the type represented by this constant onto the type stack
void Const::push() 
{
    //TODO: Consider constants point to a BaseType of string, int, real or boolean.
    //This would simplify things.

    //Constants can only be base types. Look up the type in the SIT    
    BaseType *type = dynamic_cast<BaseType*>(symTable.lookupSIT(term.token));
    
    tracker.push(identifier, type);
}
