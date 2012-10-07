%option noyywrap
%option yylineno
%option case-insensitive

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokenconsts.h"
#include "tokenutil.h"

#define BUF_SIZE (1024*3)

static void print_comment(const char*);

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
		text = "";
		if (!token)
			break;
		if (token == ystring) {
			size_t len = strlen(yytext); // TODO: yyleng
		    	if (len > sizeof buffer) {
		    		fprintf(stderr, "");
		    		exit(1);
		    	}
		    	memcpy(buffer, yytext+1, len-2);
		    	buffer[len-2] = '\0';
		    	text = buffer;
		} else if (token == ynumber || token == yident || token == yunknown) {
			text = yytext;
		}
		printf("%d %s %s\n", token, token_name(token), text);
	}
}

static void print_comment(const char *comment)
{
	size_t comment_len = strlen(comment);

	memset(buffer, 0, sizeof buffer);
	strcpy(buffer, "/*");
	memcpy(buffer+2, yytext + comment_len, yyleng - comment_len * 2);
	strcat(buffer, "*/");
	printf("%s\n", buffer);
}


