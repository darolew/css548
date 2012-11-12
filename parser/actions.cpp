// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// TODO: Write description of this file.
#include "actions.h"

list<string> idList;
list<Range> rangeList;
list<Ptrinfo> ptrList;
list<Variable> fieldList;
Function *currFunction = NULL;
AbstractType *currType = NULL;

//This method walks through the lists of pointers declared in the source code.
//It assigns a pointed-to-type to each pointer. This method is invoked when
//exiting the type def block in the source code. It allows pointers to point
//to types that have not yet been defined.
void assignTypesToPointers(void)
{
    while (!ptrList.empty()) {
        Ptrinfo pi = ptrList.front();
        pi.ptrType->addType(*pi.pointee);
        
        //The string pointee was copied in the previous statement. 
        //Free the memory.
        delete pi.pointee;
        
        ptrList.pop_front();
    }
}

//When a pointer type is defined, a new object for it and inserted into the 
//symbol table. The type of the pointer is saved locally as a string, along
//with its pointer object. That string will be used as a key to lookup the 
//type object at the end of the type def block.
void addPointerToList(string nameOfPointer, string nameOfPointee)
{
    Ptrinfo pi;
    pi.ptrType = new PointerType(nameOfPointer);
    pi.pointee = new string(nameOfPointee);
    ptrList.push_front(pi);
    symTable.insert(pi.ptrType);
}

//Walk the list of variable names being declared. For example, the declaration
//"a,b,c : interger;" includes a list of variables {a, b, c} and their type, 
//integer. For each one, a new variable object is created, assigned a type,
//and entered into the symbol table. The list is emptied as the varaibles are
//inserted into the symbol table.
 void insertCurrentVariableDecl(void) 
 {
    while (!idList.empty()) {
        string name = idList.front();
        symTable.insert(new Variable(name, currType));
        idList.pop_front();
    }
}
                    
//Convience method for creating Terminal struct.
Terminal *newTerminal(string lexeme, int token, char unaryOperatorChar)
{
    Terminal *terminal = new Terminal;
    *terminal = initTerminal(lexeme, token, unaryOperatorChar);
    return terminal;
}

Terminal initTerminal(string lexeme, int token, char unaryOperatorChar)
{
	Terminal terminal;
    terminal.str = lexeme;
    terminal.token = token;
    terminal.unaryOp = unaryOperatorChar;
    return terminal;
}

//Create a new array type and add it to the symbol table.
//Use the ranges build up in the rangeList. Empty the range
//list when the insert is complete.
void insertArrayType()
{
    AbstractType *elementType = currType;
    currType = new ArrayType(elementType, rangeList);
    rangeList.erase(rangeList.begin(), rangeList.end());
}

//Create a range object, set its members, and push it on a list.
void addRange(Terminal *low, Terminal *high)
{
    Range range;
    range.low = *low;
    range.high = *high;
    rangeList.push_back(range);
}
 
//Remove an identifer and turn it into a varaible as part of a record's fields.
void addField()
{
    while (!idList.empty()) {
        string id = idList.front();
        if (isDuplicateField(id)) {
            cerr << "error: " << id << " already exists in record\n";
        } else {
            Variable field(id, currType);
            fieldList.push_front(field);
        }
        idList.pop_front();
    } 
}

//Check for duplicated field names in a record.
bool isDuplicateField(string id) 
{
    list<Variable>::iterator it = fieldList.begin();
    for (; it != fieldList.end(); it++) {
        Variable &prev = *it;
        if (id == prev.identifier)
            return true;
    }
    return false;
}

//Create a formal method parameter from a list of identifiers.
//Add the parameter to the object for the current function.
void addFormalParam(string typeName) 
{
    while (!idList.empty()) {
        string name = idList.front();
        AbstractType *formalType = symTable.lookupType(typeName);
        Variable *formalParam = new Variable(name, formalType);
        currFunction->addParam(formalParam);
        delete formalParam;
        idList.pop_front();
    }
}

//Not much to say about this one.
void beginScope(char *name) 
{
    currFunction->identifier = name;
    symTable.insert(currFunction);
    symTable.beginScope(name);
}
