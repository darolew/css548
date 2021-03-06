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
#include <ctype.h>
#include "y.tab.h"
#include "main.h"

#define BUF_SIZE (1024*4)

/* Method declarations */
static void print_comment(const char*);
static void patch_comment(char*);
static void die(const char*);

/* Capture a token's attribute value so it can be used by the parser. */
static void capture(bool);

/* 
 * If EOF encountered in the middle of a comment, set this flag so the main 
 * method can terminate properly.
 */
static bool eof;

/* Character buffer for comments and strings. */
static char buffer[BUF_SIZE];


%}

LETTER      [a-z]
DIGIT       [0-9]
ID          {LETTER}({LETTER}|{DIGIT})*
BCOMMENT    \{[^}]*\}
INT         {DIGIT}+
EXP         [Ee][+-]?{INT}
REAL        {INT}\.{INT}{EXP}?
STRINGDQ    \"[^"]*\"
STRINGSQ    \'[^']*\'
STRING      {STRINGDQ}|{STRINGSQ}
NEWLINE     \n
WHITESPACE  [ \t\r]

%x IN_COMMENT
%x WARNING

%%

and                     {return yand;}
array                   {return yarray;}
":="                    {return yassign;}
begin                   {return ybegin;}
"^"                     {return ycaret;}
case                    {return ycase;}
":"                     {return ycolon;}
","                     {return ycomma;}
const                   {return yconst;}
dispose                 {return ydispose;}
div                     {return ydiv;}
"/"                     {return ydivide;}
do                      {return ydo;}
"."                     {return ydot;}
".."                    {return ydotdot;}
downto                  {return ydownto;}
else                    {return yelse;}
end                     {return yend;}
"="                     {return yequal;}
file                    {return yunknown;}      /* Unsupported keyword */
for                     {return yfor;}
function                {return yfunction;}
goto                    {return yunknown;}      /* Unsupported keyword */
">"                     {return ygreater;}
">="                    {return ygreaterequal;}
if                      {return yif;}
in                      {return yin;}
label                   {return yunknown;}      /* Unsupported keyword */
"["                     {return yleftbracket;}
"("                     {return yleftparen;}
"<"                     {return yless;}
"<="                    {return ylessequal;}
"-"                     {return yminus;}
mod                     {return ymod;}
"*"                     {return ymultiply;}
new                     {return ynew;}
nil                     {return ynil;}
not                     {return ynot;}
"<>"                    {return ynotequal;}
of                      {return yof;}
or                      {return yor;}
packed                  {return yunknown;}      /* Unsupported keyword */
"+"                     {return yplus;}
procedure               {return yprocedure;}
program                 {return yprogram;}
record                  {return yrecord;}
repeat                  {return yrepeat;}
"]"                     {return yrightbracket;}
")"                     {return yrightparen;}
";"                     {return ysemicolon;}
set                     {return yset;}
then                    {return ythen;}
to                      {return yto;}
type                    {return ytype;}
until                   {return yuntil;}
var                     {return yvar;}
while                   {return ywhile;}
with                    {return yunknown;}      /* Unsupported keyword */
{ID}                    {capture(false); return yident;}
{BCOMMENT}              {print_comment("{");}
{INT}                   {capture(false); return yinteger;}
{REAL}                  {capture(false); return yreal;}
{STRING}                {capture(true); return ystring;}
<INITIAL>\(\*           {BEGIN(IN_COMMENT); yymore();}
<IN_COMMENT><<EOF>>     {eof = true; return yunknown;}
<IN_COMMENT>[^*]*\*+    {BEGIN(WARNING); yymore();}
<WARNING><<EOF>>        {eof = true; return yunknown;}
<WARNING>[^)]           {BEGIN(IN_COMMENT); yymore();}
<WARNING>\)             {BEGIN(INITIAL); yymore(); print_comment("(*");}
{NEWLINE}               {/* Do nothing */}
{WHITESPACE}            {/* Do nothing */}
.                       {return yunknown;}

%%

/* 
 * Capture a token's attribute value so it can be used  by the parser.
 */
static void capture(bool preserveCase)
{
    /* Copy yytext because it can be changed before we use it. */
    if (yytext[0] == '\'' || yytext[0] == '"') {
        //This is a string and not a number or identifier
        yylval.str = strdup(yytext+1);
        yylval.str[yyleng-2] = '\0';
    } else
        yylval.str = strdup(yytext);

    if (!preserveCase) {
        /* Convert string to lower case */
        char *ptr = yylval.str;
        for (; *ptr; ++ptr)
            *ptr = tolower(*ptr);
    }
    
    /*fprintf(stderr, "[yylval] %s\n", yylval.str);*/
}

/*
 * Print out a Pascal comment (stored in yytext) as a C comment.
 * @comment_delim - The delimiter for this type of Pascal comment: '{' or '(*' 
 */
static void print_comment(const char *comment_delim)
{
    size_t delim_len = strlen(comment_delim);
    size_t total_len;
    
    /* Test the comment does not overflow buffer. */
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
    printf("%s%s", buffer, nlindent().c_str());
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
