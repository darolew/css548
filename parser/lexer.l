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
#include "y.tab.h"

#define BUF_SIZE (1024*4)

/* 
 * If EOF encountered in the middle of a comment, set this flag so the main 
 * method can terminate properly.
 */
static bool eof;

/* 
 * Character buffer for comments and strings.
 */
static char buffer[BUF_SIZE];

/* 
 * Capture a token's attribute value so it can be used  by the parser.
 */
void capture();

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
{ID}			{/*fprintf(stderr, "**ID**\n");*/ capture(); return yident;}
{BCOMMENT}		{}
{NUMBER}		{/*fprintf(stderr, "**NUMBER**\n");*/ capture(); return ynumber;}
{STRING}		{/*fprintf(stderr, "**STRING**\n");*/ capture(); return ystring;}
<INITIAL>\(\*		{BEGIN(IN_COMMENT); yymore();}
<IN_COMMENT><<EOF>>	{eof = true; return yunknown;}
<IN_COMMENT>[^*]*\*+	{BEGIN(WARNING); yymore();}
<WARNING><<EOF>>	{eof = true; return yunknown;}
<WARNING>[^)]		{BEGIN(IN_COMMENT); yymore();}
<WARNING>\)		{BEGIN(INITIAL); yymore(); }
{WHITESPACE}		{ /* Do nothing */ }
.			{return yunknown;}

%%

/* 
 * Capture a token's attribute value so it can be used  by the parser.
 */
void capture() {

  /*Copy yytext because it can be changed bcefore we use it.*/
  yylval.str = strdup(yytext);
  /*fprintf(stderr, "%s\n", yylval.str);*/
  
}
