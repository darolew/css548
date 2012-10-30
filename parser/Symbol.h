#ifndef SYMBOL
#define SYMBOL

#include <string> 
#include "Type.h"

using namespace std;

/* Class to hold a symbol's attributes
 *
 */

class Symbol {

public:
	Symbol(string id, Type* t) : ident(id), type(t) {}
	string ident;
	Type* type;

};

#endif

