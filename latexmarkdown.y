%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Function declarations
void yyerror(const char *s);
int yylex();

// Global variables
int item_number; // Counter for items in enumerate block
int num_of_cols; // Number of columns in a table

// Node structure for linked list
typedef struct Node {
    char *str;
    struct Node *next;
} Node;

// Pointers for the linked list
Node *head = NULL, *tail = NULL;

// Function to add a string to the linked list
void add_to_list(char *str) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->str = strdup(str); // Duplicate the string
    new_node->next = NULL;
    if (tail) {
        tail->next = new_node;
        tail = new_node;
    } else {
        head = tail = new_node;
    }
}

// Function to save the contents of the linked list to an output.md file
void save_list_to_file() {
    // Open the file "output.md" in write mode
    FILE *file = fopen("output.md", "w");
    
    // Check if the file was successfully opened
    if (file == NULL) {
        // If file couldn't be opened, print an error message and return
        fprintf(stderr, "Error opening file for writing\n");
        return;
    }
    
    // Pointer to traverse the linked list starting from the head node
    struct Node *current = head;
    
    // Iterate over the linked list and write each node's string to the file
    while (current != NULL) {
        // Write the string stored in the current node to the file
        fprintf(file, "%s", current->str);
        // Move to the next node in the list
        current = current->next;
    }
    
    // Close the file after writing all data
    fclose(file);
}

// Function to handle the \includegraphics command and extract the content within braces
void handle_graphics(char *str) {
    // Find the first occurrence of the opening brace '{' and closing brace '}'
    char *start = strchr(str, '{');
    char *end = strchr(str, '}');

    // Check if both braces are found and the opening brace is before the closing brace
    if (start != NULL && end != NULL && start < end) {
        start++; // Move the pointer to the character after the opening brace
        int length = end - start; // Calculate the length of the content inside the braces

        // Allocate a buffer to store the extracted content
        char content[length + 1];
        
        // Copy the content inside the braces to the buffer
        strncpy(content, start, length);
        content[length] = '\0'; // Null-terminate the string

        // Add the extracted content to the list
        add_to_list(content);
    } else {
        // If the string is invalid (missing braces), print an error message
        printf("Invalid input string.\n");
    }
}

