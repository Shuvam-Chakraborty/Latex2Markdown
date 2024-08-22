/**
 * @file latexmarkdown.y
 * @brief Bison file for the LaTeX to Markdown converter.
 *
 * This file contains the parsing rules for the creation and management
 * of an Abstract Syntax Tree (AST), and uses different functions for
 * different LaTeX commands and convert them to Markdown.
 */

%{
#include "latexmarkdown.h"
astnode *root; /**< Root of the Abstract Syntax Tree (AST). */
%}

%union {
    char *str; /**< String type used for tokens. */
    struct astnode *node; /**< AST node type used for the syntax tree. */
}

%token <str> DOCCLASS USP TITLE AUTHOR DATE BEGINDOC ENDDOC SECTION SUBSECTION SUBSUBSECTION BOLD ITALIC HRULE INCGRAPHICS TEXT NEWLINE BEGIN_VERBATIM END_VERBATIM BEGIN_ITEMIZE END_ITEMIZE BEGIN_ENUMERATE END_ENUMERATE BEGIN_TABULAR END_TABULAR HLINE

%type <node> document preamble documentclass usepac_list usepackage title author date body begindocument enddocument sections section subsections subsection subsubsections subsubsection content_list contents bold italic hrule graphics paragraph verbatim itemize enumerate table_content block_verbatim verbatim_body itemize_body block_itemize enumerate_body block_enumerate table_head table_body table_row

%start document /**< Start symbol for the parser. */

%%

/**
 * @brief Parses the document structure, including preamble and body.
 */
document: preamble body
        {
             $$ = createNode("document", "", 0); /**< Create a "document" node with children for preamble and body. */
             root = $$;
             addChild($$, $1);
             addChild($$, $2);
        }
        ;

/**
 * @brief Parses the preamble section of the document.
 */
preamble: documentclass usepackage title author date
        {
             $$ = createNode("preamble", "", 1); /**< Create a "preamble" node with children for document class, usepackage, title, author, and date. */
             addChild($$, $1);
             addChild($$, $2);
             addChild($$, $3);
             addChild($$, $4);
             addChild($$, $5);
        }
        ;

/**
 * @brief Parses the document class declaration.
 */
documentclass: DOCCLASS NEWLINE { $$ = createNode("documentclass", $1, 2); }
             | /* empty */ { $$ = createNode("documentclass", "", 2); }
             ;

/**
 * @brief Parses the list of usepackage commands.
 */
usepackage: usepac_list
          {
              $$ = createNode("usepackage", "", 2); /**< Create a "usepackage" node with children for each package. */
              addChild($$, $1);
          }
          ;

/**
 * @brief Parses a list of usepackage commands.
 */
usepac_list: usepac_list USP NEWLINE
           {
                $$ = createNode("usepac_list", "", 3); /**< Create a "usepac_list" node with children for each package. */
                addChild($$, $1);
                astnode* temp = createNode("package", $2, 3);
                addChild($$, temp);
           }
           | /* empty */ { $$ = createNode("usepac_list", "", 3); }
           ;

/**
 * @brief Parses the title section.
 */
title: TITLE NEWLINE { $$ = createNode("title", $1, 2); }
     | /* empty */ { $$ = createNode("title", "", 2); }
     ;

/**
 * @brief Parses the author section.
 */
author: AUTHOR NEWLINE { $$ = createNode("author", $1, 2); }
      | /* empty */ { $$ = createNode("author", "", 2); }
      ;

/**
 * @brief Parses the date section.
 */
date: DATE NEWLINE { $$ = createNode("date", $1, 2); }
    | /* empty */ { $$ = createNode("date", "", 2); }
    ;

/**
 * @brief Parses the body of the document, including begindocument, sections, and enddocument.
 */
body: begindocument sections enddocument
    {
         $$ = createNode("body", "", 1); /**< Create a "body" node with children for begindocument, sections, and enddocument. */
         addChild($$, $1);
         addChild($$, $2);
         addChild($$, $3);
    }
    ;

/**
 * @brief Parses the begindocument command.
 */
