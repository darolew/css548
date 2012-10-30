#ifndef SYMBOL
#define SYMBOL

#include <string> 
#include "y.tab.h"

using namespace std;

/* Class to hold a symbol's attributes
 *
 */

class Symbol {

public:
	Symbol(string id, int token) : identifier(id), token(token) {}
	string identifier;
	int token;

};

#endif

