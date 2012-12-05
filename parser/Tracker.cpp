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
     Variable * var;
    if(sym) {
        var = dynamic_cast<Variable *>(sym);
        if (!var) {
            var = dynamic_cast<Parameter *>(sym);
            if (!var)
                cout << "***ERROR: expected variable or parameter. " << __FILE__ << " " << __LINE__ << endl << flush;
        }
    }
    
    //If identifier is not in the symbol table, it might be a field in a record.
    if (!sym) {
        RecordType *rec = dynamic_cast<RecordType *>(peek());
            if(!rec) {
                cout << "***ERROR: " << flush;
                cout << ident << " is not in symbol table and record is not on top of stack " << __FILE__ << " " << __LINE__ << endl;
            }
            else {
                var = rec->lookup(ident);
                if (!var) {
                    cout << "***ERROR: " << flush;
                    cout << ident << " a member of " << var->identifier << " " << __FILE__ << " " << __LINE__ << endl;
                }
                //REMOVE RECORD TYPE ON THE TOP OF THE STACK
                pop();
            }//end else

    }//end if

    typeStack.push_front(var->getType());
    //cout << "----->Adding var: " << var->identifier << endl;
    
}//end method

//Deference a pointer type
void Tracker::deref()
{
    //TODO: If top of stack is not a pointer, throw an error.
    AbstractType *type = pop();
    push(type->type->getType());
}
void Tracker::binaryOp(int token)
{
    //TODO: this method will have the worlds longest switch statement.
    //It will switch on the token and determine if the top two types 
    //in the type stack are compatible. 
    
    //It will pop both of the top operand types off the stack and 
    //and push on the type that results from the operation.
    
}

string Tracker::arrayIndexOffset(int dim)
{
    //TODO: Assert that array is in context
    //TODO: Assret that an integer is on the top of the stack (because
    //  only integers are valid indexes).
    //TODO: In ArrayType, store array bounds and indexes as ints, not strings.

    //Pop the integer off the top of the stack because it being used to
    //index an array.
    pop();
    
    //Top the stack must now be an array.
    ArrayType *array = dynamic_cast<ArrayType *>(peek());
 
    //Get the bound offset for the C translation
    string offset = array->offsetForDim(dim);
  
    //If we have access the last dimension of the array, pop the array
    //type off the stack and replace it with the type of whatever is stored
    //in the array
    //(REMEMBER that dim is zero-based)
    if (dim == array->numDimensions()-1) {
        pop();
        push(array->type->getType());
    }
    
    return offset;
}

AbstractType * Tracker::peek()
{
    if (typeStack.empty()) {
        cerr << "***ERROR: invalid type stack access " << __FILE__ << " " << __LINE__ << endl;
        exit(-1);
    }
    
    return typeStack.front();
    
}//end method

#include <typeinfo>

void Tracker::debugPrint() {

    cout << "\n---------- TRACKER (top) ----------\n";
    list<AbstractType *>::iterator it = typeStack.begin();
    for(; it != typeStack.end(); ++it) {
    }
    cout << "------------- (bottom)-------------\n";
}


AbstractType *Tracker::pop() 
{   
    AbstractType *type = peek();
    typeStack.pop_front();
    
    //TODO: Handle popping a function
        
    return type;    
}

void Tracker::push(AbstractType *type)
{
    typeStack.push_front(type);
}

bool Tracker::isArrayInContext() 
{
    //Examine the top of the stack.
    ArrayType *array = dynamic_cast<ArrayType *>(peek());
   
    //The cast will fail if the object is an an ArrayType.
    //If the cast fails, the pointer will be null   
    return !array;     
}
