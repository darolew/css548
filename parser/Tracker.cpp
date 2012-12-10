// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This file contains the method definitions of the Tracker class.

#include <typeinfo>
#include "Tracker.h"
#include "Parameter.h"
#include "InvalidType.h"
#include "actions.h"
#include "y.tab.h"


//===== PUBLIC METHOD TO GROW THE TYPE STACK =====

//----------------------------------------------------------------------------
//Push a designator's type onto the stack.
void Tracker::push(string ident)
{
    //This method is processing varibles, paramters, const, booleans, and
    //function/procedure calls. Boolean are the only odd ball. They get in
    //here because "true" and "false" are scanned as yident
    
    //Look up the yident in the symbol table.
    Symbol *sym = symTable.lookup(ident);
    if (sym)
        sym->push();
    else 
        ERR(string("undefined identifier ") + ident);
}

//----------------------------------------------------------------------------
//Push type and its description directly onto the stack 
void Tracker::push(string description, AbstractType *type)
{
    if (!type) {
        ERR(string("attempted to push null onto type stack (") 
            + "id=" + description + ") ");
    }
    
    Frame f;
    f.str = description;
    f.type = type;
    push(f);
}

//===== PUBLIC GETTER METHODS =====

//----------------------------------------------------------------------------
//Returns the type at the top of the type stack.
AbstractType *Tracker::peekType()
{
    Frame f = peek();
    return f.type->getType();
}

//----------------------------------------------------------------------------
//Returns whether the type at the top of the type stack is an array.
bool Tracker::arrayOnTopOfStack() 
{
    if (typeStack.size() < 1)
        return false;

    return peek().type->isArrayType();
}

//----------------------------------------------------------------------------
//Returns whether the type just beneath the top of the type stack is an array.
bool Tracker::arraySecondFromTop() 
{
    if (typeStack.size() < 2)
        return false;
        
    return peek2().type->isArrayType();
}

//----------------------------------------------------------------------------
//Index into an array.
void Tracker::arrayIndexOffset(int dim)
{
    //Integer must be at top of the stack because integers are the only 
    //valid index types. Pop the stack and validate integer type.
    Frame f = pop();
    BaseType *type = dynamic_cast<BaseType*>(f.type);

    //Vaidate that an integer is on top of the stack.
    if (!type || !type->isLegalArrayIndexType()) {
        ERR(string("expected integer for acessesing array - found ") 
            + f.type->dump());
    }
    
    //Top the stack must now be an array.
    if (!peek().type->isArrayType()) {
        ERR(string("expected ArrayType, but found ") 
            + peek().type->className());    
    }
    
    //Get the bound offset for the C translation
    ArrayType *array = dynamic_cast<ArrayType*>(peek().type);
    array->offsetForDim(dim);
}

//----------------------------------------------------------------------------
//Returns whether a function call is in progress.
bool Tracker::functionCallInProgress() 
{
    if (typeStack.size() <= 1)
        return false;
        
    Frame f = peek2();
    Function *func = dynamic_cast<Function*>(f.type);
   
    //The cast fails if the object is not the correct type, returning NULL.
    return func != NULL;
}

//===== PUBLIC METHODS RESPONDING TO PARSER EVENTS =====

//----------------------------------------------------------------------------
//Event: A record field has been accessed, a la ".ident".
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
    } else {
        ERR(string("cannot access fields of non-record type ") 
            + peek().type->dump());
    }
}

//----------------------------------------------------------------------------
//Event: Assignment, a la "A = B".
void Tracker::event_Assignment() 
{
    // A = B
    //Pop B off the stack
    // Ensure A and B are assignment-compatible.
    Frame right = pop();
    Frame left = pop();
   
    if (!left.type->compatible(right.type, yassign)) {
        ERR(string("incompatible assignment of ") + right.type->dump() 
            + " to " + left.type->dump());
    }
}

//----------------------------------------------------------------------------
//Event: Deference a pointer type, a la "ident^"
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
    f.type = f.type->type->getType();
        
    //Put pointee on the type stack
    push(f);
}

//----------------------------------------------------------------------------
//If we have accessed the last dimension of the current array, pop the array
//type off the stack and replace it with the type of whatever is stored
//in the array (remember that dim is zero-based).
//Returns a delta with which to update the 'dim' value.
int Tracker::endArrayDimension(int dim, bool *last)
{
    Frame f = peek();
    ArrayType *array = dynamic_cast<ArrayType*>(f.type);
    int delta = 1;
    
    *last = false;
    
    if (dim == array->numDimensions()-1) {
        *last = true;
        Frame f = pop();
        AbstractType *typeOfArray = f.type->type->getType();
        Frame next;
        next.str = "ARRAYTYPE";
        next.type = typeOfArray;
        if (f.type->isArrayType()) {
            *last = false;
            delta = dim * -1;
        }
        push(next);
    }
    
    return delta;
}

