/*
 * CSS 548; Autumn 2012
 * Aaron Hoffer and Daniel Lewis
 */
%option noyywrap
%option yylineno
%option case-insensitive

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokenconsts.h"
#include "tokenutil.h"

#define BUF_SIZE (1024*4)

static void print_comment(const char*);
static void die(const char*);

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
<IN_COMMENT>[^*]*\*+	{BEGIN(WARNING); yymore();}
<WARNING>[^)]		{BEGIN(IN_COMMENT); yymore();}
<WARNING>\)		{BEGIN(INITIAL); yymore(); print_comment("/*");}
{WHITESPACE}		{ /* Do nothing */ }
.			{return yunknown;}

%%

int main(void)
{
	int token;
	const char *text;

	for (;;) {
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
	}
}

/*
 * Print out a Pascal comment (stored in yytext) as a C comment.
 * @comment_delim - The delimiter for this type of Pascal comment: '{' or '(*' 
 */
static void print_comment(const char *comment_delim)
{
	size_t delim_len = strlen(comment_delim);
	size_t total_len;
	
	total_len = yyleng;
	total_len -= delim_len * 2;  /* subtract Pascal deliminators */
	total_len += 4;              /* add C deliminators */
	if (total_len >= sizeof buffer)
		die("comment too long for buffer");

	memset(buffer, 0, sizeof buffer);
	strcpy(buffer, "/*");
	memcpy(buffer+2, yytext + delim_len, yyleng - delim_len * 2);
	strcat(buffer, "*/");
	printf("%s\n", buffer);
}

/*
 * Abort the lexer program with an error message.
 */
static void die(const char *message)
{
	fprintf(stderr, "%s\n", message);
	exit(EXIT_FAILURE);
}
