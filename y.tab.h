/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    SECTION = 258,                 /* SECTION  */
    SUBSECTION = 259,              /* SUBSECTION  */
    SUBSUBSECTION = 260,           /* SUBSUBSECTION  */
    TEXT = 261,                    /* TEXT  */
    NEWLINE = 262,                 /* NEWLINE  */
    ITALIC = 263,                  /* ITALIC  */
    BOLD = 264,                    /* BOLD  */
    DOCCLASS = 265,                /* DOCCLASS  */
    USP = 266,                     /* USP  */
    TITLE = 267,                   /* TITLE  */
    BEGIN_VERBATIM = 268,          /* BEGIN_VERBATIM  */
    END_VERBATIM = 269,            /* END_VERBATIM  */
    BEGIN_ITEMIZE = 270,           /* BEGIN_ITEMIZE  */
    END_ITEMIZE = 271,             /* END_ITEMIZE  */
    BEGIN_ENUMERATE = 272,         /* BEGIN_ENUMERATE  */
    END_ENUMERATE = 273,           /* END_ENUMERATE  */
    BEGIN_TABULAR = 274,           /* BEGIN_TABULAR  */
    END_TABULAR = 275,             /* END_TABULAR  */
    HLINE = 276,                   /* HLINE  */
    AUTHOR = 277,                  /* AUTHOR  */
    DATE = 278,                    /* DATE  */
    BEGINDOC = 279,                /* BEGINDOC  */
    ENDDOC = 280,                  /* ENDDOC  */
    INCGRAPHICS = 281,             /* INCGRAPHICS  */
    HRULE = 282                    /* HRULE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define SECTION 258
#define SUBSECTION 259
#define SUBSUBSECTION 260
#define TEXT 261
#define NEWLINE 262
#define ITALIC 263
#define BOLD 264
#define DOCCLASS 265
#define USP 266
#define TITLE 267
#define BEGIN_VERBATIM 268
#define END_VERBATIM 269
#define BEGIN_ITEMIZE 270
#define END_ITEMIZE 271
#define BEGIN_ENUMERATE 272
#define END_ENUMERATE 273
#define BEGIN_TABULAR 274
#define END_TABULAR 275
#define HLINE 276
#define AUTHOR 277
#define DATE 278
#define BEGINDOC 279
#define ENDDOC 280
#define INCGRAPHICS 281
#define HRULE 282

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 299 "latexmarkdown.y"

    char *str;

#line 125 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
