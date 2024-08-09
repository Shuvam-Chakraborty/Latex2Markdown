%{
#include "y.tab.h"
%}

%option noyywrap

%%

\\documentclass[^\n]+ {
    yylval.str = strdup(yytext);
    return DOCCLASS;
}

\\usepackage[^\n]+ {
    yylval.str = strdup(yytext);
    return USP;
}

\\title[^\n]+ {
    yylval.str = strdup(yytext);
    return TITLE;
}

\\author[^\n]+ {
    yylval.str = strdup(yytext);
    return AUTHOR;
}

\\date[^\n]+ {
    yylval.str = strdup(yytext);
    return DATE;
}

\\begin\{document\} {
    yylval.str = strdup(yytext);
    return BEGINDOC;
}

\\end\{document\} {
    yylval.str = strdup(yytext);
    return ENDDOC;
}

\\section\{[^\}]*\} {
    yylval.str = strdup(yytext + 9);
    yylval.str[strlen(yylval.str) - 1] = '\0';
    return SECTION;
}

\\subsection\{[^\}]*\} {
    yylval.str = strdup(yytext + 12);
    yylval.str[strlen(yylval.str) - 1] = '\0';
    return SUBSECTION;
}

\\subsubsection\{[^\}]*\} {
    yylval.str = strdup(yytext + 15);
    yylval.str[strlen(yylval.str) - 1] = '\0';
    return SUBSUBSECTION;
}

\\hrule {
    yylval.str = "---";
    return HRULE;
}

\\includegraphics[^\{]*\{[^\}]*\} {
    yylval.str = strdup(yytext);
    return INCGRAPHICS;
}

\\textbf\{[^\}]*\} {
    yylval.str = strdup(yytext + 8);
    yylval.str[strlen(yylval.str) - 1] = '\0';
    return BOLD;
}

\\textit\{[^\}]*\} {
    yylval.str = strdup(yytext + 8);
    yylval.str[strlen(yylval.str) - 1] = '\0';
    return ITALIC;
}

\\begin\{itemize\} {
    yylval.str = strdup(yytext);
    return BEGIN_ITEMIZE;
}

\\end\{itemize\} {
    yylval.str = strdup(yytext);
    return END_ITEMIZE;
}

\\begin\{verbatim\} {
    yylval.str = strdup(yytext);
    return BEGIN_VERBATIM;
}

\\end\{verbatim\} {
    yylval.str = strdup(yytext);
    return END_VERBATIM;
}

\\begin\{enumerate\} {
    yylval.str = strdup(yytext);
    return BEGIN_ENUMERATE;
}

\\end\{enumerate\} {
    yylval.str = strdup(yytext);
    return END_ENUMERATE;
}

\\begin\{tabular\}[^\n]+ {
    yylval.str = strdup(yytext);
    return BEGIN_TABULAR;
}

\\end\{tabular\} {
    yylval.str = strdup(yytext);
    return END_TABULAR;
}

\\hline {
    yylval.str = strdup(yytext);
    return HLINE;
}

[^\n]+ {
    yylval.str = strdup(yytext);
    return TEXT;
}

\n+ {
    yylval.str = strdup(yytext);
    return NEWLINE;
}

%%


