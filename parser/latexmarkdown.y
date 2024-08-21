%{
#include "latexmarkdown.h"
astnode *root;
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
