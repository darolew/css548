#include <typeinfo>
#include "Tracker.h"
#include "Parameter.h"
#include "actions.h"

//Debug print macro
#define ERR(X) (cout << "\n***ERROR: " << X << " | " << __FILE__ << " line " << __LINE__ << endl)

//A new identifier has been encoutered. It is either:
//  -Defined in the symbol table (Designator: yident)
//  -A field of a record (theDesignatorStuff: ydot yident)
void Tracker::push(string ident)
{
    //Ident could represent:
    // 1. Instance of Variable. 
    //    An lvalue like "x". It would have been declared and entered into 
    //    the symbol table when the parser encountered "x:integer;"  
    // 2. Field of a record.
    // 3. SIT global identifier like "false".
    
    //TODO: Handle constants. Push their type onto the stack.
        
    //Look up the yident in the symbol table.
    Symbol *sym = symTable.lookup(ident);
    
    //If yident is in the symbol table, push it and exit.
    if (sym) {
        sym->push();
        return;
    }
 
    //If identifier is not in the symbol table, it might be a field in a record.
    RecordType *rec = dynamic_cast<RecordType *>(peek().type);
    if(rec) {       
        //Yes, a record type is on the top of the stack
        Variable *var = rec->lookup(ident);
        if (var) {
            //Yes, the yident is actually a field in the record. 
            
            //REMOVE RECORD TYPE ON THE TOP OF THE STACK. The record is no 
            //longer the current type. The field of the reocrd is now the 
            //the curernt type
            pop();
        
            //Push it. Push it real good.
            var->push();          
            
            //Exit method input was sucessfullly recognized.
            return;            
        }
    }
}
//----------------------------------------------------------------------------
void Tracker::deref()
{
    //Deference a pointer type
    
    //TODO: If top of stack is not a pointer, throw an error.
    frame f = pop();
    PointerType *type = dynamic_cast<PointerType *>(f.type);
    if (!type)
       ERR("expected a pointer type");
    
    //Get the pointee type. Not pretty.
    //Example 1 - pointer to a base type
    //
    //   PointerType->BaseType : BaseType
    //
    //Example 2- pointer to a typedef of a typedef of a base type
    //
    //    PointerType->TypeDef->TypeDef->BaseType : still BaseType
    //
    
    //cerr << "\n<<<<<<<< f.type " << f.type->className() << endl;
    //cerr << "\n<<<<<<<< f.type->getType() " << f.type->getType()->className() << endl;
    //cerr << "\n<<<<<<<< f.type->type->getType() " << f.type->type->getType()->className() << endl;
    f.type = f.type->type->getType();
        
    //Put pointee on the type stack
    push(f);
}
//----------------------------------------------------------------------------
void Tracker::binaryOp(int token)
{
    //TODO: this method will have the worlds longest switch statement.
    //It will switch on the token and determine if the top two types 
    //in the type stack are compatible. 
    
    //It will pop both of the top operand types off the stack and 
    //and push on the type that results from the operation.
}
//----------------------------------------------------------------------------
string Tracker::arrayIndexOffset(int dim)
{
    //TODO: In ArrayType, store array bounds and indexes as ints, not strings.

    //Pop the integer off the top of the stack because it being used to
    //index an array.
    //TODO: Assret that an integer is on the top of the stack (because
    //  only integers are valid indexes).
    pop();
    
    //Top the stack must now be an array.
    //TODO: Assert the array pointer is not void.
    ArrayType *array = dynamic_cast<ArrayType *>(peek().type);
 
    if(!array)
        ERR(string("fatal error - expected ArrayType, but found ") + peek().type->className());    

    //Get the bound offset for the C translation
    string offset = array->offsetForDim(dim);
  
    //If we have access the last dimension of the array, pop the array
    //type off the stack and replace it with the type of whatever is stored
    //in the array
    //(REMEMBER that dim is zero-based)
    if (dim == array->numDimensions()-1) {
        frame f = pop();
        
        //Example 1: 
        f.type = f.type->type->getType();
        push(f);
    }
    
    return offset;
}
//----------------------------------------------------------------------------
frame Tracker::peek()
{
    if (typeStack.empty()) {
        cout << "***ERROR: invalid type stack access " << __FILE__ << " " << __LINE__ << endl;
        exit(-1);
    }
    
    return typeStack.front();
    
}
//----------------------------------------------------------------------------
void Tracker::debugPrint(string msg) {

    cout << "\n--" << msg << "-------- TRACKER (top) ----------\n";
    list<frame>::iterator it = typeStack.begin();
    for(; it != typeStack.end(); ++it) {
        if (it->type == NULL)
            ERR("fatal err - encountered null on type stack");
            
        cout <<  it->type->className()  << "\t" << it->str << endl;
    }
    cout << "------------- (bottom)-------------\n";
}
//----------------------------------------------------------------------------
frame Tracker::pop() 
{   
    frame f = peek();
    typeStack.pop_front();
    //debugPrint("pop");
        
    return f;    
}
//----------------------------------------------------------------------------
bool Tracker::isArrayInContext() 
{
    //Examine the top of the stack.
    frame f = peek();
    ArrayType *array = dynamic_cast<ArrayType *>(f.type);
   
    //The cast will fail if the object is an an ArrayType.
    //If the cast fails, the pointer will be null   
    return !array;     
}
//----------------------------------------------------------------------------
void Tracker::push(string description, AbstractType *type)
{
    if (!type)
        ERR(string("fatal error - type is null (") + "id=" + description + ") ");

    frame f;
    f.str = description;
    f.type = type;
    push(f);
}
//----------------------------------------------------------------------------
void Tracker::push(frame f) 
{
    typeStack.push_front(f);
    //debugPrint("push");
}
