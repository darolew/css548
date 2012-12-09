CSS 548; Autumn 2012
Aaron Hoffer and Daniel Lewis
Phase 4

Section 1:

    simplest.p      - translates correctly
    simplest2.p     - translates correctly
    if.p            - translates correctly
    add.p           - translates correctly
    subscripts.p    - translates correctly
    case.p          - translates correctly
    pointer.p       - translates correctly

Section 2:

    array.p         - translates correctly
    array2.p        - translates correctly
    list.p          - translates correctly
    fact.p          - translates correctly
    
Section 3:

    set.p           - translates correctly
    nestedprocs.p   - nested procedures not implemented
    sttest.p        - nested procedures not implemented
    sttest2.p       - nested procedures not implemented
    sttest3.p       - nested procedures not implemented
    list2.p         - translates correctly (except "count")

Section 4:

    List any of the Pascal programs with "errors" that you do not display an
    appropriate error message. I.e., say which errors you do not detect.

Section 5:

    List things in Pascal that you have handled and are proud of that I do not
    test in my sample Pascal Programs. Feel free to be brief and point me to
    sample Pascal programs that you have submitted. If there is nothing, say
    none.

Section 6:

    Describe anything else you think I should know that will help me to assess
    what you have done. Describe anything that was way more challenging than
    you originally thought (and that you suspect I don't know the depth of the
    challenges).

Section 7:

    Tell me whether your pair/group would like to demonstrate your compiler to
    the class or would rather show me individually. This is not meant to be a
    big deal, not a formal presentation; you'll just "make" it and run it with
    sample code perhaps describing how you handled something or challenges you
    ran into. 



Design
------

One pass print-as-you-go translator.

Type checking is implemented using a stack of types which are compared
and popped as required.

Pointers and arrays were especially difficult. Pascal pointers are dereferenced
after the identifier (ident^). C++ pointers are dereferenced before 
the identifier (*ident). Printing-as-you-go means that by the time you 
discover the dereference in Pascal, the identifier has already been printed.
The problem was solved by dereferencing by using C's array access notation 
to dereference pointers. That is, in C these two statement are equivalent:
    *ident
    ident[0]

The translator appends the string "[0]" to pointer identifiers 
to dereference them.    
    
Arrays were more challenging. Pascal array are indexed with an
expression list inside of a single set of the terminals "[" and "]". 
C arrays are dereferenced with a single expression inside multiple sets of
"[" and "]" terminals. 

The solution was to use the type-checking stack. The parser can query the 
type-checking stack (called the "tracker") to determine whether an array type
is at the top of the stack. If it is, the parser prints the appropriate C
translation each time an expression is parsed. A similar mechanism is used for
functions. 

A more advanced solution would be to use a stack to track all symbols and use
the syntax actions as events which trigger print C++ code for items which are 
deeper in the stack.

Bugs
----

-The parser does not implement any kind of "panic mode". That is, it cannot 
recover its state after it encounters an error. It will continue to print 
code and additional error methods. It can even SEGFAULT because it is in
an inconsistent state. Therefore, only the first error message it prints
can be trusted. If parser does detect an error, it could attempt recovery by
resetting the state of the tracking and continuing to the next block or 
statement in the parser. The parser would have to modified to use
Yacc's panic mode.

-Sets are not implemented. 

-Nested function definitions are not implemented.

-Nested array acesses such as:  a[i, b[ii, jj, kk], j] is not supported.
         a[i, b[ii, jj, kk], j]

Other type of nested expressions are not supported:
 1. nested function calls
 2. any array access inside a function call
 3. any function call inside an array access.

The original implementation of tracker includes a stack that stored
the number of dimension of each array it encountered. The parser signaled
the tracker every time a dimension was the array was accessed. These numbers
would be tracked together in one stack frame. When the
number of acesses == the number of dimension on the the stack, 
the couting stack is popped, the array type is popped off the type stack, 
and type of the array was pushed We would have to do the same thing for the 
number of parameters in a function.

-Pascal's divide operation returns a real when dividing two integer. C's
divide operation returns an integer under the same circcumstances. Our 
compiler should have one of the integer operands to float or double so the
C program would have the same behavior as the Pascal program. It does not.

-Used C++ RTTI to identify types. Not really a bug, but not recommended 
OO style. Future work includes improving style.

-Positive tests were a higher priority than negative testing.
It is likely that unknown bugs exist.

Memory Leaks
------------

Implementing the functionalty took priority over finding and fixing mem leaks.

Who Did What
------------

Very collaborative.
