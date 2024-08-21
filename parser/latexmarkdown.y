%{
#include "latexmarkdown.h"

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

struct astnode *root;
int item_number;
int num_of_cols;

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

%}

%union {
    char *str;
    struct astnode *node;
}

%token <str> DOCCLASS USP TITLE AUTHOR DATE BEGINDOC ENDDOC SECTION SUBSECTION SUBSUBSECTION BOLD ITALIC HRULE INCGRAPHICS TEXT NEWLINE BEGIN_VERBATIM END_VERBATIM BEGIN_ITEMIZE END_ITEMIZE BEGIN_ENUMERATE END_ENUMERATE BEGIN_TABULAR END_TABULAR HLINE

%type <node> document preamble documentclass usepac_list usepackage title author date body begindocument enddocument sections section subsections subsection subsubsections subsubsection content_list contents bold italic hrule graphics paragraph verbatim itemize enumerate table_content block_verbatim verbatim_body itemize_body block_itemize enumerate_body block_enumerate table_head table_body table_row

%start document

%%

document: preamble body
        {
             $$ = createNode("document", "", 0);
             root = $$;
             addChild($$, $1);
             addChild($$, $2);
        }
        ;

preamble: documentclass usepackage title author date
        {
             $$ = createNode("preamble", "", 1);
             addChild($$, $1);
             addChild($$, $2);
             addChild($$, $3);
             addChild($$, $4);
             addChild($$, $5);
        }
        ;

documentclass: DOCCLASS NEWLINE { $$ = createNode("documentclass", $1, 2); }
             | /* empty */ { $$ = createNode("documentclass", "", 2); }
             ;

usepackage: usepac_list
          {
              $$ = createNode("usepackage", "", 2);
              addChild($$, $1);
          }
          ;

usepac_list: usepac_list USP NEWLINE
           {
                $$ = createNode("usepac_list", "", 3);
                addChild($$, $1);
                astnode* temp = createNode("package", $2, 3);
                addChild($$, temp);
           }
           | /* empty */ { $$ = createNode("usepac_list", "", 3); }
           ;

title: TITLE NEWLINE { $$ = createNode("title", $1, 2); }
     | /* empty */ { $$ = createNode("title", "", 2); }
     ;

author: AUTHOR NEWLINE { $$ = createNode("author", $1, 2); }
      | /* empty */ { $$ = createNode("author", "", 2); }
      ;

date: DATE NEWLINE { $$ = createNode("date", $1, 2); }
    | /* empty */ { $$ = createNode("date", "", 2); }
    ;

body: begindocument sections enddocument
    {
         $$ = createNode("body", "", 1);
         addChild($$, $1);
         addChild($$, $2);
         addChild($$, $3);
    }
    ;

begindocument: BEGINDOC NEWLINE { $$ = createNode("begindocument", "", 2); }
             ;

enddocument: ENDDOC NEWLINE { $$ = createNode("enddocument", "", 2); }
           ;

sections: sections section subsections
        {
            $$ = createNode("sections_list", "", 3);
            addChild($$, $1);
            addChild($$, $2);
            addChild($$, $3);
        }
        | /* empty */ { $$ = createNode("sections_list", "", 3); }
        ;

section: SECTION NEWLINE
       {
            $$ = createNode("sections", "", 3);
            astnode* temp1 = createNode("section", $1, 4);
            addChild($$, temp1);
            astnode* temp2 = createNode("newline", $2, 4);
            addChild($$, temp2);
       }
       ;

subsections: subsections subsection subsubsections
           {
                $$ = createNode("subsections_list", "" , 4);
                addChild($$, $1);
                addChild($$, $2);
                addChild($$, $3);
           }
           | /* empty */ { $$ = createNode("subsections_list", "" , 4); }
           ;

subsection: SUBSECTION NEWLINE
          {
               $$ = createNode("subsections", "", 4);
               astnode* temp1 = createNode("subsection", $1, 5);
               addChild($$, temp1);
               astnode* temp2 = createNode("newline", $2, 5);
               addChild($$, temp2);
          }
          ;

subsubsections: subsubsections subsubsection content_list
              {
                   $$ = createNode("subsubsections_list", "", 5);
                   addChild($$, $1);
                   addChild($$, $2);
                   addChild($$, $3);
              }
              | /* empty */ { $$ = createNode("subsubsections_list", "" , 5); }
              ;

