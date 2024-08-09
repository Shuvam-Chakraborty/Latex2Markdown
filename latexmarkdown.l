%{
#include "y.tab.h"
%}

%option noyywrap

%%

\\documentclass[^\n]+      { yylval.str = strdup(yytext); return DOCCLASS; }

\\usepackage[^\n]+         { yylval.str = strdup(yytext); return USP; }

\\title[^\n]+              { yylval.str = strdup(yytext); return TITLE; }

\\author[^\n]+             {  yylval.str = strdup(yytext); return AUTHOR; }

\\date[^\n]+               { yylval.str = strdup(yytext); return DATE; }

\\begin\{document\}        { yylval.str = strdup(yytext); return BEGINDOC; }

\\end\{document\}          { yylval.str = strdup(yytext); return ENDDOC; }

\\section\{[^\}]*\} {
    yylval.str = strdup(yytext + 9);  // Skip the '\section{' part
    yylval.str[strlen(yylval.str) - 1] = '\0';  // Remove the trailing brace
    return SECTION;
}

\\subsection\{[^\}]*\} {
    yylval.str = strdup(yytext + 12);  // Skip the '\subsection{' part
    yylval.str[strlen(yylval.str) - 1] = '\0';  // Remove the trailing brace
    return SUBSECTION;
}

\\subsubsection\{[^\}]*\} {
    yylval.str = strdup(yytext + 15);  // Skip the '\subsubsection{' part
    yylval.str[strlen(yylval.str) - 1] = '\0';  // Remove the trailing brace
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
    yylval.str = strdup(yytext + 8);  // Skip the '\textbf{' part
    yylval.str[strlen(yylval.str) - 1] = '\0';  // Remove the trailing brace
    return BOLD;
}

\\textit\{[^\}]*\} {
    yylval.str = strdup(yytext + 8);  // Skip the '\textit{' part
    yylval.str[strlen(yylval.str) - 1] = '\0';  // Remove the trailing brace
    return ITALIC;
}

\\begin\{itemize\}      { yylval.str = strdup(yytext); return BEGIN_ITEMIZE; }

\\end\{itemize\}        { yylval.str = strdup(yytext); return END_ITEMIZE; }

\\begin\{verbatim\}     { yylval.str = strdup(yytext); return BEGIN_VERBATIM; }

\\end\{verbatim\}       { yylval.str = strdup(yytext); return END_VERBATIM; }

\\begin\{enumerate\}    { yylval.str = strdup(yytext); return BEGIN_ENUMERATE; }

\\end\{enumerate\}      { yylval.str = strdup(yytext); return END_ENUMERATE; }


[^\n]+ {
    yylval.str = strdup(yytext);
    return TEXT;
}

\n+ {
    yylval.str = strdup(yytext);
    return NEWLINE;
}

%%


