// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This file contains the method definitions of the RecordType class.

#include <sstream>
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

bool RecordType::isRecordType() 
{
    return true;
}

Variable *RecordType::lookup(string ident) 
{
    Variable *var;
    list<Variable>::iterator it = fields.begin();
    for (; it != fields.end(); it++) {
        if (ident == it->identifier)
            return &*it; //TODO: Fix this: '&*it' means 'it'
    }
    
    return NULL;
}
