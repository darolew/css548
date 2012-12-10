// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This file contains the method definitions of the RecordType class.

#include "RecordType.h"
#include "main.h"

//Simple constructor that takes a list of the records fields and copies them
//into itself.
RecordType::RecordType(list<Variable> fields) : AbstractType()
{
    //This does an explicit field-by-field copy since simply assigning the
    //field list does not seem to work long-term.
    list<Variable>::iterator it = fields.begin();
    for (; it != fields.end(); it++)
        this->fields.push_front(*it);
}

//Generate the equivalent C++ code for this record.
void RecordType::generateCode(string ident)
{
    cout << "struct " << ident << " {" << endl;
    list<Variable>::iterator it = fields.begin();
    for (; it != fields.end(); it++) {
        cout << "    ";
        it->generateCode(it->identifier);
        cout << ";" << endl;
    }
    cout << "} " << ident;
}

//A RecordType is a record.
bool RecordType::isRecordType() 
{
    return true;
}

//Check if an identifier is a field in this record.
Variable *RecordType::lookup(string ident) 
{
    list<Variable>::iterator it = fields.begin();
    for (; it != fields.end(); it++) {
        if (ident == it->identifier)
            //Return pointer to the record's field. The seemingly redundant
            //'&*' notation exists to work-around a g++ error.
            return &*it; 
    }
    
    return NULL;
}
