// CSS 548; Autumn 2012
// Aaron Hoffer and Daniel Lewis
//
// This file contains the method definitions of the IntSet class.
#include "IntSet.h"

#define MIN(a, b)   ((a) < (b) ? (a) : (b))
#define MAX(a, b)   ((a) > (b) ? (a) : (b))

//
// Public IntSet methods
//

//Create an IntSet.
IntSet::IntSet()
{
    numbers = NULL;
    clear();
}

//Destroy an IntSet.
IntSet::~IntSet()
{
    clear();
}

//---------------------------------------------------------------------------
// +
// Add two int sets together. Return the set.
//---------------------------------------------------------------------------
IntSet IntSet::operator+(const IntSet &other) const
{
    IntSet sum;
    bool either;
    
    //The union uses the "larger" of both bounds.
    sum.min = MIN(min, other.min);
    sum.max = MAX(max, other.max);
    
    //Return the empty set if there is nothing in either set.
    if (!sum.size())
        return sum;
    
    sum.numbers = new bool[sum.size()];
    
    //The union consists of numbers that are present in either array.
    for (int num = sum.min; num <= sum.max; num++) {
        either = isInSet(num) || other.isInSet(num);
        sum.set(num, either);
    }
    
    return sum;
}

//---------------------------------------------------------------------------
// *
// Returns a set of what two sets have in common.
//---------------------------------------------------------------------------
IntSet IntSet::operator*(const IntSet &other) const
{
    IntSet cmn;
    bool both;
    
    //The intersection uses the "lower" of the two bounds.
    cmn.min = MAX(min, other.min);
    cmn.max = MIN(max, other.max);
    
    //Return an empty set if the arrays have no overlap.
    if (!cmn.size())
        return cmn;

    cmn.numbers = new bool[cmn.size()];
    
    //The intersection consists of numbers in both sets.
    for (int num = cmn.min; num <= cmn.max; num++) {
        both = isInSet(num) && other.isInSet(num);
        cmn.set(num, both);
    }
   
    return cmn;
}

//---------------------------------------------------------------------------
// -
// Returns a set of the difference between two sets; i.e., all elements that
// are in this set but not in the other set.
//---------------------------------------------------------------------------
IntSet IntSet::operator-(const IntSet &other) const
{
    IntSet diff;
    bool excl;
    
    //The returned set has the same bounds as this set.
    diff.min = min;
    diff.max = max;
    
    //Return the empty set if this set is empty.
    if (!diff.size())
        return diff;

    diff.numbers = new bool[diff.size()];
    
    //Find numbers that are in this set, but not in the other set.
    for (int num = diff.min; num <= diff.max; num++) {
        excl = isInSet(num) && !other.isInSet(num);
        diff.set(num, excl);
    }

    return diff;
}

//---------------------------------------------------------------------------
// <=
// Returns whether this set is a subset of the other set, meaning all of the
// elements in this set are also in the other set.
//---------------------------------------------------------------------------
bool IntSet::operator<=(const IntSet &other) const
{
    //If a number exists in this set that does not exist in the other set,
    //then this is not a subset.
    for (int num = min; num <= max; num++) {
        if (isInSet(num) && !other.isInSet(num))
            return false;
    }
    
    return true;
}

//---------------------------------------------------------------------------
// =
// Assign the set the value of another set.
//---------------------------------------------------------------------------
IntSet &IntSet::operator=(const IntSet &other)
{
    //If necessary, make this array large enough to hold everything in the
    //other array.
    grow(other.min);
    grow(other.max);
    
    //This set has all the same numbers as the other set.
    for (int num = min; num <= max; num++)
        set(num, other.isInSet(num));
    
    return *this;
}

//---------------------------------------------------------------------------
// Inserts a new integer into the set. Returns true if successful, false if
// not.
//---------------------------------------------------------------------------
void IntSet::insert(int num)
{
    //Make sure there is a slot for this number, then set it.
    grow(num);
    set(num, true);
}

//---------------------------------------------------------------------------
// Remove all integers from this set.
//---------------------------------------------------------------------------
void IntSet::clear()
{
    min = 0;
    max = -1;
    if (numbers) {
        delete numbers;
        numbers = NULL;
    }
}

//---------------------------------------------------------------------------
// Checks for the presence of a given integer, returns true if it is in the
// set, false otherwise.
//---------------------------------------------------------------------------
bool IntSet::isInSet(int num) const
{
    return num >= min && num <= max && numbers[num - min];
}

//
// Public Static IntSet utilities
//

//Create a set literal. The first value is stored in firstval, the rest are
//in the variable-length parameter list. A value of SETTERM terminates the
//set literal. If firstval is SETTERM, the result is the empty set.
IntSet IntSet::makeLiteral(int firstval, ...)
{
    IntSet setLiteral;
    va_list vlist;
    int num;
    
    //Start with an empty set.
    setLiteral.clear();
    
    //Add the passed-in integer values to the set literal, until the predefined
    //terminating value is encountered.
    va_start(vlist, firstval);
    for (num = firstval; num != SETTERM; num = va_arg(vlist, int))
        setLiteral.insert(num);
    va_end(vlist);
    
    return setLiteral;
}

//
// Non-Member Operators
//

//---------------------------------------------------------------------------
// %
// Overload an arbitrary operator to support the functionality of the Pascal
// "in" operator. This is much easier than calling isInSet() directly, since
// in Pascal, the integer precedes the set:
//     if (4 in theset) then
// To use isIntSet(), the grammar would have to be ripped apart in order to
// reverse when the two values are printed, to generate "theset.isInSet(4)".
// Using the overloaded operator, we can leave things where they are.
//---------------------------------------------------------------------------
bool operator%(const int &num, const IntSet &set)
{
    return set.isInSet(num);
}

//
// Private IntSet methods
//

//If necessary, increase the size of the underlying array so that the set can
//contain the given number.
void IntSet::grow(int num)
{
    const int oldsize = size();
    const int oldmin = min;
    const int oldmax = max;
    
    //Special case: This set is growing for the first time.
    if (!numbers) {
        min = num;
        max = num;
        numbers = new bool[1];
        numbers[0] = false;
        return;
    }
    
    //Check if this value exceeds either the upper or lower bound; if it does,
    //the array must grow.
    if (num < min)
        min = num;
    else if (num > max)
        max = num;
    else
        return; //Array contains this number; no need to grow.
    
    //Create a new, larger copy of the array.
    int newsize = size();
    bool *grown = new bool[newsize];
    
    //Copy the old contents of the array.
    if (min == oldmin) {
        //Added elements at the end; copy the old contents and initialize the
        //new area to empty.
        for (int i = 0; i < oldsize; i++)
            grown[i] = numbers[i];
        for (int i = oldsize; i < newsize; i++)
            grown[i] = false;
    } else {
        //Added elements at the beginning; initialize the new area to empty and
        //copy the old contents into the equivalent area.
        const int diff = newsize - oldsize;
        for (int i = 0; i < diff; i++)
            grown[i] = false;
        for (int i = diff, j = 0; j < oldsize; i++, j++)
            grown[i] = numbers[j];
    }
    
    //Replace the old array with the grown version.
    delete numbers;
    numbers = grown;
}

//Insert or remove a value from the set. Assumes bounds checking is handled by
//the caller.
void IntSet::set(int num, bool present)
{
    numbers[num - min] = present;
}

//Returns the size of the array.
int IntSet::size() const
{
    if (max < min)
        return 0;

    return max - min + 1;
}
