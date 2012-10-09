/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     yand = 258,
     yarray = 259,
     yassign = 260,
     ybegin = 261,
     ycaret = 262,
     ycase = 263,
     ycolon = 264,
     ycomma = 265,
     yconst = 266,
     ydispose = 267,
     ydiv = 268,
     ydivide = 269,
     ydo = 270,
     ydot = 271,
     ydotdot = 272,
     ydownto = 273,
     yelse = 274,
     yend = 275,
     yequal = 276,
     yfalse = 277,
     yfor = 278,
     yfunction = 279,
     ygreater = 280,
     ygreaterequal = 281,
     yident = 282,
     yif = 283,
     yin = 284,
     yleftbracket = 285,
     yleftparen = 286,
     yless = 287,
     ylessequal = 288,
     yminus = 289,
     ymod = 290,
     ymultiply = 291,
     ynew = 292,
     ynil = 293,
     ynot = 294,
     ynotequal = 295,
     ynumber = 296,
     yof = 297,
     yor = 298,
     yplus = 299,
     yprocedure = 300,
     yprogram = 301,
     yread = 302,
     yreadln = 303,
     yrecord = 304,
     yrepeat = 305,
     yrightbracket = 306,
     yrightparen = 307,
     ysemicolon = 308,
     yset = 309,
     ystring = 310,
     ythen = 311,
     yto = 312,
     ytrue = 313,
     ytype = 314,
     yuntil = 315,
     yvar = 316,
     ywhile = 317,
     ywrite = 318,
     ywriteln = 319,
     yunknown = 320
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