begindocument: BEGINDOC NEWLINE { $$ = createNode("begindocument", "", 2); }
             ;

/**
 * @brief Parses the enddocument command.
 */
enddocument: ENDDOC NEWLINE { $$ = createNode("enddocument", "", 2); }
           ;

/**
 * @brief Parses the sections list in the document.
 */
sections: sections section subsections
        {
            $$ = createNode("sections_list", "", 3); /**< Create a "sections_list" node with children for each section and subsection. */
            addChild($$, $1);
            addChild($$, $2);
            addChild($$, $3);
        }
        | /* empty */ { $$ = createNode("sections_list", "", 3); }
        ;

/**
 * @brief Parses a section of the document.
 */
section: SECTION NEWLINE
       {
            $$ = createNode("sections", "", 3); /**< Create a "sections" node with a child for the section and newline. */
            astnode* temp1 = createNode("section", $1, 4);
            addChild($$, temp1);
            astnode* temp2 = createNode("newline", $2, 4);
            addChild($$, temp2);
       }
       ;

/**
 * @brief Parses a list of subsections in the document.
 */
subsections: subsections subsection subsubsections
           {
                $$ = createNode("subsections_list", "" , 4); /**< Create a "subsections_list" node with children for each subsection and subsubsections. */
                addChild($$, $1);
                addChild($$, $2);
                addChild($$, $3);
           }
           | /* empty */ { $$ = createNode("subsections_list", "" , 4); }
           ;

/**
 * @brief Parses a subsection of the document.
 */
subsection: SUBSECTION NEWLINE
          {
               $$ = createNode("subsections", "", 4); /**< Create a "subsections" node with a child for the subsection and newline. */
               astnode* temp1 = createNode("subsection", $1, 5);
               addChild($$, temp1);
               astnode* temp2 = createNode("newline", $2, 5);
               addChild($$, temp2);
          }
          ;

/**
 * @brief Parses a list of subsubsections.
 */
subsubsections: subsubsections subsubsection content_list
              {
                   $$ = createNode("subsubsections_list", "", 5); /**< Create a "subsubsections_list" node with children for each subsubsection and content list. */
                   addChild($$, $1);
                   addChild($$, $2);
                   addChild($$, $3);
              }
              | /* empty */ { $$ = createNode("subsubsections_list", "" , 5); }
              ;

/**
 * @brief Parses a subsubsection of the document.
 */
subsubsection: SUBSUBSECTION NEWLINE
             {
                 $$ = createNode("subsubsections", "", 5); /**< Create a "subsubsections" node with a child for the subsubsection and newline. */
                 astnode* temp1 = createNode("subsubsection", $1, 6);
                 addChild($$, temp1);
                 astnode* temp2 = createNode("newline", $2, 6);
                 addChild($$, temp2);
             }
             ;

/**
 * @brief Parses a list of content items.
 */
content_list: contents
            {
                 $$ = createNode("content_list","",6); /**< Create a "content_list" node with a child for the content items. */
                 addChild($$, $1);
            }
            ;

/**
 * @brief Parses different types of content.
 */
contents: contents bold
        {
             $$ = createNode("content","", 6); /**< Create a "content" node with children for bold content. */
             addChild($$, $1);
             addChild($$, $2);
        }
        | contents italic
        {
             $$ = createNode("content","", 6); /**< Create a "content" node with children for italic content. */
             addChild($$, $1);
             addChild($$, $2);
        }
        | contents hrule
        {
             $$ = createNode("content","", 6); /**< Create a "content" node with children for hrule content. */
             addChild($$, $1);
             addChild($$, $2);
        }
        | contents graphics
        {
             $$ = createNode("content","", 6); /**< Create a "content" node with children for graphics content. */
             addChild($$, $1);
             addChild($$, $2);
        }
        | contents paragraph
        {
             $$ = createNode("content","", 6); /**< Create a "content" node with children for paragraph content. */
             addChild($$, $1);
             addChild($$, $2);
        }
        | contents verbatim
        {
             $$ = createNode("content","", 6); /**< Create a "content" node with children for verbatim content. */
             addChild($$, $1);
             addChild($$, $2);
        }
        | contents itemize
        {
             $$ = createNode("content","", 6); /**< Create a "content" node with children for itemize content. */
             addChild($$, $1);
             addChild($$, $2);

        }
        | contents enumerate
        {
             $$ = createNode("content","", 6); /**< Create a "content" node with children for enumerate content. */
             addChild($$, $1);
             addChild($$, $2);
        }
        | contents table_content
        {
             $$ = createNode("content","", 6); /**< Create a "content" node with children for table_content content. */
             addChild($$, $1);
             addChild($$, $2);
        }
        | /* empty */ { $$ = createNode("content","", 6); }
        ;

