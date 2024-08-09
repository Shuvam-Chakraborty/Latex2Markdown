/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "latexmarkdown.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void yyerror(const char *s);
int yylex();

int item_number; // Counter for items in enumerate block
int num_of_cols; // Number of columns

typedef struct Node {
    char *str;
    struct Node *next;
} Node;

Node *head = NULL, *tail = NULL;

void add_to_list(char *str) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->str = strdup(str);
    new_node->next = NULL;
    if (tail) {
        tail->next = new_node;
        tail = new_node;
    } else {
        head = tail = new_node;
    }
}

// Function to save the list to an output.md file
void save_list_to_file() {
    FILE *file = fopen("output.md", "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file for writing\n");
        return;
    }

    struct Node *current = head; // Assuming head is the start of your list

    while (current != NULL) {
        fprintf(file, "%s", current->str);
        current = current->next;
    }

    fclose(file);
}

void handle_graphics(char *str) {
    char *start = strchr(str, '{');
    char *end = strchr(str, '}');
    if (start != NULL && end != NULL && start < end) {
        start++;
        int length = end - start;
        char content[length + 1];
        strncpy(content, start, length);
        content[length] = '\0';
        add_to_list(content);
    } else {
        printf("Invalid input string.\n");
    }
}

void handle_href(char* str) {
    char *left, *url, *text, *right;

    const char* href_start = strstr(str, "\\href{");
    if (href_start == NULL) {
        // No \href found, print the entire string as left
        left = strdup(str);
        url = text = right = NULL;
    }
    else {
        // Extract left part
        size_t left_len = href_start - str;
        left = (char*)malloc(left_len + 1);
        strncpy(left, str, left_len);
        left[left_len] = '\0';

        // Move the pointer past \href{
        href_start += 6;

        // Find the end of the URL
        const char* url_end = strstr(href_start, "}");
        size_t url_len = url_end - href_start;
        url = (char*)malloc(url_len + 1);
        strncpy(url, href_start, url_len);
        url[url_len] = '\0';

        // Move the pointer past the closing }
        const char* text_start = url_end + 2;

        // Find the end of the text
        const char* text_end = strstr(text_start, "}");
        size_t text_len = text_end - text_start;
        text = (char*)malloc(text_len + 1);
        strncpy(text, text_start, text_len);
        text[text_len] = '\0';

        // Extract right part
        const char* right_start = text_end + 1;
        right = strdup(right_start);
    }

    // Print the results
    add_to_list(left);
    add_to_list("[");
    add_to_list(text);
    add_to_list("]");
    add_to_list("(");
    add_to_list(url);
    add_to_list(")");
    add_to_list(right);
    

    // Free allocated memory
    free(left);
    free(url);
    free(text);
    free(right);
}

void check_and_handle_href(char* str) {
    if (strstr(str, "\\href{")) {
        handle_href(str);
    } else {
        add_to_list(str);
    }
}

void handle_para(char *str) {
    const char *search = "\\par";
    const char *replace = "\n\n";
    int search_len = strlen(search);
    int replace_len = strlen(replace);
    int count = 0;
    char *pos = str;
    while ((pos = strstr(pos, search)) != NULL) {
        count++;
        pos += search_len;
    }
    size_t new_len = strlen(str) + count * (replace_len - search_len) + 1;
    char *result = (char *)malloc(new_len);
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    char *current_pos = result;
    while ((pos = strstr(str, search)) != NULL) {
        size_t len = pos - str;
        memcpy(current_pos, str, len);
        current_pos += len;
        memcpy(current_pos, replace, replace_len);
        current_pos += replace_len;
        str = pos + search_len;
    }
    strcpy(current_pos, str);
    check_and_handle_href(result);
    free(result);
}

