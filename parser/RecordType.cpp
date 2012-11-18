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

void RecordType::generateCode(ostream &out, string ident)
{
    out << "struct " << ident << " {" << endl;
    list<Variable>::iterator it = fields.begin();
    for (; it != fields.end(); it++) {
        out << "    ";
        it->generateCode(out, it->identifier);
        out << ";" << endl;
    }
    out << "} " << ident;
}

//Returns a string representation. The cell record is formatted:
//
//    cell
//         id integer
//         info int3d
//         next cellptr
//
string RecordType::toString()
{
    stringstream ss (stringstream::in | stringstream::out);
    ss << nlindent();
    list<Variable>::iterator it = fields.begin();
    for (; it != fields.end(); it++)
        ss << "    " << it->toString();
    return ss.str();
}