/**
 * @brief Parses bold text followed by a newline.
 */
bold: BOLD NEWLINE
    {
         $$ = createNode("bold", "", 7);  /**< Create a "bold" node with children for bold_text and newline. */
         astnode* temp1 = createNode("bold_text", $1, 8);
         addChild($$, temp1);
         astnode* temp2 = createNode("newline", $2, 8);
         addChild($$, temp2);
    }
    ;

/**
 * @brief Parses italic text followed by a newline.
 */
italic: ITALIC NEWLINE
      {
           $$ = createNode("italic", "", 7); /**< Create a "italic" node with children for italic_text and newline. */
           astnode* temp1 = createNode("italic_text", $1, 8);
           addChild($$, temp1);
           astnode* temp2 = createNode("newline", $2, 8);
           addChild($$, temp2);
      }
      ;

/**
 * @brief Parses a horizontal rule followed by a newline.
 */
hrule: HRULE NEWLINE
     {
          $$ = createNode("hrule", "", 7); /**< Create a "hrule" node with children for hrule_text and newline. */
         astnode* temp1 = createNode("hrule_text", $1, 8);
         addChild($$, temp1);
         astnode* temp2 = createNode("newline", $2, 8);
         addChild($$, temp2);
     }
     ;

/**
 * @brief Parses an \includegraphics command followed by a newline.
 */
graphics: INCGRAPHICS NEWLINE
        {
             $$ = createNode("graphics", "", 7); /**< Create a "graphics" node with children for graphics_text and newline. */
             astnode* temp1 = createNode("graphics_text", $1, 8);
             addChild($$, temp1);
             astnode* temp2 = createNode("newline", $2, 8);
             addChild($$, temp2);
        }
        ;

/**
 * @brief Parses a paragraph of text followed by a newline.
 */
paragraph: TEXT NEWLINE
         {
              $$ = createNode("para", "", 7); /**< Create a "para" node with children for para_text and newline. */
              astnode* temp1 = createNode("para_text", $1, 8);
              addChild($$, temp1);
              astnode* temp2 = createNode("newline", $2, 8);
              addChild($$, temp2);
         }
         ;

/**
 * @brief Parses verbatim content with a beginning and end.
 */
