# CSS 548; Autumn 2012
# Aaron Hoffer and Daniel Lewis
# Generate parser using Yacc and compile it with g++.

OBJ = y.tab.o lex.yy.o main.o AbstractType.o actions.o ArrayType.o \
    BaseType.o Const.o Function.o InvalidType.o Parameter.o PointerType.o \
    RecordType.o SetType.o Symbol.o Tracker.o SymTable.o Variable.o
EXTRA = IntSet.o
H = y.tab.h actions.h  AbstractType.h ArrayType.h BaseType.h Const.h \
    Function.h IoFunction.h InvalidType.h Parameter.h PointerType.h \
    Range.h RecordType.h SetType.h Symbol.h SymTable.h Tracker.h  \
    Variable.h
CC = g++

all: all_quiet
	@echo Done

all_quiet: $(OBJ) $(EXTRA) $H
	@$(CC) $(OBJ) -g -o a.out -lfl 
    
main.o: $H main.cpp
	@$(CC) -g -c main.cpp

AbstractType.o: $H AbstractType.cpp
	@$(CC) -g -c AbstractType.cpp

ArrayType.o: $H ArrayType.cpp
	@$(CC) -g -c ArrayType.cpp

actions.o: $H actions.cpp
	@$(CC) -g -c actions.cpp

BaseType.o: $H BaseType.cpp
	@$(CC) -g -c BaseType.cpp

Const.o: $H Const.cpp
	@$(CC) -g -c Const.cpp

Function.o: $H Function.cpp
	@$(CC) -g -c Function.cpp

#NOTE: This file is only used by generated code, not the compiler, but we
#      should make sure it builds.
IntSet.o: IntSet.h IntSet.cpp
	@$(CC) -g -c IntSet.cpp

InvalidType.o: $H InvalidType.cpp
	@$(CC) -g -c InvalidType.cpp

lex.yy.o: $H lex.yy.c
	@$(CC) -g -c lex.yy.c

lex.yy.c: $H lexer.l
	@flex lexer.l

Parameter.o: $H Parameter.cpp
	@$(CC) -g -c Parameter.cpp

PointerType.o: $H PointerType.cpp
	@$(CC) -g -c PointerType.cpp

RecordType.o: $H RecordType.cpp
	@$(CC) -g -c RecordType.cpp

SetType.o: $H SetType.cpp
	@$(CC) -g -c SetType.cpp

SymTable.o: $H SymTable.cpp
	@$(CC) -g -c SymTable.cpp

Symbol.o: $H Symbol.cpp
	@$(CC) -g -c Symbol.cpp

Tracker.o: $H Tracker.cpp
	@$(CC) -g -c Tracker.cpp

Variable.o: $H Variable.cpp
	@$(CC) -g -c Variable.cpp

y.tab.o: y.tab.c
	@$(CC) -c y.tab.c

y.tab.c: parser.y
	@bison -dv parser.y

clean:
	@rm -f a.out lex.yy.c y.tab.h y.tab.c $(OBJ) $(EXTRA) *.output *.gch

test: all_quiet
	@./run

print:
	@echo '************************************************'
