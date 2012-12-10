CSS 548; Autumn 2012
Aaron Hoffer and Daniel Lewis
Phase 4


Section 1:

Correctly translating "regular" programs:

    simplest.p      - translates correctly
    simplest2.p     - translates correctly
    if.p            - translates correctly
    add.p           - translates correctly
    subscripts.p    - translates correctly
    case.p          - translates correctly
    pointer.p       - translates correctly

Incorrectly translating "regular" programs:

    <none>


Section 2:

Correctly translating "medium" programs:

    array.p         - translates correctly
    array2.p        - translates correctly
    list.p          - translates correctly
    fact.p          - translates correctly
    
Incorrectly translating "medium" programs:

    <none>


Section 3:

Correctly translating "hard" programs:

    set.p           - translates correctly
    list2.p         - translates correctly (except "count")
    
list2.p translates correctly, but it uses a global variable named "count" which
conflicts with names used in the C++ library. If "count" is renamed, then it
works fine.

Incorrectly translating "hard" programs:

    nestedprocs.p   - nested procedures not implemented
    sttest.p        - nested procedures not implemented
    sttest2.p       - nested procedures not implemented
    sttest3.p       - nested procedures not implemented


Section 4:

Error programs that are not entirely correct:
    
    sterror.p       - correct except for nested procedures
    funcerror.p     - finds errors, then terminates with syntax error

sterror.p produces all the correct symbol table errors, but it has other errors
also since we do not support nested procedures. funcerror.p does produce some
of the expected errors, but compilation terminates with a syntax error before
it reaches the end.

All of these error programs produce the expected errors:

    arrayerror.p    - produces expected errors
    sterror2.p      - produces expected errors
    harderror.p     - produces expected errors
    simpleerror.p   - produces expected errors
    paramtypeerror.p - produces expected errors
    errors.p        - produces expected errors
    

Section 5:

The sample Pascal programs test these, but we are still proud of them:

    - Sets
    - Array subscripts
    - Pointers
    - Return values from functions
    - Type checking for math and relational operators


Section 6:

* Compiling and Output

Our compiler supports Pascal sets via the IntSet class, which is placed in the
generated code via an include directive. For this reason, in order to compile
the generated C++ programs, IntSet.cpp and IntSet.h must be placed in the same
directory as the generated C++ source files.

We tried to make our C++ code print with the correct indentation in order to
be more readable. However, there are some quirks; most notably, the closing
brace is usually indended an extra level.

* Design

This is a one pass, print-as-you-go translator.

* Type Checking

Type checking is implemented using a stack of types which are pushed, 
compared, and popped as required.

* Pointers

Pointers were tricky. Pascal pointers are dereferenced after the identifier
(ident^). C++ pointers are dereferenced before the identifier (*ident).
Printing-as-you-go means that by the time you discover the dereference in
Pascal, the identifier has already been printed. The problem was solved by
dereferencing pointers via C's array access notation. That is, in C, these two
statement are equivalent:

    *ident
    ident[0]

The compiler appends the string "[0]" to pointer identifiers to dereference
them. This looks a bit odd, and is poor C++ style, but it works.

* Arrays

Arrays were challenging. Pascal array are indexed with an expression list
inside of a single set of the terminals "[" and "]". C arrays are dereferenced
with a single expression inside multiple sets of "[" and "]" terminals. 

The solution was to use the type-checking stack. The parser can query the 
type-checking stack (called the "tracker") to determine whether an array type
is at the top of the stack. If it is, the parser prints the appropriate C
translation each time an expression is parsed. A similar mechanism is used for
functions. 

A more advanced solution would be to use a stack to track all symbols and use
the syntax actions as events which trigger C++ print code for items which are 
deeper in the stack.

* Sets

There were a couple of tricky problems with sets. The first was handling set
literals. For example, take the Pascal code:

    stuff := [2, 4, 6, 8];

In order to keep the "stuff = " syntax working, the generated code looks like
this:

    stuff = IntSet::makeLiteral(2, 4, 6, 8, SETTERM);

makeLiteral() is a static function in IntSet; it takes a variable number of
arguments (terminated by SETTERM) and creates and returns an object. The
second hard problem was the 'in' operator. For example, take the Pascal code:

    if 3 in stuff then

Note how the 3 precedes stuff in the grammar. Therefore, it was not possible
to generate a function call, like stuff.isInSet(3). We worked around this by
overloading an arbitrary operator in a non-member function. No operator really
made sense, so we just overloaded modulus:

    if (3 % stuff) {
    
Also, note that the IntSet class has no notion of set bounds; it grows as
necessary to hold any set of values. All bounds-checking is performed at
compile time.

* Issues

Used C++ RTTI to identify types. Not really a bug, but not recommended 
OO style. Future work includes improving style.

Implementing functionality took priority over finding and fixing memory
leaks.

Pascal's divide operation returns a real when dividing two integer. C's
divide operation returns an integer under the same circumstances. Our 
compiler should cast one of the integer operands to float or double so the
C program would have the same behavior as the Pascal program. It does not.


Section 7:

We would prefer not to present to the class.