verbatim: BEGIN_VERBATIM NEWLINE verbatim_body END_VERBATIM NEWLINE
        {
             $$ = createNode("verbatim", "", 7); /**< Create a "verbatim" node with children for the verbatim block. */
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

/**
 * @brief Creates a node for the body of a verbatim block.
 */
verbatim_body: block_verbatim
             {
                $$ = createNode("verbatim_body", "", 8); /**< Create a "verbatim_body" node with a child for the verbatim block. */
                addChild($$, $1);
             }
             ;

/**
 * @brief Creates a node for a block of verbatim text, handling nested text and newlines.
 */
block_verbatim: block_verbatim TEXT NEWLINE
              {
                  $$ = createNode("block_verbatim", "", 9); /**< Create a "block_verbatim" node with children for each line of the verbatim block. */
                  addChild($$, $1);
                  astnode* temp1 = createNode("verbatim_text", $2, 9);
                  addChild($$, temp1);
                  astnode* temp2 = createNode("newline", $3, 9);
                  addChild($$, temp2);
              }
              | /* empty */ { $$ = createNode("block_verbatim", "", 9); }
              ;

/**
 * @brief Parses an itemized list, starting and ending with appropriate tags.
 */
itemize: BEGIN_ITEMIZE NEWLINE itemize_body END_ITEMIZE NEWLINE
       {
           $$ = createNode("itemize", "", 7); /**< Create a "itemize" node with children for the itemize block. */
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

/**
 * @brief Creates a node for the body of an itemized list.
 */
itemize_body: block_itemize
            {
                $$ = createNode("itemize_body", "", 8); /**< Create an "itemize_body" node with a child for the itemize environment. */
                addChild($$, $1);
            }
            ;

/**
 * @brief Creates a node for a block of itemized text, handling nested text and newlines.
 */
block_itemize: block_itemize TEXT NEWLINE
             {
                 $$ = createNode("block_itemize", "", 9); /**< Create a "block_itemize" node with children for each item in the itemize environment. */
                 addChild($$, $1);
                 astnode* temp1 = createNode("itemize_text", $2, 9);
                 addChild($$, temp1);
                 astnode* temp2 = createNode("newline", $3, 9);
                 addChild($$, temp2);
             }
             | /* empty */ { $$ = createNode("block_itemize", "", 9); }
             ;

/**
 * @brief Parses an enumerated list, starting and ending with appropriate tags.
 */
enumerate: BEGIN_ENUMERATE NEWLINE enumerate_body END_ENUMERATE NEWLINE
         {
            $$ = createNode("enumerate", "", 7); /**< Create a "enumerate" node with children for the enumerate block. */
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

/**
 * @brief Creates a node for the body of an enumerated list.
 */
enumerate_body: block_enumerate
              {
                  $$ = createNode("enumerate_body", "", 8);/**< Create an "enumerate_body" node with a child for the enumerate environment. */
                  addChild($$, $1);
              }
              ;

/**
 * @brief Creates a node for a block of enumerated text, handling nested text and newlines.
 */
block_enumerate: block_enumerate TEXT NEWLINE
               {
                   $$ = createNode("block_enumerate", "", 9); /**< Create a "block_enumerate" node with children for each item in the enumerate environment. */
                   addChild($$, $1);
                   astnode* temp1 = createNode("enumerate_text", $2, 9);
                   addChild($$, temp1);
                   astnode* temp2 = createNode("newline", $3, 9);
                   addChild($$, temp2);
               }
               | /* empty */ { $$ = createNode("block_enumerate", "", 9); }
               ;

/**
 * @brief Parses a table content, including headers and body rows.
 */
table_content: BEGIN_TABULAR NEWLINE HLINE NEWLINE table_head HLINE NEWLINE table_body HLINE NEWLINE END_TABULAR NEWLINE
             {
                 $$ = createNode("table_content", "", 7); /**< Create a "table_content" node with children for the table environment. */
                 addChild($$, $5);
                 astnode* temp = createNode("hline","", 8);
                 addChild($$, temp);
                 addChild($$, $8);
             }
             ;

/**
 * @brief Creates a node for the table header block.
 */
table_head: table_row
          {
              $$ = createNode("table_head_block", "", 8); /**< Create a "table_head_block" node with a child for the table row. */
              addChild($$, $1);
          }
          ;

/**
 * @brief Creates a node for the table body block.
 */
table_body: table_row
          {
              $$ = createNode("table_body_block", "", 8); /**< Create a "table_body_block" node with a child for the table row. */
              addChild($$, $1);
          }
          ;

/**
 * @brief Creates a node for a table row, handling nested text and newlines.
 */
table_row: table_row TEXT NEWLINE
         {
              $$ = createNode("table_row", "", 9); /**< Create a "table_row" node with children for the table_text and newline. */
              addChild($$, $1);
              astnode* temp1 = createNode("table_text", $2, 9);
              addChild($$, temp1);
              astnode* temp2 = createNode("newline", $3, 9);
              addChild($$, temp2);
         }
         | /* empty */ { $$ = createNode("table_row", "", 9); }
         ;

%%

/**
 * @brief Main function that initializes parsing and file generation.
 *
 * @return int Exit status of the program.
 */
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
