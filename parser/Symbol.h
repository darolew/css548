#ifndef SYMBOL
#define SYMBOL

#include <string> 

using namespace std;

/* Class to hold a symbol's attributes
 *
 */

class Symbol {
public:
	Symbol(string id, int token) : identifier(id), token(token) {}
	Symbol(string id) : identifier(id) {}
	virtual string toString()
	{
		return identifier;
	}
	
	virtual bool isType() {
	    return false;
	}
	string identifier;
	int token;
};

#endif

