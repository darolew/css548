// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
// 
// This file contains the method definitions of the RecordType class

#include <sstream>
#include "RecordType.h" 

//Simple constructor that takes a list of the records fields and copies them
//into itself.
RecordType::RecordType(list<Variable> fields) : AbstractType("")
{
    list<Variable>::iterator it = fields.begin();
    for (; it != fields.end(); it++)
        this->fields.push_front(*it);
}

//Returns a string representation. The cell records is formatted:
//
//      cell
//          id integer
//          info int3d
//          next cellptr
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