subsubsection: SUBSUBSECTION NEWLINE
             {
                 $$ = createNode("subsubsections", "", 5);
                 astnode* temp1 = createNode("subsubsection", $1, 6);
                 addChild($$, temp1);
                 astnode* temp2 = createNode("newline", $2, 6);
                 addChild($$, temp2);
             }
             ;

content_list: contents
            {
                 $$ = createNode("content_list","",6);
                 addChild($$, $1);
            }
            ;

contents: contents bold
        {
             $$ = createNode("content","", 6);
             addChild($$, $1);
             addChild($$, $2);
        }
        | contents italic
        {
             $$ = createNode("content","", 6);
             addChild($$, $1);
             addChild($$, $2);
        }
        | contents hrule
        {
             $$ = createNode("content","", 6);
             addChild($$, $1);
             addChild($$, $2);
        }
        | contents graphics
        {
             $$ = createNode("content","", 6);
             addChild($$, $1);
             addChild($$, $2);
        }
        | contents paragraph
        {
             $$ = createNode("content","", 6);
             addChild($$, $1);
             addChild($$, $2);
        }
        | contents verbatim
        {
             $$ = createNode("content","", 6);
             addChild($$, $1);
             addChild($$, $2);
        }
        | contents itemize
        {
             $$ = createNode("content","", 6);
             addChild($$, $1);
             addChild($$, $2);

        }
        | contents enumerate
        {
             $$ = createNode("content","", 6);
             addChild($$, $1);
             addChild($$, $2);
        }
        | contents table_content
        {
             $$ = createNode("content","", 6);
             addChild($$, $1);
             addChild($$, $2);
        }
        | /* empty */ { $$ = createNode("content","", 6); }
        ;

bold: BOLD NEWLINE
    {
         $$ = createNode("bold", "", 7);
         astnode* temp1 = createNode("bold_text", $1, 8);
         addChild($$, temp1);
         astnode* temp2 = createNode("newline", $2, 8);
         addChild($$, temp2);
    }
    ;

italic: ITALIC NEWLINE
      {
           $$ = createNode("italic", "", 7);
           astnode* temp1 = createNode("italic_text", $1, 8);
           addChild($$, temp1);
           astnode* temp2 = createNode("newline", $2, 8);
           addChild($$, temp2);
      }
      ;

hrule: HRULE NEWLINE
     {
          $$ = createNode("hrule", "", 7);
         astnode* temp1 = createNode("hrule_text", $1, 8);
         addChild($$, temp1);
         astnode* temp2 = createNode("newline", $2, 8);
         addChild($$, temp2);
     }
     ;

graphics: INCGRAPHICS NEWLINE
        {
             $$ = createNode("graphics", "", 7);
             astnode* temp1 = createNode("graphics_text", $1, 8);
             addChild($$, temp1);
             astnode* temp2 = createNode("newline", $2, 8);
             addChild($$, temp2);
        }
        ;

paragraph: TEXT NEWLINE
         {
              $$ = createNode("para", "", 7);
              astnode* temp1 = createNode("para_text", $1, 8);
              addChild($$, temp1);
              astnode* temp2 = createNode("newline", $2, 8);
              addChild($$, temp2);
         }
         ;

verbatim: BEGIN_VERBATIM NEWLINE verbatim_body END_VERBATIM NEWLINE
        {
             $$ = createNode("verbatim", "", 7);
             astnode* temp1 = createNode("start_verbatim", "```python", 8);
             addChild($$, temp1);
             astnode* temp2 = createNode("newline", $2, 8);
             addChild($$, temp2);
             addChild($$, $3);
             astnode* temp3 = createNode("end_verbatim", "```", 8);
             addChild($$, temp3);
             astnode* temp4 = createNode("newline", $5, 8);
             addChild($$, temp4);
        }
        ;

verbatim_body: block_verbatim
             {
                $$ = createNode("verbatim_body", "", 8);
                addChild($$, $1);
             }
             ;

