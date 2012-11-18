# CSS 548; Autumn 2012
# Aaron Hoffer and Daniel Lewis
# Generate parser using Yacc and compile it with g++.

OBJ = y.tab.o lex.yy.o main.o AbstractType.o actions.o ArrayType.o \
    BaseType.o Const.o Function.o PointerType.o RecordType.o SetType.o \
    Symbol.o SymTable.o Variable.o
H = y.tab.h actions.h  AbstractType.h ArrayType.h BaseType.h Const.h \
    Function.h IoFunction.h MemFunction.h PointerType.h Range.h RecordType.h \
    SetType.h Symbol.h SymTable.h Variable.h
CC = g++

all: all_quiet
	@echo Done

all_quiet: $(OBJ) $H
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

lex.yy.o: $H lex.yy.c
	@$(CC) -g -c lex.yy.c

lex.yy.c: $H lexer.l
	@flex lexer.l

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

Variable.o: $H Variable.cpp
	@$(CC) -g -c Variable.cpp

y.tab.o: y.tab.c
	@$(CC) -c y.tab.c

y.tab.c: parser.y
	@bison -dv parser.y

clean:
	@rm -f a.out lex.yy.c y.tab.h y.tab.c $(OBJ) *.output *.gch

test: all_quiet
	@./run

print:
	@echo '************************************************'
