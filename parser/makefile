# CSS 548; Autumn 2012
# Aaron Hoffer and Daniel Lewis
# Generate parser using Yacc and compile it with gcc.

OBJ = y.tab.o lex.yy.o 
H   = y.tab.h
CC  = gcc

all: $(OBJ) $H                   
	@$(CC) $(OBJ) -o a.out -lfl
	@echo Done

lex.yy.o: $H lex.yy.c                
	@$(CC) -c lex.yy.c

lex.yy.c: $H lexer.l                 
	@flex lexer.l                   

y.tab.o: y.tab.c               
	@$(CC) -c y.tab.c

y.tab.c: parser.y                   
	@yacc -dv parser.y

clean:
	@rm -f parser lex.yy.c y.tab.h y.tab.c *.o *.output
