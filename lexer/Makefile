# CSS 548; Autumn 2012
# Aaron Hoffer and Daniel Lewis
# Generate lexer using Flex and compiler it with gcc.

lexer:	lexer.l tokenutil.c
	flex lexer.l
	gcc lex.yy.c tokenutil.c -o lexer

clean:
	rm -f lexer lex.yy.c
