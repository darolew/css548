/*
 * CSS 548; Autumn 2012
 * Aaron Hoffer and Daniel Lewis
 *
 * This is a Flex input file. Flex will generate a scanner in C code from this
 * definition. The tokens defined represents a subset of Pascal and are
 * phase I of the CSS 548 compiler project.
 */
 
%option noyywrap
%option yylineno
%option case-insensitive

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "tokenconsts.h"
#include "tokenutil.h"

#define BUF_SIZE (1024*4)

static void print_comment(const char*);
static void patch_comment(char*);
static void die(const char*);

/* 
 * If EOF encountered in the middle of a comment, set this flag so the main 
 * method can terminate properly.
 */
static bool eof;

/* 
 * Character buffer for comments and strings.
 */
static char buffer[BUF_SIZE];
%}

LETTER		[a-z]
DIGIT		[0-9]
ID		{LETTER}({LETTER}|{DIGIT})*
BCOMMENT	\{[^}]*\}
INT		{DIGIT}+
EXP		[Ee][+-]?{INT}
REAL		{INT}\.{INT}{EXP}?
NUMBER		{INT}|{REAL}
STRINGDQ	\"[^"]*\"
STRINGSQ	\'[^']*\'
STRING		{STRINGDQ}|{STRINGSQ}
WHITESPACE	[ \n\t\r]

%x IN_COMMENT
%x WARNING

%%

and			{return yand;}
array			{return yarray;}
":="			{return yassign;}
begin			{return ybegin;}
"^"			{return ycaret;}
case			{return ycase;}
":"			{return ycolon;}
","			{return ycomma;}
const			{return yconst;}
dispose			{return ydispose;}
div			{return ydiv;}
"/"			{return ydivide;}
do			{return ydo;}
"."			{return ydot;}
".."			{return ydotdot;}
downto			{return ydownto;}
else			{return yelse;}
end			{return yend;}
"="			{return yequal;}
false			{return yfalse;}
for			{return yfor;}
function		{return yfunction;}
">"			{return ygreater;}
">="			{return ygreaterequal;}
if			{return yif;}
in			{return yin;}
"["			{return yleftbracket;}
"("			{return yleftparen;}
"<"			{return yless;}
"<="			{return ylessequal;}
"-"			{return yminus;}
mod			{return ymod;}
"*"			{return ymultiply;}
new			{return ynew;}
nil			{return ynil;}
not			{return ynot;}
"<>"			{return ynotequal;}
of			{return yof;}
or			{return yor;}
"+"			{return yplus;}
procedure		{return yprocedure;}
program			{return yprogram;}
read			{return yread;}
readln			{return yreadln;}
record			{return yrecord;}
repeat			{return yrepeat;}
"]"			{return yrightbracket;}
")"			{return yrightparen;}
";"			{return ysemicolon;}
set			{return yset;}
then			{return ythen;}
to			{return yto;}
true			{return ytrue;}
type			{return ytype;}
until			{return yuntil;}
var			{return yvar;}
while			{return ywhile;}
write			{return ywrite;}
writeln			{return ywriteln;}
{ID}			{return yident;}
{BCOMMENT}		{print_comment("{");}
{NUMBER}		{return ynumber;}
{STRING}		{return ystring;}
<INITIAL>\(\*		{BEGIN(IN_COMMENT); yymore();}
<IN_COMMENT><<EOF>>	{eof = true; return yunknown;}
<IN_COMMENT>[^*]*\*+	{BEGIN(WARNING); yymore();}
<WARNING><<EOF>>	{eof = true; return yunknown;}
<WARNING>[^)]		{BEGIN(IN_COMMENT); yymore();}
<WARNING>\)		{BEGIN(INITIAL); yymore(); print_comment("/*");}
{WHITESPACE}		{ /* Do nothing */ }
.			{return yunknown;}

%%

/* 
 * Test the lexer. Takes the input text and turns it into a token stream. 
 * Prints the token number, token name, and any data associated with it.
 * Exit when the input text is exhausted.
 */
int main(void)
{
	int token;
	const char *text;

	do {
		token = yylex();
		if (!token)
			break;
		if (token == ystring) {
			if (yyleng >= sizeof buffer)
				die("string too long for buffer");
			/* copy in the string, minus the quotation marks */
			memcpy(buffer, yytext+1, yyleng-2);
			buffer[yyleng-2] = '\0';
			text = buffer;
		} else if (token == ynumber || token == yident ||
		           token == yunknown) {
			text = yytext;
		} else
			text = "";
		printf("%-5d %-15s  %s\n", token, token_name(token), text);
	} while(!eof);
	
	return 0;
}

/*
 * Print out a Pascal comment (stored in yytext) as a C comment.
 * @comment_delim - The delimiter for this type of Pascal comment: '{' or '(*' 
 */
static void print_comment(const char *comment_delim)
{
	size_t delim_len = strlen(comment_delim);
	size_t total_len;
	
	/* Test the coment does not overflow buffer. */
	total_len = yyleng;
	total_len -= delim_len * 2;  /* subtract Pascal deliminators */
	total_len += 4;              /* add C deliminators */
	if (total_len >= sizeof buffer)
		die("comment too long for buffer");

	/* Convert from Pascal style comment to C style and print it. */
	memset(buffer, 0, sizeof buffer);
	strcpy(buffer, "/*");
	memcpy(buffer+2, yytext + delim_len, yyleng - delim_len * 2);
	patch_comment(buffer+2);
	strcat(buffer, "*/");
	printf("%s\n", buffer);
}

/*
 * Search for instances of asterisk-slash in the Pascal comment and munge
 * them, since those characters terminate a C comment.
 * @comment - Pointer to a buffer containing the text of the Pascal comment.
 */
static void patch_comment(char *comment)
{
	char *close_delim;
	
	for (;;) {
		close_delim = strstr(comment, "*/");
		if (!close_delim)
			break;
		*close_delim = '.';
		comment = close_delim;
	}
}

/*
 * Abort the lexer program with an error message.
 */
static void die(const char *message)
{
	fprintf(stderr, "%s\n", message);
	exit(EXIT_FAILURE);
}
