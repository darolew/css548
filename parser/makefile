# CSS 548; Autumn 2012
# Aaron Hoffer and Daniel Lewis
# Generate parser using Yacc and compile it with gcc.

OBJ = y.tab.o lex.yy.o main.o SymTable.o Symbol.o PointerType.o OfType.o
H   = y.tab.h SymTable.h Symbol.h BaseType.h TypeDef.h Type.h Variable.h Const.h Array.h PointerType.h OfType.h
CC  = g++ 

all: print $(OBJ) $H
	@$(CC) $(OBJ) -g -o a.out -lfl
	@echo Done
		
print:
	@echo '************************************************'
	
main.o: $H main.cpp                
	@$(CC) -g -c main.cpp

SymTable.o: $H SymTable.cpp
	@$(CC) -g -c SymTable.cpp

Symbol.o: $H Symbol.cpp
	@$(CC) -g -c Symbol.cpp	
	
PointerType.o: $H PointerType.cpp
	@$(CC) -g -c PointerType.cpp
	
OfType.o: $H OfType.cpp
	@$(CC) -g -c OfType.cpp	
	
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