// Function to handle \href command and format it for Markdown
void handle_href(char* str) {
    char *left, *url, *text, *right;
    const char* href_start = strstr(str, "\\href{");

    if (href_start == NULL) {
        // No \href found, print the entire string as left
        left = strdup(str);
        url = text = right = NULL;
    } else {
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

    // Print the results in Markdown format
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

// Check if a string contains \href and handle accordingly
void check_and_handle_href(char* str) {
    if (strstr(str, "\\href{")) {
        handle_href(str);
    } else {
        add_to_list(str);
    }
}

// Function to handle the \par command and convert it to Markdown paragraph breaks
void handle_para(char *str) {
    // Define the LaTeX \par command to search for and its Markdown equivalent
    const char *search = "\\par";
    const char *replace = "\n\n";
    int search_len = strlen(search);   // Length of the \par command
    int replace_len = strlen(replace); // Length of the Markdown equivalent
    int count = 0;                     // Counter for occurrences of \par
    char *pos = str;

    // Count the number of \par occurrences in the input string
    while ((pos = strstr(pos, search)) != NULL) {
        count++;
        pos += search_len;
    }

    // Calculate the new length of the string after replacements
    size_t new_len = strlen(str) + count * (replace_len - search_len) + 1;
    char *result = (char *)malloc(new_len);

    // Check if memory allocation was successful
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    char *current_pos = result; // Pointer to the current position in the result string

    // Replace all \par occurrences with \n\n (Markdown paragraph breaks)
    while ((pos = strstr(str, search)) != NULL) {
        size_t len = pos - str;              // Length of the segment before \par
        memcpy(current_pos, str, len);       // Copy the segment before \par
        current_pos += len;                  // Move the current position
        memcpy(current_pos, replace, replace_len); // Copy the replacement \n\n
        current_pos += replace_len;          // Move the current position
        str = pos + search_len;              // Move the input string pointer past \par
    }

    // Copy any remaining part of the string after the last \par
    strcpy(current_pos, str);

    // Handle any \href commands within the processed text
    check_and_handle_href(result);

    // Free the allocated memory for the result string
    free(result);
}

// Function to add an enumeration item number and format it for Markdown
void enumerate_list(int item_number) {
    // Calculate the length required for the item number string (including the dot and null terminator)
    int length = snprintf(NULL, 0, "%d.", item_number) + 1;

    // Dynamically allocate memory for the item number string
    char *num_str = (char *)malloc(length * sizeof(char));

    // Check if memory allocation was successful
    if (num_str == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    // Format the item number with a dot (e.g., "1. ") and store it in the allocated memory
    snprintf(num_str, length, "%d. ", item_number);

    // Add the formatted item number to the list for Markdown output
    add_to_list(num_str);

    // Free the allocated memory for the item number string
    free(num_str);
}

// Function to process each item in the enumerate block
void process_item(char *str) {
    // Step 1: Skip leading whitespace characters and tabs
    while (isspace((unsigned char)*str) || (unsigned char)*str == '\t') {
        str++;
    }

    // Step 2: Check if the string starts with the LaTeX \item command
    if (strncmp(str, "\\item", 5) == 0) {
        str += 5;  // Skip the \item keyword
    }

    // Step 3: Skip any whitespace characters after \item
    while (isspace((unsigned char)*str)) {
        str++;
    }

    // Step 4: Duplicate the remaining string after \item and any whitespace
    char *result_str = strdup(str);

    // Step 5: Check if memory allocation was successful
    if (result_str == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    // Step 6: Add the processed item text to the list for Markdown output
    add_to_list(result_str);

    // Step 7: Free the allocated memory for the processed item string
    free(result_str);
}

// Function to count the number of colums in a table by counting number of 'c' characters in the \begin{tabular}{...} string
void count_c(char* str) {
    int count = 0;
    while (*str != '\0') {
        if (*str == 'c') {
            count++;
        }
        str++;
    }
    num_of_cols = count;
}

// Function to process each row in a LaTeX tabular environment and format it for Markdown
void process_table_row(char *str) {
    char result[1002] = ""; // Buffer to store the final formatted row
    int cnt = 49, i = 1; // cnt tracks the remaining space in a cell, i is the index for the result array

    // Get the length of the input string
    size_t len = strlen(str);

    // Check if the row ends with "\\" and replace it with "&"
    if (len > 1 && str[len - 1] == '\\' && str[len - 2] == '\\') {
        str[len - 2] = '&';  // Replace the "\\" with "&"
        str[len - 1] = '\0'; // Remove the trailing "\\"
    }

    // Start the Markdown table row with a pipe character '|'
    result[0] = '|';

    // Loop through the input string to process each character
    while (*str) {
        if ((char)*str != '&') { // If the current character is not a column separator ('&')
            result[i] = (char)*str; // Copy the character to the result buffer
            str++; // Move to the next character in the input string
            cnt--; // Decrement the remaining space counter for the current cell
            i++; // Move to the next index in the result buffer
        } else {
            // If the current character is a column separator ('&')
            while (cnt--) { // Fill the remaining space in the current cell with spaces
                result[i++] = ' ';
            }
            cnt = 49; // Reset the space counter for the next cell
            result[i++] = '|'; // Add a pipe character to mark the end of the current cell
            str++; // Move to the next character in the input string
        }
        result[i] = '\0'; // Null-terminate the result string
    }

    // Add the formatted row to the output list for Markdown
    add_to_list(result);
}

// Function to add a separator line for a table with the specified number of columns
void add_table_separator(int num_of_cols) {
    // Each column separator has 50 characters: "|" + 49 "-"
    int column_width = 50;
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
    char s[51];  // | + 49 dashes + nullchar
    strcpy(s, "|");
    memset(s + 1, '-', 49);  // Fill the next 49 characters with '-'
    s[50] = '\0';  // Null-terminate the string

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

%token <str> BEGINDOC ENDDOC DOCCLASS USP TITLE AUTHOR DATE SECTION SUBSECTION SUBSUBSECTION BOLD ITALIC HLINE INCGRAPHICS TEXT NEWLINE BEGIN_VERBATIM END_VERBATIM BEGIN_ITEMIZE END_ITEMIZE BEGIN_ENUMERATE END_ENUMERATE BEGIN_TABULAR END_TABULAR HRULE

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

%%

// Error handling function for the parser
void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s); // Print error message to standard error stream
}

// Main function to execute the parser and save the output
int main() {
    yyparse(); // Start the parsing process; this will use the lexer and parser to process the input
    save_list_to_file();  // Save the accumulated results (from the linked list) to an output.md file
    return 0;
}