void enumerate_list(int item_number) {
    // Calculate the length required for the string (including the dot and null terminator)
    int length = snprintf(NULL, 0, "%d.", item_number) + 1;

    // Allocate memory dynamically for the string
    char *num_str = (char *)malloc(length * sizeof(char));

    if (num_str == NULL) {
        // Handle memory allocation failure
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    // Convert the number to a string and append a dot at the end
    snprintf(num_str, length, "%d. ", item_number);

    // Print the result
    add_to_list(num_str);

    // Free the allocated memory
    free(num_str);
}

void process_item(char *str) {
    // Step 1: Skip leading spaces
    while (isspace((unsigned char)*str) || (unsigned char)*str=='\t' ) {
        str++;
    }

    // Step 2: Skip the "\item" keyword
    if (strncmp(str, "\\item", 5) == 0) {
        str += 5;
    }

    // Step 3: Skip any spaces after "\item"
    while (isspace((unsigned char)*str)) {
        str++;
    }

    // Step 4: Copy the rest of the string into a new string
    char *result_str = strdup(str);  // strdup dynamically allocates and copies the string

    if (result_str == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    // Step 5: Print the resulting string
    add_to_list(result_str);

    // Step 6: Free the allocated memory
    free(result_str);
}

void count_c(char* str) {
    // Initialize the count to 0
    int count = 0;

    // Traverse the string
    while (*str != '\0') {
        // If the current character is 'c', increment the count
        if (*str == 'c') {
            count++;
        }
        // Move to the next character
        str++;
    }
    num_of_cols = count;
}

void process_table_row(char *str) {
    char result[1002] = ""; // Buffer for the final result
    int cnt = 50, i = 1;
    // Remove trailing \\ if present
    size_t len = strlen(str);
    if (len > 1 && str[len - 1] == '\\' && str[len - 2] == '\\') {
        str[len - 2] = '&';
	str[len - 1] = '\0'; // Remove the trailing "\\"
    }
    result[0] = '|';
    while(*str) {
	if((char)*str != '&') {
	    result[i]=(char)*str;
	    str++;
	    cnt--;
	    i++;
	}
	else {
	    while(cnt--) {
		result[i++]=' ';
	    }
	    cnt = 50;
	    result[i++] = '|';
	    str++;
	}
	result[i]='\0';
	
    }
    // Print the formatted row
    add_to_list(result);
}

void add_table_separator(int num_of_cols) {
    // Each column separator has 51 characters: "|" + 50 "-"
    int column_width = 51;
    int total_size = (column_width * num_of_cols) + 2; // Add 2 for the final "|" and the null terminator

    // Dynamically allocate memory for the result string
    char* result = (char*)malloc(total_size * sizeof(char));
    if (result == NULL) {
        printf("Memory allocation failed\n");
        return ;
    }

    // Initialize the result string
    result[0] = '\0';

    // Prepare the single column separator string
    char s[52];  // | + 50 dashes + nullchar
    strcpy(s, "|");
    memset(s + 1, '-', 50);  // Fill the next 50 characters with '-'
    s[51] = '\0';  // Null-terminate the string

    // Construct the full separator line
    for (int i = 0; i < num_of_cols; i++) {
        strcat(result, s);  // Append the separator string for each column
    }

    strcat(result, "|");  // End the row with a closing pipe

    add_to_list(result);  // Return the dynamically allocated string
    add_to_list("\n");
}


#line 369 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
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

#line 480 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_SECTION = 3,                    /* SECTION  */
  YYSYMBOL_SUBSECTION = 4,                 /* SUBSECTION  */
  YYSYMBOL_SUBSUBSECTION = 5,              /* SUBSUBSECTION  */
  YYSYMBOL_TEXT = 6,                       /* TEXT  */
  YYSYMBOL_NEWLINE = 7,                    /* NEWLINE  */
  YYSYMBOL_ITALIC = 8,                     /* ITALIC  */
  YYSYMBOL_BOLD = 9,                       /* BOLD  */
  YYSYMBOL_DOCCLASS = 10,                  /* DOCCLASS  */
  YYSYMBOL_USP = 11,                       /* USP  */
  YYSYMBOL_TITLE = 12,                     /* TITLE  */
  YYSYMBOL_BEGIN_VERBATIM = 13,            /* BEGIN_VERBATIM  */
  YYSYMBOL_END_VERBATIM = 14,              /* END_VERBATIM  */
  YYSYMBOL_BEGIN_ITEMIZE = 15,             /* BEGIN_ITEMIZE  */
  YYSYMBOL_END_ITEMIZE = 16,               /* END_ITEMIZE  */
  YYSYMBOL_BEGIN_ENUMERATE = 17,           /* BEGIN_ENUMERATE  */
  YYSYMBOL_END_ENUMERATE = 18,             /* END_ENUMERATE  */
  YYSYMBOL_BEGIN_TABULAR = 19,             /* BEGIN_TABULAR  */
  YYSYMBOL_END_TABULAR = 20,               /* END_TABULAR  */
  YYSYMBOL_HLINE = 21,                     /* HLINE  */
  YYSYMBOL_AUTHOR = 22,                    /* AUTHOR  */
  YYSYMBOL_DATE = 23,                      /* DATE  */
  YYSYMBOL_BEGINDOC = 24,                  /* BEGINDOC  */
  YYSYMBOL_ENDDOC = 25,                    /* ENDDOC  */
  YYSYMBOL_INCGRAPHICS = 26,               /* INCGRAPHICS  */
  YYSYMBOL_HRULE = 27,                     /* HRULE  */
  YYSYMBOL_YYACCEPT = 28,                  /* $accept  */
  YYSYMBOL_document = 29,                  /* document  */
  YYSYMBOL_preamble = 30,                  /* preamble  */
  YYSYMBOL_documentclass = 31,             /* documentclass  */
  YYSYMBOL_usepackage = 32,                /* usepackage  */
  YYSYMBOL_33_1 = 33,                      /* $@1  */
  YYSYMBOL_title = 34,                     /* title  */
  YYSYMBOL_author = 35,                    /* author  */
  YYSYMBOL_date = 36,                      /* date  */
  YYSYMBOL_body = 37,                      /* body  */
  YYSYMBOL_begindocument = 38,             /* begindocument  */
  YYSYMBOL_enddocument = 39,               /* enddocument  */
  YYSYMBOL_sections = 40,                  /* sections  */
  YYSYMBOL_section = 41,                   /* section  */
  YYSYMBOL_subsections = 42,               /* subsections  */
  YYSYMBOL_subsection = 43,                /* subsection  */
  YYSYMBOL_subsubsections = 44,            /* subsubsections  */
  YYSYMBOL_subsubsection = 45,             /* subsubsection  */
  YYSYMBOL_contents = 46,                  /* contents  */
  YYSYMBOL_47_2 = 47,                      /* $@2  */
  YYSYMBOL_48_3 = 48,                      /* $@3  */
  YYSYMBOL_49_4 = 49,                      /* $@4  */
  YYSYMBOL_50_5 = 50,                      /* $@5  */
  YYSYMBOL_51_6 = 51,                      /* $@6  */
  YYSYMBOL_52_7 = 52,                      /* $@7  */
  YYSYMBOL_53_8 = 53,                      /* $@8  */
  YYSYMBOL_table_head = 54,                /* table_head  */
  YYSYMBOL_table_body = 55,                /* table_body  */
  YYSYMBOL_56_9 = 56,                      /* $@9  */
  YYSYMBOL_block_verbatim = 57,            /* block_verbatim  */
  YYSYMBOL_58_10 = 58,                     /* $@10  */
  YYSYMBOL_block_itemize = 59,             /* block_itemize  */
  YYSYMBOL_60_11 = 60,                     /* $@11  */
  YYSYMBOL_block_enumerate = 61,           /* block_enumerate  */
  YYSYMBOL_62_12 = 62,                     /* $@12  */
  YYSYMBOL_bold = 63,                      /* bold  */
  YYSYMBOL_italic = 64,                    /* italic  */
  YYSYMBOL_hrule = 65,                     /* hrule  */
  YYSYMBOL_graphics = 66,                  /* graphics  */
  YYSYMBOL_paragraph = 67                  /* paragraph  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  6
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   112

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  28
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  40
/* YYNRULES -- Number of rules.  */
#define YYNRULES  61
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  120

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   282


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   308,   308,   312,   315,   316,   319,   320,   320,   323,
     324,   327,   328,   331,   332,   336,   340,   344,   347,   348,
     352,   359,   360,   364,   371,   372,   376,   383,   384,   385,
     386,   387,   388,   389,   389,   389,   390,   390,   390,   391,
     391,   391,   392,   392,   396,   403,   404,   404,   410,   411,
     411,   417,   418,   418,   425,   426,   426,   436,   445,   454,
     461,   470
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "SECTION",
  "SUBSECTION", "SUBSUBSECTION", "TEXT", "NEWLINE", "ITALIC", "BOLD",
  "DOCCLASS", "USP", "TITLE", "BEGIN_VERBATIM", "END_VERBATIM",
  "BEGIN_ITEMIZE", "END_ITEMIZE", "BEGIN_ENUMERATE", "END_ENUMERATE",
  "BEGIN_TABULAR", "END_TABULAR", "HLINE", "AUTHOR", "DATE", "BEGINDOC",
  "ENDDOC", "INCGRAPHICS", "HRULE", "$accept", "document", "preamble",
  "documentclass", "usepackage", "$@1", "title", "author", "date", "body",
  "begindocument", "enddocument", "sections", "section", "subsections",
  "subsection", "subsubsections", "subsubsection", "contents", "$@2",
  "$@3", "$@4", "$@5", "$@6", "$@7", "$@8", "table_head", "table_body",
  "$@9", "block_verbatim", "$@10", "block_itemize", "$@11",
  "block_enumerate", "$@12", "bold", "italic", "hrule", "graphics",
  "paragraph", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-52)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      -4,     2,    10,   -13,     4,   -52,   -52,     6,   -52,    14,
      12,     8,   -52,    15,    -2,    17,   -52,    20,     7,   -52,
      21,   -52,    23,    14,    19,     4,   -52,    24,     9,   -52,
     -52,   -52,    26,    17,    -1,   -52,   -52,    27,   -52,   -52,
     -52,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      19,    -1,    -1,    -1,    -1,    -1,   -52,   -52,   -52,   -52,
     -52,   -52,   -52,   -52,   -52,   -52,   -52,   -52,   -52,   -52,
     -52,   -52,    38,    39,    40,    37,    42,    41,    44,    43,
      46,    45,    47,   -52,    49,   -52,    50,   -52,    53,    55,
      38,   -52,    39,   -52,    40,   -52,    57,    48,   -52,    -1,
     -52,    -1,   -52,    -1,   -52,    58,   -52,   -52,   -52,    56,
      59,    51,   -52,    61,    56,    54,   -52,    63,    -1,   -52
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       4,     0,     0,     0,     6,     5,     1,     0,     2,    18,
       0,     9,    16,     0,     0,    21,     7,     0,    11,    20,
       0,    15,     0,    18,    24,     6,    10,     0,    13,    17,
      23,    19,     0,    21,    27,     8,    12,     0,     3,    26,
      22,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      24,    27,    27,    27,    27,    27,    14,    61,    58,    57,
      33,    36,    39,    42,    60,    59,    25,    28,    29,    30,
      31,    32,    48,    51,    54,     0,     0,     0,     0,     0,
       0,     0,     0,    49,     0,    52,     0,    55,     0,     0,
      48,    34,    51,    37,    54,    40,     0,     0,    50,    27,
      53,    27,    56,    27,    44,     0,    35,    38,    41,    45,
       0,     0,    46,     0,    45,     0,    47,     0,    27,    43
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -52,   -52,   -52,   -52,    22,   -52,   -52,   -52,   -52,   -52,
     -52,   -52,    52,   -52,    60,   -52,    62,   -52,   -51,   -52,
     -52,   -52,   -52,   -52,   -52,   -52,   -52,   -43,   -52,   -17,
     -52,   -16,   -52,   -15,   -52,   -52,   -52,   -52,   -52,   -52
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     2,     3,     4,    11,    25,    18,    28,    38,     8,
       9,    21,    14,    15,    23,    24,    33,    34,    50,    72,
      99,    73,   101,    74,   103,    75,    97,   111,   114,    77,
      90,    79,    92,    81,    94,    51,    52,    53,    54,    55
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      67,    68,    69,    70,    71,    41,     1,    42,    43,     5,
       6,     7,    44,    12,    45,    10,    46,    13,    47,    16,
      17,    22,    19,    20,    32,    48,    49,    26,    29,    27,
      30,    36,    37,    39,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    76,    78,    80,    35,   106,    83,
     107,    85,   108,    87,    89,    84,    91,    93,    82,    86,
      95,    96,   110,    88,   104,   109,   112,   119,   115,   105,
     118,   116,   113,    98,   117,    31,   100,     0,     0,   102,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    66
};