block_verbatim: block_verbatim TEXT NEWLINE
              {
                  $$ = createNode("block_verbatim", "", 9);
                  addChild($$, $1);
                  astnode* temp1 = createNode("verbatim_text", $2, 9);
                  addChild($$, temp1);
                  astnode* temp2 = createNode("newline", $3, 9);
                  addChild($$, temp2);
              }
              | /* empty */ { $$ = createNode("block_verbatim", "", 9); }
              ;

itemize: BEGIN_ITEMIZE NEWLINE itemize_body END_ITEMIZE NEWLINE
       {
           $$ = createNode("itemize", "", 7);
           astnode* temp1 = createNode("start_itemize", "", 8);
           addChild($$, temp1);
           astnode* temp2 = createNode("newline", $2, 8);
           addChild($$, temp2);
           addChild($$, $3);
           astnode* temp3 = createNode("end_itemize", "", 8);
           addChild($$, temp3);
           astnode* temp4 = createNode("newline", $5, 8);
           addChild($$, temp4);
       }
       ;

itemize_body: block_itemize
            {
                $$ = createNode("itemize_body", "", 8);
                addChild($$, $1);
            }
            ;

block_itemize: block_itemize TEXT NEWLINE
             {
                 $$ = createNode("block_itemize", "", 9);
                 addChild($$, $1);
                 astnode* temp1 = createNode("itemize_text", $2, 9);
                 addChild($$, temp1);
                 astnode* temp2 = createNode("newline", $3, 9);
                 addChild($$, temp2);
             }
             | /* empty */ { $$ = createNode("block_itemize", "", 9); }
             ;

enumerate: BEGIN_ENUMERATE NEWLINE enumerate_body END_ENUMERATE NEWLINE
         {
            $$ = createNode("enumerate", "", 7);
            astnode* temp1 = createNode("start_enumerate", "", 8);
            addChild($$, temp1);
            astnode* temp2 = createNode("newline", $2, 8);
            addChild($$, temp2);
            addChild($$, $3);
            astnode* temp3 = createNode("end_enumerate", "", 8);
            addChild($$, temp3);
            astnode* temp4 = createNode("newline", $5, 8);
            addChild($$, temp4);
        }
        ;

enumerate_body: block_enumerate
              {
                  $$ = createNode("enumerate_body", "", 8);
                  addChild($$, $1);
              }
              ;

block_enumerate: block_enumerate TEXT NEWLINE
               {
                   $$ = createNode("block_enumerate", "", 9);
                   addChild($$, $1);
                   astnode* temp1 = createNode("enumerate_text", $2, 9);
                   addChild($$, temp1);
                   astnode* temp2 = createNode("newline", $3, 9);
                   addChild($$, temp2);
               }
               | /* empty */ { $$ = createNode("block_enumerate", "", 9); }
               ;

table_content: BEGIN_TABULAR NEWLINE HLINE NEWLINE table_head HLINE NEWLINE table_body HLINE NEWLINE END_TABULAR NEWLINE
             {
                 $$ = createNode("table_content", "", 7);
                 addChild($$, $5);
                 astnode* temp = createNode("hline","", 8);
                 addChild($$, temp);
                 addChild($$, $8);
             }
             ;

table_head: table_row
          {
              $$ = createNode("table_head_block", "", 8);
              addChild($$, $1);
          }
          ;

table_body: table_row
          {
              $$ = createNode("table_body_block", "", 8);
              addChild($$, $1);
          }
          ;

table_row: table_row TEXT NEWLINE
         {
              $$ = createNode("table_row", "", 9);
              addChild($$, $1);
              astnode* temp1 = createNode("table_text", $2, 9);
              addChild($$, temp1);
              astnode* temp2 = createNode("newline", $3, 9);
              addChild($$, temp2);
         }
         | /* empty */ { $$ = createNode("table_row", "", 9); }
         ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main() {
    yyparse();
    FILE *fileast = fopen("ast.tex", "w");
    if (fileast == NULL) {
        perror("Failed to open file");
        return 1;
    }
    printAST(root, fileast);
    fclose(fileast);
    FILE *filemd = fopen("output.md", "w");
    if (filemd == NULL) {
        perror("Failed to open file");
        return 1;
    }
    createMarkdown(root, filemd);
    fclose(filemd);
    freeAST(root);
    return 0;
}
