CSS 548; Autumn 2012
Aaron Hoffer and Daniel Lewis
Phase 4

Design
------
One pass print-as-you-go translator.
Type checking is implmeneted using a stack of types which are compared
and popped as required.

Pointers and arrays were especially difficult. Pascal pointers are deferenced
after the identifier (ident^). C++ pointers are derefrenced before 
the identifier (*ident). Printing-as-you-go means that by the time you 
discover the dereference in Pascal, the identifier has already been printed.
The problem was solved by dereferencing by using C's array acess notation 
to defreference pointers. That is, in C these two statement are equivalent:
    *ident
    ident[0]

The translator appends the string "[0] to pointer identifiers 
to dereference them.    
    
Array were more challenging. Pascal array are indexed with an
expression list inside of a single set of the terminals "[" and "]". 
C ararys are dereferenced with a single expression inside multiple sets of
"[" and "]" terminals. 

The solution was to use the type-checking stack. The parser can query the 
type-checking stack (called the "tracker") to determine is an array type is at
the top of the stack. If it is, the parser prints the appropriate C translation
each time an expression is parsed. A similar mechanism is used for functions. 

A more advanced solution would be to use a stack to track all symbols and use
the syntax actions as events which trigger print C++ code for items which are 
deeper in the stack.

Bugs
----

Positive tests were a higher priority than negative testing.
It is likely that unknown bugs exist.

Mem leaks
---------
Implementing the functionalty took priority over finding and fixing mem leaks.


Who Did What
------------
Very collaborative.