static const yytype_int8 yycheck[] =
{
      51,    52,    53,    54,    55,     6,    10,     8,     9,     7,
       0,    24,    13,     7,    15,    11,    17,     3,    19,     7,
      12,     4,     7,    25,     5,    26,    27,     7,     7,    22,
       7,     7,    23,     7,     7,     7,     7,     7,     7,     7,
       7,     7,     7,     7,     6,     6,     6,    25,    99,     7,
     101,     7,   103,     7,     7,    14,     7,     7,    21,    16,
       7,     6,     6,    18,     7,     7,     7,   118,     7,    21,
       7,   114,    21,    90,    20,    23,    92,    -1,    -1,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    50
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    10,    29,    30,    31,     7,     0,    24,    37,    38,
      11,    32,     7,     3,    40,    41,     7,    12,    34,     7,
      25,    39,     4,    42,    43,    33,     7,    22,    35,     7,
       7,    40,     5,    44,    45,    32,     7,    23,    36,     7,
      42,     6,     8,     9,    13,    15,    17,    19,    26,    27,
      46,    63,    64,    65,    66,    67,     7,     7,     7,     7,
       7,     7,     7,     7,     7,     7,    44,    46,    46,    46,
      46,    46,    47,    49,    51,    53,     6,    57,     6,    59,
       6,    61,    21,     7,    14,     7,    16,     7,    18,     7,
      58,     7,    60,     7,    62,     7,     6,    54,    57,    48,
      59,    50,    61,    52,     7,    21,    46,    46,    46,     7,
       6,    55,     7,    21,    56,     7,    55,    20,     7,    46
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    28,    29,    30,    31,    31,    32,    33,    32,    34,
      34,    35,    35,    36,    36,    37,    38,    39,    40,    40,
      41,    42,    42,    43,    44,    44,    45,    46,    46,    46,
      46,    46,    46,    47,    48,    46,    49,    50,    46,    51,
      52,    46,    53,    46,    54,    55,    56,    55,    57,    58,
      57,    59,    60,    59,    61,    62,    61,    63,    64,    65,
      66,    67
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     5,     0,     2,     0,     0,     4,     0,
       2,     0,     2,     0,     2,     3,     2,     2,     0,     3,
       2,     0,     3,     2,     0,     3,     2,     0,     2,     2,
       2,     2,     2,     0,     0,     8,     0,     0,     8,     0,
       0,     8,     0,    14,     2,     0,     0,     4,     0,     0,
       4,     0,     0,     4,     0,     0,     4,     2,     2,     2,
       2,     2
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 5: /* documentclass: DOCCLASS NEWLINE  */
#line 316 "latexmarkdown.y"
                       { }
#line 1605 "y.tab.c"
    break;

  case 7: /* $@1: %empty  */
#line 320 "latexmarkdown.y"
                  { }
#line 1611 "y.tab.c"
    break;

  case 10: /* title: TITLE NEWLINE  */
#line 324 "latexmarkdown.y"
                    { }
#line 1617 "y.tab.c"
    break;

  case 12: /* author: AUTHOR NEWLINE  */
#line 328 "latexmarkdown.y"
                     { }
#line 1623 "y.tab.c"
    break;

  case 14: /* date: DATE NEWLINE  */
#line 332 "latexmarkdown.y"
                   { }
#line 1629 "y.tab.c"
    break;

  case 16: /* begindocument: BEGINDOC NEWLINE  */
#line 340 "latexmarkdown.y"
                     { add_to_list("\n"); }
#line 1635 "y.tab.c"
    break;

  case 17: /* enddocument: ENDDOC NEWLINE  */
#line 344 "latexmarkdown.y"
                   { add_to_list("\n"); }
#line 1641 "y.tab.c"
    break;

  case 20: /* section: SECTION NEWLINE  */
#line 352 "latexmarkdown.y"
                    {
        add_to_list("# ");
        add_to_list((yyvsp[-1].str));
        add_to_list((yyvsp[0].str));
    }
#line 1651 "y.tab.c"
    break;

  case 23: /* subsection: SUBSECTION NEWLINE  */
#line 364 "latexmarkdown.y"
                      {
        add_to_list("## ");
        add_to_list((yyvsp[-1].str));
        add_to_list((yyvsp[0].str));
   }
#line 1661 "y.tab.c"
    break;

  case 26: /* subsubsection: SUBSUBSECTION NEWLINE  */
#line 376 "latexmarkdown.y"
                         {
        add_to_list("### ");
        add_to_list((yyvsp[-1].str));
        add_to_list((yyvsp[0].str));
   }
#line 1671 "y.tab.c"
    break;

  case 33: /* $@2: %empty  */
#line 389 "latexmarkdown.y"
                             { add_to_list("```python"); add_to_list((yyvsp[0].str)); }
#line 1677 "y.tab.c"
    break;

  case 34: /* $@3: %empty  */
#line 389 "latexmarkdown.y"
                                                                                                                { add_to_list("```\n"); add_to_list((yyvsp[-4].str)); }
#line 1683 "y.tab.c"
    break;

  case 36: /* $@4: %empty  */
#line 390 "latexmarkdown.y"
                            { add_to_list((yyvsp[0].str)); }
#line 1689 "y.tab.c"
    break;

  case 37: /* $@5: %empty  */
#line 390 "latexmarkdown.y"
                                                                                   { add_to_list((yyvsp[-4].str)); }
#line 1695 "y.tab.c"
    break;

  case 39: /* $@6: %empty  */
#line 391 "latexmarkdown.y"
                              { item_number=1; add_to_list((yyvsp[0].str)); }
#line 1701 "y.tab.c"
    break;

  case 40: /* $@7: %empty  */
#line 391 "latexmarkdown.y"
                                                                                                        { add_to_list((yyvsp[-4].str)); }
#line 1707 "y.tab.c"
    break;

  case 42: /* $@8: %empty  */
#line 392 "latexmarkdown.y"
                            { count_c((yyvsp[-1].str)); }
#line 1713 "y.tab.c"
    break;

  case 44: /* table_head: TEXT NEWLINE  */
#line 396 "latexmarkdown.y"
                 {
	process_table_row((yyvsp[-1].str));
	add_to_list((yyvsp[0].str));
	add_table_separator(num_of_cols);
    }
#line 1723 "y.tab.c"
    break;

  case 46: /* $@9: %empty  */
#line 404 "latexmarkdown.y"
                   {
	process_table_row((yyvsp[-1].str));
	add_to_list((yyvsp[0].str));
    }
#line 1732 "y.tab.c"
    break;

  case 49: /* $@10: %empty  */
#line 411 "latexmarkdown.y"
                   {
	add_to_list((yyvsp[-1].str));
	add_to_list((yyvsp[0].str));
    }
#line 1741 "y.tab.c"
    break;

  case 52: /* $@11: %empty  */
#line 418 "latexmarkdown.y"
                   {
	add_to_list("- ");
        process_item((yyvsp[-1].str));
        add_to_list((yyvsp[0].str));
    }
#line 1751 "y.tab.c"
    break;

  case 55: /* $@12: %empty  */
#line 426 "latexmarkdown.y"
                   {
	enumerate_list(item_number);
	item_number++;
        add_to_list(" ");
	process_item((yyvsp[-1].str));
        add_to_list((yyvsp[0].str));
    }
#line 1763 "y.tab.c"
    break;

  case 57: /* bold: BOLD NEWLINE  */
#line 436 "latexmarkdown.y"
                 {
	add_to_list("**");
        add_to_list((yyvsp[-1].str));
        add_to_list("**");
	add_to_list((yyvsp[0].str));
    }
#line 1774 "y.tab.c"
    break;

  case 58: /* italic: ITALIC NEWLINE  */
#line 445 "latexmarkdown.y"
                   {
        add_to_list("*");
	add_to_list((yyvsp[-1].str));
	add_to_list("*");
	add_to_list((yyvsp[0].str));
    }
#line 1785 "y.tab.c"
    break;

  case 59: /* hrule: HRULE NEWLINE  */
#line 454 "latexmarkdown.y"
                   {
	add_to_list((yyvsp[-1].str));
	add_to_list((yyvsp[0].str));
     }
#line 1794 "y.tab.c"
    break;

  case 60: /* graphics: INCGRAPHICS NEWLINE  */
#line 461 "latexmarkdown.y"
                         {
	add_to_list("![IIT Delhi Campus](");
	handle_graphics((yyvsp[-1].str));
        add_to_list(")");
	add_to_list((yyvsp[0].str));
     }
#line 1805 "y.tab.c"
    break;

  case 61: /* paragraph: TEXT NEWLINE  */
#line 470 "latexmarkdown.y"
                 {
        handle_para((yyvsp[-1].str));
        add_to_list((yyvsp[0].str));
    }
#line 1814 "y.tab.c"
    break;


#line 1818 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 476 "latexmarkdown.y"


void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main() {
    yyparse();
    save_list_to_file();  // Save the list to output.md
    return 0;
}

