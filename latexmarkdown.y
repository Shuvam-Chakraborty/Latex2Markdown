%{
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

%}

%union {
    char *str;
}

%token <str> SECTION SUBSECTION SUBSUBSECTION TEXT NEWLINE ITALIC BOLD DOCCLASS USP TITLE BEGIN_VERBATIM END_VERBATIM BEGIN_ITEMIZE END_ITEMIZE BEGIN_ENUMERATE END_ENUMERATE BEGIN_TABULAR END_TABULAR HLINE AUTHOR DATE BEGINDOC ENDDOC  INCGRAPHICS HRULE

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
    | BEGIN_VERBATIM NEWLINE { add_to_list("```python"); add_to_list($2); } block_verbatim END_VERBATIM NEWLINE { add_to_list("```\n"); add_to_list($2); } contents
    | BEGIN_ITEMIZE NEWLINE { add_to_list($2); } block_itemize END_ITEMIZE NEWLINE { add_to_list($2); } contents
    | BEGIN_ENUMERATE NEWLINE { item_number=1; add_to_list($2); } block_enumerate END_ENUMERATE NEWLINE { add_to_list($2); } contents
    | BEGIN_TABULAR NEWLINE { count_c($1); } HLINE NEWLINE table_head HLINE NEWLINE table_body HLINE NEWLINE END_TABULAR NEWLINE contents 
    ;

table_head:
    TEXT NEWLINE {
	process_table_row($1);
	add_to_list($2);
	add_table_separator(num_of_cols);
    }
    ;

table_body:
    | TEXT NEWLINE {
	process_table_row($1);
	add_to_list($2);
    } table_body
    ;

block_verbatim:
    | TEXT NEWLINE {
	add_to_list($1);
	add_to_list($2);
    } block_verbatim
    ;

block_itemize:
    | TEXT NEWLINE {
	add_to_list("- ");
        process_item($1);
        add_to_list($2);
    } block_itemize
    ;

block_enumerate:
    | TEXT NEWLINE {
	enumerate_list(item_number);
	item_number++;
        add_to_list(" ");
	process_item($1);
        add_to_list($2);
    } block_enumerate
    ;

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
    save_list_to_file();  // Save the list to output.md
    return 0;
}

