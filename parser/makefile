# CSS 548; Autumn 2012
# Aaron Hoffer and Daniel Lewis
# Generate parser using Yacc and compile it with gcc.

OBJ = parser.tab.o main.o lexer.o 
H = parser.tab.h tokenconsts.h
CC=gcc

a.out: $(OBJ) $H                   # a.out depends on everything
	$(CC) $(OBJ) -o a.out

main.o: main.cpp                   # main.o depends on main.cpp
	$(CC) -c  main.cpp

lex.yy.o: $H lex.yy.c                
	$(CC) -c lex.yy.c 

lex.yy.c: $H lexer.l                 
	flex lexer.l                   

parser.tab.o: parser.tab.c                   # y.tab.o depends on y.tab.c
	$(CC) -c parser.tab.c

parser.tab.c: parser.y                         # y.tab.c depends on the yacc file, y   
	yacc -d -v parser.y                   # -d separates out defines into .h file

clean:
	rm -f parser lex.yy.c parser.tab.h parser.tab.c *.o
