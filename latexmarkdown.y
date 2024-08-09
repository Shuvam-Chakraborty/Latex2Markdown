%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void yyerror(const char *s);
int yylex();

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

void print_list() {
    Node *current = head;
    while (current) {
        printf("%s", current->str);
        Node *temp = current;
        current = current->next;
        free(temp->str);
        free(temp);
    }
    head = tail = NULL;
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
    const char *replace = "\n\t";
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

%}

%union {
    char *str;
}

%token <str> SECTION SUBSECTION SUBSUBSECTION TEXT NEWLINE ITALIC BOLD DOCCLASS USP TITLE AUTHOR DATE BEGINDOC ENDDOC  INCGRAPHICS HRULE

%%

document:
    preamble body
    ;

preamble:
    documentclass usepackage title author date
    ;

documentclass:
    | DOCCLASS NEWLINE { }
    ;

usepackage:
    | USP NEWLINE { } usepackage
    ;

title:
    | TITLE NEWLINE { }
    ;

author:
    | AUTHOR NEWLINE { }
    ;

date:
    | DATE NEWLINE { }
    ;

body:
    begindocument sections enddocument
    ;

begindocument:
    BEGINDOC NEWLINE { add_to_list("\n"); }
    ;

enddocument:
    ENDDOC NEWLINE { add_to_list("\n"); }
    ;

sections:
    | section subsections sections
    ;

section:
    SECTION NEWLINE {
        add_to_list("# ");
        add_to_list($1);
        add_to_list($2);
    }
    ;

subsections:
    | subsection subsubsections subsections
    ;

subsection:
   SUBSECTION NEWLINE {
        add_to_list("## ");
        add_to_list($1);
        add_to_list($2);
   }
   ;

subsubsections:
    | subsubsection contents subsubsections
    ;

subsubsection:
   SUBSUBSECTION NEWLINE {
        add_to_list("### ");
        add_to_list($1);
        add_to_list($2);
   }
   ;

contents:
    | bold contents
    | italic contents
    | hrule contents
    | graphics contents
    | paragraph contents

bold:
    BOLD NEWLINE {
	add_to_list("**");
        add_to_list($1);
        add_to_list("**");
	add_to_list($2);
    }
    ;

italic:
    ITALIC NEWLINE {
        add_to_list("*");
	add_to_list($1);
	add_to_list("*");
	add_to_list($2);
    }
    ;

hrule:
     HRULE NEWLINE {
	add_to_list($1);
	add_to_list($2);
     }
     ;

graphics:
     INCGRAPHICS NEWLINE {
	add_to_list("![IIT Delhi Campus](");
	handle_graphics($1);
        add_to_list(")");
	add_to_list($2);
     }
     ;

paragraph:
    TEXT NEWLINE {
        handle_para($1);
        add_to_list($2);
    }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main() {
    yyparse();
    print_list();
    return 0;
}

