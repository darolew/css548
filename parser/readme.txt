CSS 548; Autumn 2012
Aaron Hoffer and Daniel Lewis
Phase 3


Design
------

The basic data structure for the symbol table is a stack of lists. When a
new scope is is detected in the parser, a new list is created and pushed
onto the stack. When leaving a scope, the stack is popped.

The class SymTable is the abstraction that represents the stack of
lists. The lists themselves hold pointers to subclasses of Symbol. Any
subclass of Symbol can be inserted into the symbol table. Earlier, the
SymTable implementation used Maps (key/value hash tables) where each
symbol's identifier was the key to a symbol objects' pointer. However,
there was value in printing the symbols in the order they were defined
and the Map ADT did not support this.

The SymTable class supports several basic operations:

    -Begin scope
    -End scope
    -Insert a symbol
    -Look up any symbol
    -Look up any symbol that represents a type

The method SymTable::beginScope(string) creates a new, named scope. The
very first scope is created when the SymbTable object is created. This
scope is the Standard Identifier Table (SIT) and contains the keywords
and fundamental data types. The next scope to be created is the name of
the program. Successive scopes represent functions or procedures.

There is a SymTable::printST() method that is called everytime a scope is
exited. It prints the identifiers of all the symbols that were defined
in that scope. The Symbol calls hierarchy defines print methods. These
methods are invoked from SymTable::printST().


Bugs
----

- valgrind still reports memory leaks, some of which are probably our fault.

Who Did What
------------

Most of the code was written in pair-programming sessions in the Linux
lab. Both Aaron and Daniel contributed individual coding sessions, mostly
to cleanup code but occasionally implementing new functionality.  Daniel 
modified the grammar to make it easier to build the symbol table without having
the benefit of an AST. In particular, the introduction of the non-termal
"NPType" (non-pointer type) allowed pointers to be declared ahead of the type
to which they point. Daniel also introduced what the Dragon Book calls 
"markers"-- empty string productions that trigger semantic actions. The 
non-terminal "PointerCheck" is an example of a marker. Danield also came up
with an elegant way to prevent reserved words like "goto" from being used as
identifiers. His solution was to modify the lexer the return yunknown for these
words.

Aaron got the class hierarchy started. Both Daniel and Aaron beat it into
shape over the course of the project. Aaron introduced objected-oriented 
constructs like virtual unctions. He also came up with the mechanism where
each class in the hierachy can insert itself into the symbol table in its own
special way. He overrode the toString method in the sub-classes of Symbol.

Gremlins are responsible for all typos, bugs, and ugly code.
