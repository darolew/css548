// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// A mathematical set of integers (positive or negative). Functionality
// includes the ability to find the union, intersection, and difference;
// whether a given set is a subset; element insertion; and whether an
// element is already in the set.
#ifndef INTSET_H
#define INTSET_H
#include <iostream>
#include <cstdarg>
#include <climits>
using namespace std;

//Constant used with IntSet::makeLiteral()
#define SETTERM INT_MIN

//Note: This class is embedded into the generated code, so its name must not
//      conflict with any Pascal identifiers. The name "IntSet" includes
//      capital letters, and we convert all Pascal identifiers to lower case,
//      so since C++ is case sensitive, this name is safe.
class IntSet {
    
public:
    int min;
    int max;

private:
	bool *numbers;
    
public:
    IntSet();
    virtual ~IntSet();

	IntSet operator+(const IntSet&) const;
	IntSet operator*(const IntSet&) const;
    IntSet operator-(const IntSet&) const;
    bool operator<=(const IntSet&) const;
	IntSet &operator=(const IntSet&);
    
    void insert(int);
    void clear();
	bool isInSet(int) const;
    
    static IntSet makeLiteral(int firstval, ...);
    
private:
    void grow(int);
    void set(int, bool);
    int size() const;
};


//Overload an arbitrary operator for the "in" Pascal operator.
bool operator%(const int&, const IntSet&);

#endif
