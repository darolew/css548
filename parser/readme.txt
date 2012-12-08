CSS 548; Autumn 2012
Aaron Hoffer and Daniel Lewis
Phase 4

Section 1:
First list all the "regular" Pascal programs that translate correctly. For example:
    add.p - translates correctly
Then list any of the "regular" Pascal programs that do NOT translate properly. If a part does not translate, describe precisely what is wrong. If you have not implemented something, e.g., the case statement, that is precise enough. For example:
    case.p - case statement is not implemented
If something mostly works, but does not work in some situations, describe that. If everything translates, say that.

Section 2:
First list all the "medium" Pascal programs that translate correctly. For example:
    array.p - translates correctly
Then list any of the "medium" Pascal programs that do NOT translate properly. If a part does not translate, describe precisely, but briefly, what is wrong.

Section 3:
As before, list all the "hard" Pascal programs that translate correctly. Then list the "hard" Pascal programs that do NOT translate properly. If a part does not translate, describe precisely what is wrong. Again, if you have not implemented something, e.g., nested procedures, that is precise enough. For example:
    phase2.p - nested procedures are not implemented
If none of these are implemented, say none.

Section 4:
List any of the Pascal programs with "errors" that you do not display an appropriate error message. I.e., say which errors you do not detect.

Section 5:
List things in Pascal that you have handled and are proud of that I do not test in my sample Pascal Programs. Feel free to be brief and point me to sample Pascal programs that you have submitted. If there is nothing, say none.

Section 6:
Describe anything else you think I should know that will help me to assess what you have done. Describe anything that was way more challenging than you originally thought (and that you suspect I don't know the depth of the challenges).

Section 7:
Tell me whether your pair/group would like to demonstrate your compiler to the class or would rather show me individually. This is not meant to be a big deal, not a formal presentation; you'll just "make" it and run it with sample code perhaps describing how you handled something or challenges you ran into. 



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