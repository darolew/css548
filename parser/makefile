# CSS 548; Autumn 2012
# Aaron Hoffer and Daniel Lewis
# Generate parser using Yacc and compile it with gcc.

OBJ = y.tab.o lex.yy.o main.o SymTable.o
H   = y.tab.h SymTable.h Symbol.h BaseType.h TypeDef.h AbstractType.h Variable.h Const.h Array.h
CC  = g++ 

all: $(OBJ) $H
	@$(CC) $(OBJ) -g -o a.out -lfl
	@echo Done
		
main.o: $H main.cpp                
	@$(CC) -g -c main.cpp

SymTable.o: $H SymTable.cpp
	@$(CC) -g -c SymTable.cpp

lex.yy.o: $H lex.yy.c                
	@$(CC) -g -c lex.yy.c

lex.yy.c: $H lexer.l                 
	@flex lexer.l                   

y.tab.o: y.tab.c               
	@$(CC) -c y.tab.c

y.tab.c: parser.y                   
	@yacc -dv parser.y

clean:
	@rm -f a.out lex.yy.c y.tab.h y.tab.c *.o *.output
