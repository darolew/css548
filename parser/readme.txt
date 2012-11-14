CSS 548; Autumn 2012
Aaron Hoffer and Daniel Lewis
Phase 3


Design
------

The basic data structure for the symbol table is a stack of lists. When a
new scope is detected by the parser, a new list is created and pushed
onto the stack. When leaving a scope, the stack is popped and the list is
discarded.

The class SymTable is the abstraction that represents the stack of
lists. The lists themselves hold pointers to subclasses of Symbol. Any
subclass of Symbol can be inserted into the symbol table. Originally, the
SymTable implementation used Maps (key/value hash tables) where each
symbol's identifier was the key to a symbol object's pointer. However,
there was value in printing the symbols in the order they were defined
and the Map did not support this.

The SymTable class implements several basic operations:

    -Begin scope
    -End scope
    -Insert a symbol
    -Look up any symbol
    -Look up any symbol that represents a type

The method SymTable::beginScope(string) creates a new, named scope. The
very first scope is created in the SymbTable's constructor. This scope
is the Standard Identifier Table (SIT) and contains the fundamental data
types, memory functions, and IO functions. The next scope to be created is the
name of
the program. Successive scopes represent functions or procedures.

There is a SymTable::printST() method that is called every time a scope is
exited. It prints the identifiers of all the symbols that were defined
in that scope. The Symbol class hierarchy defines print methods that are
invoked from SymTable::printST().


Bugs
----

-Mem leaks
    valgrind still reports memory leaks, some of which are probably our fault.

-Arrays of pointers that point to unknown identifiers
    The file sterror2 defines an array:
   
        anotherArray = array [5..9] of ^apple;

    However, "apple" is not a type-- it is an unknown identifier. Our program
    should set the type of this array to be an (incomplete) instance of 
    PointerType. However, our program sets the array type to null. The 
    actual output is:

        anotherarray  5..9 <bogus_type> 
        
    The proper output show would show the array type as a pointer to a bogus type
    with the caret:
    
        anotherarray  5..9 ^ <bogus_type> 

Who Did What
------------
Most of the code was written in pair-programming sessions in the Linux
lab. Both Aaron and Daniel contributed individual coding sessions, mostly
to cleanup code but occasionally to implement new functionality. 

Daniel modified the grammar to make it easier to build the symbol table. In
particular, he introduced of the non-terminal "NPType" (non-pointer type)
allowed pointers to be declared ahead of the type to which they point. Daniel
also introduced what the Dragon Book calls "markers"-- empty string productions
that trigger semantic actions. The non-terminal "PointerCheck" is an example of
a marker. Daniel also came up with an elegant way to prevent reserved words
like "goto" from being used as identifiers. His solution was to modify the 
lexer the return yunknown as the token type for these words.

Aaron got the class hierarchy started. Both Daniel and Aaron beat the class
structure into submission over the course of the project. Aaron introduced
objected-oriented constructs like virtual functions. He contributed the
mechanism where each subclass of Symbol inserts itself into the symbol table
in its own special way. Aaron in made a heroic effort to cleanup the memory
leaks.

Gremlins are responsible for all typos, bugs, and ugly code.


How to read the output
----------------------
When entering a scope, the program prints a banner with the name of the scope.
If the program is named "first", you will see:

ENTER Standard Identifier Table 
---------------------------------------------------------------------------

ENTER first
---------------------------------------------------------------------------

The first line tells you the SIT was created and pushed onto the stack. 
It is the most encompassing scope. Then the program scope, "first", 
is created and pushed onto the stack. Notice that the SIT identifiers-- 
"boolean"; "integer"; etcetera-- are not printed when entering the SIT scope.
Instead, they are printed when the SIT is popped off the stack, at the very
end of the output. The last few lines of the output are:

/* scopeExit first */
integer
boolean
real
char
write
writeln
read
readln
new
dispose

EXIT Standard Identifier Table
===========================================================================


Routines and records are printed with their formal parameters indented from
their identifiers In the example below, "i", "j" and "k" are formal parameters
of routine "d".

Formal parameters are displayed as variables in a routine's own scope. 
Subroutines inside of the routine only print their identifiers. For example, 
the "e" and "f" in the output are subroutines defined in "d".
print-out below:

    d
        i integer
        j integer
        k integer

        x integer
        y integer
        e
        f

EXIT d
===========================================================================
/* scopeExit D */