//----------------------------------------------------------------------------
//Event: function call.
void Tracker::event_FunctionCall()
{
    //A function should now be on the top of the stack. Validate that rule.
    if (!functionCallInProgress())
        ERR(peek().str + " is not the name of a function");
 }

//----------------------------------------------------------------------------
//This method is called after the parser has parsed an expression for an actual
//parameter. The integer index is the zero-based index of which parameter was
//parsed. The left-most parameter is index 0.
void Tracker::endParameter(int index)
{
    //The top of the stack should be the actual parameter of a function call.
    //Verify that the actual parameter type matches the formal parameter type.
    Frame actualParam = pop();
    
    //Skip parameter type-checking for I/O functions like writeln.
    if (currIoFunc)
        return;

    AbstractType *actualParamType = actualParam.type;

    //Function should be on the top of the stack.
    Function *func = dynamic_cast<Function*>(peek().type);

    //TODO: Validate bounds on the collection of parameters
    //so we don't try and access a parameter does not exist
    Parameter *formalParam = func->getParam(index);
    AbstractType *formalParamType = formalParam->type->getType();
    if (!formalParamType->compatible(actualParamType, yequal)) {   
        ERR(string("expected formal param of type ") 
            + formalParamType->className() 
            + " but actual param is of type " 
            + actualParamType->className());
	}

    //If this is the last parameter in the function call, 
    //pop the function off the stack
    if (index == func->numParams()-1) {
        pop();
        
	    //If the function had a return type (i.e. it was not a procedure), 
    	//push the return type onto the stack.
	    if (func->isFunction() && !func->isProcedure())
	        push("function return ", func->returnType->getType());
    }
}

//----------------------------------------------------------------------------
//Event: A math operator, like "A + B" or "A / B".
void Tracker::event_MathOp(int opToken)
{
    //A (binary) math operation has just ocurred. Remove the operands and 
    //check their types.
    
    Frame right = pop();
    Frame left = pop();
    
    BaseType *r = dynamic_cast<BaseType*>(right.type);
    BaseType *l = dynamic_cast<BaseType*>(left.type);
    
    if (opToken == yand || opToken == yor) {
        //TODO: verify that operands are booleans
        
        //The results of a boolean operation is a boolean
        push("", symTable.lookupSIT(yboolean));
    } else if (r && l) {
        BaseType *result = BaseType::getMathType(l, r, opToken);
        if (!result) {
            ERR(string("invalid math operation - ")
                + l->dump() 
                + " is not compatible with "
                + r->dump());
        } else {
            //Push the result type onto the stack
            push("", result);
        }
    } else {
        //The operands are not base types; check if they are sets.
        SetType *rs = dynamic_cast<SetType*>(right.type);
        SetType *ls = dynamic_cast<SetType*>(left.type);
        if (rs && ls) {
            //If this is a legal operation for a set, push a set on the tpye
            //stack.
            if (rs->legalMathOp(opToken))
                push(right.str, right.type);
            else
                ERR("Illegal math operation on set");
        } else
            ERR("Illegal math operation");
    }
}

//----------------------------------------------------------------------------
//Event: Relational operator, like "<=" or "<>".
void Tracker::event_RelationalOp(int opToken)
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
    Frame left = peek();
            
    //Validate that the comparison was valid
    if (!left.type->compatible(right.type, opToken)) {
        ERR(string("invalid relational operation ") + left.type->dump() 
            + " cannot be comparable to " + right.type->dump());
    }
}

//===== OTHER PUBLIC METHODS =====

//----------------------------------------------------------------------------
//Dump the type stack to stdout.
void Tracker::debugPrint(string msg) {

    cout << "\n--" << msg << "-------- TRACKER (top) ----------\n";
    list<Frame>::iterator it = typeStack.begin();
    for (; it != typeStack.end(); ++it) {
        if (!it->type)
            ERR("fatal err - encountered null on type stack");
            
        cout <<  it->str << "\t" << it->type->dump() << endl;
    }
    cout << "------------- (bottom)-------------\n";
}

/////////////////////// PRIVATE METHODS //////////////////////////

//----------------------------------------------------------------------------
//Return the top of the type stack.
Frame Tracker::peek()
{
    if (typeStack.empty()) {
        Frame invalid;
        invalid.str = "INVALID";
        invalid.type = new InvalidType();
        push(invalid);
        return invalid;
    }
    
    return typeStack.front();
}

//----------------------------------------------------------------------------
//Return the second-from-the top frame
Frame Tracker::peek2() 
{
    Frame top = pop();
    Frame second = peek();
    push(top);
    return second;
}

//----------------------------------------------------------------------------
//Return and remove the top of the type stack.
//
//TODO: This is not private in the class. Should be moved up in the file.
Frame Tracker::pop()
{   
    Frame f = peek();
    typeStack.pop_front();
    return f;    
}

//----------------------------------------------------------------------------
//Push a new frame onto the type stack.
void Tracker::push(Frame f) 
{
    typeStack.push_front(f);
}
