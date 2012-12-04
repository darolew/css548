#include "Tracker.h"
#include "Parameter.h"
#include "actions.h"


//A new identifier has been encoutered. It is either:
//  -Defined in the symbol table (Designator: yident)
//  -A field of a record (theDesignatorStuff: ydot yident)
void Tracker::push(string ident)
{
    //Look up the yident in the symbol table.
    Symbol *sym = symTable.lookup(ident);
    
    //If identifier is in symbol table, verify that the sym is a variable.
    //dynamic_cast is always successful when we cast a class to one of its 
    //base classes.  dynamic_cast performs a special checking during runtime
    //to ensure that the expression yields a valid complete object 
    if(sym) {
        Variable * var = dynamic_cast<Variable *>(sym);
        if (!var) {
            var = dynamic_cast<Parameter *>(sym);
            if (!var)
                cout << "***ERROR: expected variable or parameter. " << __FILE__ << " " << __LINE__ << endl;
        }
    }
    
    //If identifier is not in the symbol table, it might be a field in a record.
    if (!sym) {
        RecordType *rec = dynamic_cast<RecordType *>(peek());
            if(!rec)
                cout << "***ERROR: " << ident << " is not in symbol table and record is not on top of stack " << __FILE__ << " " << __LINE__ << endl;
            else {
                Variable * v = rec->lookup(ident);
                if (!v)
                    cout << "***ERROR: " << ident << " a member of " << v->identifier << " " << __FILE__ << " " << __LINE__ << endl;
            }//end else
    }//end if
}//end method

void Tracker::deref()
{

}
void Tracker::binaryOp(int token)
{

}
void Tracker::startArrayAccess()
{

}
void Tracker::endArrayAccess()
{

}
string Tracker::arrayIndexOffset(int dim)
{

}
void Tracker::endFunctionCall()
{

}

Symbol * Tracker::peek()
{
    if (typeStack.empty()) {
        cout << "***ERROR: invalid type stack access " << __FILE__ << " " << __LINE__ << endl;
        exit(-1);
    }
    
    return typeStack.front();
}//end method
