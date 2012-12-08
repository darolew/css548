#include <typeinfo>
#include "Tracker.h"
#include "Parameter.h"
#include "actions.h"
#include "y.tab.h"

void Tracker::push(string ident)
{
    //This method is processing varibles, paramters, const, booleans,
    //and function/procedure calls. 
    //Boolean are the only odd ball. They get in here because "true"
    //and "false" are scanned as yident
    
    //Look up the yident in the symbol table.
    Symbol *sym = symTable.lookup(ident);
    if (sym) {
        sym->push();
    }
    else 
        ERR(string("undefined identifier ") + ident);
}

//----------------------------------------------------------------------------
//Deference a pointer type
void Tracker::event_Deref()
{    
    Frame f = pop();
    PointerType *type = dynamic_cast<PointerType*>(f.type);
    if (!type)
       ERR("expected a pointer type");
    
    //Get the pointee type. Not pretty.
    //Example 1 - pointer to a base type
    //
    //   PointerType->BaseType => BaseType
    //
    //Example 2- pointer to a typedef of a typedef of a base type
    //
    //    PointerType->TypeDef->TypeDef->BaseType => still BaseType
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
debugPrint();

    //TODO: In ArrayType, store array bounds and indexes as ints, not strings.
   
    //An integer must be the top of the stack because integers are the only 
    //valid index types. Pop the stack and validate integer type.
    Frame f = pop();
    BaseType *type = dynamic_cast<BaseType *>(f.type);
    
    //Vaidate that an integer is on top of the stack.
    if (!(type && type->isLegalArrayIndexType()))
      ERR(string("expected integer for acessesing array - found ") 
        + f.type->dump());
  
    //Top the stack must now be an array.
    if(!arrayInContext())
        ERR(string("expected ArrayType, but found ") 
            + peek().type->className());    
        
    //Get the bound offset for the C translation
    ArrayType *array = dynamic_cast<ArrayType *>(peek().type);
    return array->offsetForDim(dim);
}

//----------------------------------------------------------------------------
Frame Tracker::peek()
{
    if (typeStack.empty()) {
        ERR("invalid type stack access");
        exit(-1);
    }
    
    return typeStack.front();
}

//----------------------------------------------------------------------------
void Tracker::debugPrint(string msg) {

    cout << "\n--" << msg << "-------- TRACKER (top) ----------\n";
    list<Frame>::iterator it = typeStack.begin();
    for(; it != typeStack.end(); ++it) {
        if (!it->type)
            ERR("fatal err - encountered null on type stack");
            
        cout <<  it->str << "\t" << it->type->dump() << endl;
    }
    cout << "------------- (bottom)-------------\n";
}

//----------------------------------------------------------------------------
Frame Tracker::pop() 
{   
    Frame f = peek();
    typeStack.pop_front();
    //debugPrint("pop");
        
    return f;    
}

//----------------------------------------------------------------------------
bool Tracker::arrayInContext() 
{
    //TODO: This code is almost identify toe ifFunctionInContext().
    //Find a way to consolidate duplicated code.
    
    //Examine the top of the stack.
    Frame f = peek();
    ArrayType *array = dynamic_cast<ArrayType *>(f.type);
  
    //The cast fails if the object is not an ArrayType, returning NULL.
    return array != NULL;
}

//----------------------------------------------------------------------------
bool Tracker::functionInContext() 
{
    //Examine the top of the stack.
    Frame f = peek();
    Function *func = dynamic_cast<Function *>(f.type);
   
    //The cast fails if the object is not the correct type, returning NULL.
    return func != NULL;
}

//----------------------------------------------------------------------------
void Tracker::push(string description, AbstractType *type)
{
    if (!type)
        ERR(string("attempted to push null onto type stack (") 
            + "id=" + description + ") ");

    Frame f;
    f.str = description;
    f.type = type;
    push(f);
}

//----------------------------------------------------------------------------
void Tracker::push(Frame f) 
{
    typeStack.push_front(f);
    //debugPrint("push");
}

