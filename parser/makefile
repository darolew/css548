# CSS 548; Autumn 2012
# Aaron Hoffer and Daniel Lewis
# Generate parser using Yacc and compile it with gcc.

OBJ = parser.tab.o lex.yy.o 
H = parser.tab.h tokenconsts.h
CC=gcc

all: $(OBJ) $H                   
	$(CC) $(OBJ) -o a.out -fl -ly

main.o: main.cpp                   
	$(CC) -c  main.cpp

lex.yy.o: $H lex.yy.c                
	$(CC) -c lex.yy.c -lfl

lex.yy.c: $H lexer.l                 
	flex lexer.l                   

parser.tab.o: parser.tab.c               
	$(CC) -c parser.tab.c -ly

parser.tab.c: parser.y                   
	yacc -d -v parser.y              

clean:
	rm -f parser lex.yy.c parser.tab.h parser.tab.c *.o *.output
