# Compiler
CC = gcc
LEX = flex
BISON = bison

# Files
LEX_SRC = latexmarkdown.l
YACC_SRC = latexmarkdown.y
PARSER_EXEC = parser
INPUT_FILE = input.tex

# Targets
all: parser

parser: lex.yy.c latexmarkdown.tab.c
	$(CC) -o $(PARSER_EXEC) lex.yy.c latexmarkdown.tab.c -ll
	./$(PARSER_EXEC) < $(INPUT_FILE)

lex.yy.c: $(LEX_SRC)
	$(LEX) $(LEX_SRC)

latexmarkdown.tab.c latexmarkdown.tab.h: $(YACC_SRC)
	$(BISON) -d $(YACC_SRC)

clean:
	rm -f $(PARSER_EXEC) lex.yy.c lex.yy.o latexmarkdown.tab.c latexmarkdown.tab.h latexmarkdown.tab.o

outclean:
	rm -f ast.tex output.md

.PHONY: all parser run test clean