//----------------------------------------------------------------------------
//This method is called after the parser has parsed an expression for an actual
//parameter. The integer index is the zero-based index of which parameter was
//parsed. The left-most parameter is index 0;
void Tracker::endParameter(int index)
{
    //The top of the stack should be the actual parameter of a function call.
    //Verify that the actual parameter type matches the formal parameter type.
    Frame f = pop();
    AbstractType *actualParamType = f.type;
    
    //The function itself should now be on the top of the stack.
<<<<<<< HEAD
    if (!functionInContext())
=======
    if(!functionInContext())
>>>>>>> b7947c63ad305a612c85630e78af2be7b2b1a97f
        ERR("expected function to  be in context");

    Function *func = dynamic_cast<Function *>(peek().type);

    //TODO: Validate bounds on the collection of parameters
    //so we don't try and access a parameter does not exist
    Parameter *formalParam = func->getParam(index);
    AbstractType *formalParamType = formalParam->type->getType();
    if (formalParamType->compatible(actualParamType)) {
        //The parameter types match. 
        //Pop the actual param type off the stack
        pop();
<<<<<<< HEAD
    } else {
=======
    }
    else {
>>>>>>> b7947c63ad305a612c85630e78af2be7b2b1a97f
        ERR(string("expected formal param of type ") 
            + formalParamType->className() 
            + " but actual param is of type " 
            + actualParamType->className());

        //If this is the last parameter in the function call, 
        //pop the function off the stack
        if (index == func->numParams()-1)
            pop();

<<<<<<< HEAD
        //If the function had a return type (i.e. it was not a procedure), 
        //push the return type onto the stack.
        if (func->isFunction())
            push("function return ", func->returnType->getType());
=======
    //If the function had a return type (i.e. it was not a procedure), 
    //push the return type onto the stack.
    if (func->isFunction())
        push("function return ", func->returnType->getType());
>>>>>>> b7947c63ad305a612c85630e78af2be7b2b1a97f
   }
}

//----------------------------------------------------------------------------
//If we have accessed the last dimension of the current array, pop the array
//type off the stack and replace it with the type of whatever is stored
//in the array (REMEMBER that dim is zero-based)
void Tracker::endArrayDimension(int dim)
{
    Frame f = peek();
    ArrayType *array = dynamic_cast<ArrayType *>(f.type);

    if (dim == array->numDimensions()-1) {
        Frame f = pop();
        f.type = f.type->type->getType();
        push(f);
    }
}

//----------------------------------------------------------------------------
void Tracker::event_FunctionCall()
{
    //A function should now be on the top of the stack. Validate that rule.
    if(!functionInContext())
        ERR(peek().str + " is not the name of a function");
 }

 //----------------------------------------------------------------------------
 void Tracker::event_AccessRecordField(string ident)
 {
     //If identifier is not in the symbol table, it might be a field
     //in a record.
    RecordType *rec = dynamic_cast<RecordType*>(peek().type);
    if(rec) {       
        //Yes, a record type is on the top of the stack
        Variable *var = rec->lookup(ident);
        if (var) {
            //Yes, the yident is actually a field in the record. 
            
            //REMOVE RECORD TYPE ON THE TOP OF THE STACK. The record is no 
            //longer the current type. The field of the record is now the 
            //the curernt type
            pop();

            //Push it.
            var->push();   
        } else
            ERR(string("unknown field named ") + ident + " of " + rec->dump());
    } else 
        ERR(string("cannot access fields of non-record type ") 
            + peek().type->dump());
}

//----------------------------------------------------------------------------
void Tracker::event_RelationalOp()
{
    //Permitted comparisons
    //   integers and reals can be compared
    //   chars and chars
    //   booleans and booleans
    //   pointers and pointers
    //   pointers and nil
    
    //After a reltional operations, the left and right operands are popped 
    //and the results, a boolean, is pushed.
    Frame right = pop();
    Frame left = pop();
            
    //Validate that the comparison was valid
    if(! left.type->relationCompatible(right.type) )
        ERR(string("invalid relational operation ") + left.type->dump() 
            + " cannot be comparable to " + right.type->dump());
}

//----------------------------------------------------------------------------
void Tracker::event_MathOp(int opToken)
{
    //A (binary) math operation has just ocurred. Remove the operands and 
    //check their types.
    
    Frame right = pop();
    Frame left = pop();
    
    BaseType * r = dynamic_cast<BaseType *>(right.type);
    BaseType * l = dynamic_cast<BaseType *>(left.type);
    
    //yand is a MultOperator, but I think it should be a boolean operator.
    //Pacal defines four boolean operators: and, or, not, xor.
    if (opToken == yand) {
        //TODO: This will be a lot easiser when the scanner is using
        //yboolean tokens. Until then, make all yand operations valid.
        return;    
    }
    else {
        BaseType * result = BaseType::getMathType(l, r, opToken);
        if (!result) 
            ERR(string("invalid math operation - ")
                + l->dump() 
                + " is not compatible with "
                + r->dump());
    }
}
