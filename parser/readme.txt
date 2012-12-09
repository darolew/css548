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
also since we do not support nested procedures.

All of these error programs produce the expected errors:

    arrayerror.p    - produces expected errors
    sterror2.p      - produces expected errors
    harderror.p     - produces expected errors
    simpleerror.p   - produces expected errors
    paramtypeerror.p - produces expected errors
    errors.p        - produces expected errors
    

Section 5:

    -Sets
    -Array subscripts
    -Pointers
    -Return values from functions


Section 6:

-This is a one pass, print-as-you-go translator.

-Type checking is implemented using a stack of types which are pushed, 
compared, and popped as required.

-Pointers and arrays were especially difficult. Pascal pointers are dereferenced
after the identifier (ident^). C++ pointers are dereferenced before 
the identifier (*ident). Printing-as-you-go means that by the time you 
discover the dereference in Pascal, the identifier has already been printed.
The problem was solved by dereferencing by using C's array access notation 
to dereference pointers. That is, in C these two statement are equivalent:
    *ident
    ident[0]

The translator appends the string "[0]" to pointer identifiers 
to dereference them.    
    
-Arrays were more challenging. Pascal array are indexed with an
expression list inside of a single set of the terminals "[" and "]". 
C arrays are dereferenced with a single expression inside multiple sets of
"[" and "]" terminals. 

The solution was to use the type-checking stack. The parser can query the 
type-checking stack (called the "tracker") to determine whether an array type
is at the top of the stack. If it is, the parser prints the appropriate C
translation each time an expression is parsed. A similar mechanism is used for
functions. 

Here are notes on how functions were intended to work:

    Idea was not to push the function onto the tracker because it is not 
    a type. Instead, the idea was to push the return type (if it is a 
    function and not a procedure) and then push the parameters on to 
    the tracker (in reverse order).
    For example, the function
    
        function average(newrec: cellPtr, offset real): integer;   
     
    would push itself onto the tracker like this:
    
     | CELLPTR | (formal parameter)
     | REAL |  | (formal parameter)
     | INTEGER | (the return type)
    
    After the parser finishes parsing ActualParameters, it checks to 
    see that the actual parameters match the formal parameters:
    
     | CELLPTR | (actual parameter)
     | REAL |  | (actual parameter)
     | CELLPTR | (formal parameter)
     | REAL |  | (formal parameter)
     | INTEGER | (the return type)
        
    The actual and formal parameter types match. No error is printed.
    The tracker pops the actual and formal parameters off the stack,
    leaving only the return type on the stack:
    
     | INTEGER | (the return type)

A more advanced solution would be to use a stack to track all symbols and use
the syntax actions as events which trigger print C++ code for items which are 
deeper in the stack.

-Used C++ RTTI to identify types. Not really a bug, but not recommended 
OO style. Future work includes improving style.

-Implementing the functionalty took priority over finding and fixing mem leaks.

-Pascal's divide operation returns a real when dividing two integer. C's
divide operation returns an integer under the same circumstances. Our 
compiler should have one of the integer operands to float or double so the
C program would have the same behavior as the Pascal program. It does not.

Section 7:

    Tell me whether your pair/group would like to demonstrate your compiler to
    the class or would rather show me individually. This is not meant to be a
    big deal, not a formal presentation; you'll just "make" it and run it with
    sample code perhaps describing how you handled something or challenges you
    ran into. 
