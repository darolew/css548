// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This class represents the definition of a Pacal record.
// It has the name of the record and a list of the records
// fields. The fields are stored as instances of the Variable
// class that have not been entered into the symbol table.

#ifndef RECORDTYPE_H
#define RECORDTYPE_H

#include <list>
#include "AbstractType.h"
#include "Variable.h"

class RecordType : public AbstractType {

private:
    list<Variable> fields;  // fields contained within the record

public:
    //Refer to the .cpp file for descriptions of these methods.
    RecordType(list<Variable>);
    virtual void generateCode(string);
    virtual bool isRecordType();
    Variable *lookup(string);
    virtual string toString();
};

#endif
