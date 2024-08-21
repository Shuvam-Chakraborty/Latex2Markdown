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

int item_number;
int num_of_cols;

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

astnode* createNode(char *token, char *data, int tabs) {
    astnode *node = (astnode*)malloc(sizeof(astnode));
    if (node == NULL) {
        fprintf(stderr, "Error: memory allocation failed\n");
        exit(1);
    }
    node->token = strdup(token);
    node->data = strdup(data);
    node->tabs = tabs;
    node->unvisited = 1;
    node->child_count = 0;
    for (int i = 0; i < 30; i++) {
        node->children[i] = NULL;
    }
    return node;
}

void addChild(astnode *parent, astnode *child) {
    if (parent == NULL) {
        fprintf(stderr, "Error: null parent pointer\n");
        return;
    }
    if (child == NULL) return;
    if (parent->child_count < 30) {
        parent->children[parent->child_count] = child;
        parent->child_count++;
    } else {
        fprintf(stderr, "Warning: Node '%s' exceeded maximum children limit of 30.\n", parent->token);
    }
}

int shouldSkip(const char *token) {
    const char *skipList[] = { "usepac_list", "sections_list", "subsections_list", "subsubsections_list", "content","content_list", "block_verbatim", "block_itemize", "block_enumerate", "table_row", "newline"};
    int skipCount = sizeof(skipList) / sizeof(skipList[0]);
    for (int i = 0; i < skipCount; i++) {
        if (strcmp(token, skipList[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void printAST(astnode *node, FILE *file) {
    if (node == NULL) return;
    if (!shouldSkip(node->token)) {
        for (int i = 0; i < node->tabs; i++) {
            fprintf(file, "  ");
        }
        fprintf(file, "%s: %s\n", node->token, node->data);
    }
    for (int i = 0; i < node->child_count; i++) {
        printAST(node->children[i], file);
    }
}

void freeAST(astnode *node) {
    if (node == NULL) return;
    for (int i = 0; i < node->child_count; i++) {
        freeAST(node->children[i]);
    }
    free(node);
}

void handle_graphics(char *str, FILE *file) {
    char *start = strchr(str, '{');
    char *end = strchr(str, '}');
    if (start != NULL && end != NULL && start < end) {
        start++;
        int length = end - start;
        char content[length + 1];
        strncpy(content, start, length);
        content[length] = '\0';
        fprintf(file, "%s", content);
    } else {
        printf("Invalid input string.\n");
    }
}

void handle_href(char *str, FILE *file) {
    if (!(strstr(str, "\\href{"))) {
        fprintf(file, "%s", str);
        return;
    }
    char *left, *url, *text, *right;
    const char* href_start = strstr(str, "\\href{");
    if (href_start == NULL) {
        left = strdup(str);
        url = text = right = NULL;
    } else {
        size_t left_len = href_start - str;
        left = (char*)malloc(left_len + 1);
        strncpy(left, str, left_len);
        left[left_len] = '\0';
        href_start += 6;
        const char* url_end = strstr(href_start, "}");
        size_t url_len = url_end - href_start;
        url = (char*)malloc(url_len + 1);
        strncpy(url, href_start, url_len);
        url[url_len] = '\0';
        const char* text_start = url_end + 2;
        const char* text_end = strstr(text_start, "}");
        size_t text_len = text_end - text_start;
        text = (char*)malloc(text_len + 1);
        strncpy(text, text_start, text_len);
        text[text_len] = '\0';
        const char* right_start = text_end + 1;
        right = strdup(right_start);
    }
    fprintf(file, "%s", left);
    fprintf(file, "[");
    fprintf(file,"%s", text);
    fprintf(file, "]");
    fprintf(file, "(");
    fprintf(file,"%s", url);
    fprintf(file, ")");
    fprintf(file,"%s", right);
    free(left);
    free(text);
    free(url);
    free(right);
}

void handle_para(char *str, FILE *file) {
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
    handle_href(result, file);
    free(result);
}

void process_item(char *str, FILE *file) {
    while (isspace((unsigned char)*str) || (unsigned char)*str == '\t') {
        str++;
    }
    if (strncmp(str, "\\item", 5) == 0) {
        str += 5;
    }
    while (isspace((unsigned char)*str)) {
        str++;
    }
    char *result_str = strdup(str);
    if (result_str == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    fprintf(file, "%s", result_str);
    free(result_str);
}

void enumerate_list(int item_number, FILE *file) {
    int length = snprintf(NULL, 0, "%d.", item_number) + 1;
    char *num_str = (char *)malloc(length * sizeof(char));
    if (num_str == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    snprintf(num_str, length, "%d. ", item_number);
    fprintf(file, "%s", num_str);
    free(num_str);
}

void count_column(char *str) {
    int count = 0;
    while (*str != '\0') {
        if (*str == '&') {
            count++;
        }
        str++;
    }
    num_of_cols = count + 1;
}

void process_table_row(char *str, FILE *file) {
    char result[1002] = "";
    int cnt = 49, i = 1;
    size_t len = strlen(str);
    if (len > 1 && str[len - 1] == '\\' && str[len - 2] == '\\') {
        str[len - 2] = '&';
        str[len - 1] = '\0';
    }
    result[0] = '|';
    while (*str) {
        if ((char)*str != '&') {
            result[i] = (char)*str;
            str++;
            cnt--;
            i++;
        } else {
            while (cnt--) {
                result[i++] = ' ';
            }
            cnt = 49;
            result[i++] = '|';
            str++;
        }
        result[i] = '\0';
    }
    fprintf(file, "%s", result);
}

void add_table_separator(int num_of_cols, FILE *file) {
    int column_width = 50;
    int total_size = (column_width * num_of_cols) + 2;
    char* result = (char*)malloc(total_size * sizeof(char));
    if (result == NULL) {
        printf("Memory allocation failed\n");
        return ;
    }
    result[0] = '\0';
    char s[51];
    strcpy(s, "|");
    memset(s + 1, '-', 49);
    s[50] = '\0';
    for (int i = 0; i < num_of_cols; i++) {
        strcat(result, s);
    }
    strcat(result, "|");
    fprintf(file, "%s", result);
    fprintf(file, "\n");
}

void createMarkdown(astnode *node, FILE *file) {
    if (node == NULL) return;
    node->unvisited = 0;
    if (strcmp(node->token, "newline") == 0) {
        fprintf(file, "%s", node->data);
    } else if (strcmp(node->token, "section") == 0) {
        fprintf(file, "# %s", node->data);
    } else if (strcmp(node->token, "subsection") == 0) {
        fprintf(file, "## %s", node->data);
    } else if (strcmp(node->token, "subsubsection") == 0) {
        fprintf(file, "### %s", node->data);
    } else if (strcmp(node->token, "bold_text") == 0) {
        fprintf(file, "**%s**", node->data);
    } else if (strcmp(node->token, "italic_text") == 0) {
        fprintf(file, "*%s*", node->data);
    } else if (strcmp(node->token, "hrule_text") == 0) {
        fprintf(file, "%s", node->data);
    } else if (strcmp(node->token, "graphics_text") == 0) {
        fprintf(file, "![IIT Delhi Campus](");
        handle_graphics(node->data, file);
        fprintf(file, ")");
    } else if (strcmp(node->token, "para_text") == 0) {
        handle_para(node->data, file);
    } else if (strcmp(node->token, "start_verbatim") == 0) {
        fprintf(file, "%s", node->data);
    } else if (strcmp(node->token, "end_verbatim") == 0) {
        fprintf(file, "%s", node->data);
    } else if (strcmp(node->token, "verbatim_text") == 0) {
        fprintf(file, "%s", node->data);
    } else if (strcmp(node->token, "itemize_text") == 0) {
        fprintf(file, "- ");
        process_item(node->data, file);
    } else if (strcmp(node->token, "enumerate_text") == 0) {
        enumerate_list(item_number, file);
        item_number++;
        process_item(node->data, file);
    } else if (strcmp(node->token, "enumerate") == 0) {
        item_number = 1;
    } else if (strcmp(node->token, "hline") == 0) {
        add_table_separator(num_of_cols, file);
    } else if (strcmp(node->token, "table_text") == 0) {
        count_column(node->data);
        process_table_row(node->data, file);
    }
    for (int i = 0; i < node->child_count; i++) {
       if(node->children[i] && node->children[i]->unvisited)
         createMarkdown(node->children[i], file);
    }
}

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

#endif /* LATEXMARKDOWN_H */
