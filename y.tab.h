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
    DOCCLASS = 258,                /* DOCCLASS  */
    USP = 259,                     /* USP  */
    TITLE = 260,                   /* TITLE  */
    AUTHOR = 261,                  /* AUTHOR  */
    DATE = 262,                    /* DATE  */
    BEGINDOC = 263,                /* BEGINDOC  */
    ENDDOC = 264,                  /* ENDDOC  */
    SECTION = 265,                 /* SECTION  */
    SUBSECTION = 266,              /* SUBSECTION  */
    SUBSUBSECTION = 267,           /* SUBSUBSECTION  */
    BOLD = 268,                    /* BOLD  */
    ITALIC = 269,                  /* ITALIC  */
    HRULE = 270,                   /* HRULE  */
    INCGRAPHICS = 271,             /* INCGRAPHICS  */
    TEXT = 272,                    /* TEXT  */
    NEWLINE = 273,                 /* NEWLINE  */
    BEGIN_VERBATIM = 274,          /* BEGIN_VERBATIM  */
    END_VERBATIM = 275,            /* END_VERBATIM  */
    BEGIN_ITEMIZE = 276,           /* BEGIN_ITEMIZE  */
    END_ITEMIZE = 277,             /* END_ITEMIZE  */
    BEGIN_ENUMERATE = 278,         /* BEGIN_ENUMERATE  */
    END_ENUMERATE = 279,           /* END_ENUMERATE  */
    BEGIN_TABULAR = 280,           /* BEGIN_TABULAR  */
    END_TABULAR = 281,             /* END_TABULAR  */
    HLINE = 282                    /* HLINE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define DOCCLASS 258
#define USP 259
#define TITLE 260
#define AUTHOR 261
#define DATE 262
#define BEGINDOC 263
#define ENDDOC 264
#define SECTION 265
#define SUBSECTION 266
#define SUBSUBSECTION 267
#define BOLD 268
#define ITALIC 269
#define HRULE 270
#define INCGRAPHICS 271
#define TEXT 272
#define NEWLINE 273
#define BEGIN_VERBATIM 274
#define END_VERBATIM 275
#define BEGIN_ITEMIZE 276
#define END_ITEMIZE 277
#define BEGIN_ENUMERATE 278
#define END_ENUMERATE 279
#define BEGIN_TABULAR 280
#define END_TABULAR 281
#define HLINE 282

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 324 "latexmarkdown.y"

    char *str;
    struct astnode *node;

#line 126 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
