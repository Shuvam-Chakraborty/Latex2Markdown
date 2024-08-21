#ifndef LATEXMARKDOWN_H
#define LATEXMARKDOWN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct astnode {
    char *token;
    char *data;
    int tabs;
    int unvisited;
    struct astnode *children[30];
    int child_count;
} astnode;

astnode* createNode(char*, char*, int );
void addChild(astnode*, astnode*);
int shouldSkip(const char*);
void printAST(astnode*, FILE*);
void freeAST(astnode*);
void handle_graphics(char*, FILE*);
void handle_href(char*, FILE*);
void handle_para(char*, FILE*);
void process_item(char*, FILE*);
void enumerate_list(int, FILE*);
void count_column(char*);
void process_table_row(char*, FILE*);
void add_table_separator(int, FILE*);
void createMarkdown(astnode*, FILE*);
void yyerror(const char*);
int yylex(void);

#endif /* LATEXMARKDOWN_H */
