# Makefile for the LaTeX to Markdown converter

# Compiler and tools
CC = gcc
FLEX = flex
YACC = yacc

# Flags
FLEX_FLAGS = 
YACC_FLAGS = -d
CC_FLAGS = -ll

# Source and output files
LEX_FILE = latexmarkdown.l
YACC_FILE = latexmarkdown.y
INPUT_FILE = input.tex
OUTPUT_FILE = latexmarkdown
AST_FILE = ast.tex

# Intermediate files
LEX_OUTPUT = lex.yy.c
YACC_OUTPUT = y.tab.c
YACC_HEADER = y.tab.h

# Default rule
all: $(OUTPUT_FILE)

# Rule to build the final executable
$(OUTPUT_FILE): $(LEX_OUTPUT) $(YACC_OUTPUT)
	$(CC) $(LEX_OUTPUT) $(YACC_OUTPUT) -o $(OUTPUT_FILE) $(CC_FLAGS)

# Rule to generate lex.yy.c from the .l file
$(LEX_OUTPUT): $(LEX_FILE)
	$(FLEX) $(FLEX_FLAGS) $(LEX_FILE)

# Rule to generate y.tab.c and y.tab.h from the .y file
$(YACC_OUTPUT) $(YACC_HEADER): $(YACC_FILE)
	$(YACC) $(YACC_FLAGS) $(YACC_FILE)

# Rule to run the executable
run: $(OUTPUT_FILE)
	./$(OUTPUT_FILE) < $(INPUT_FILE)

# Rule to clean up generated files
clean:
	rm -f $(LEX_OUTPUT) $(YACC_OUTPUT) $(YACC_HEADER) $(OUTPUT_FILE)

# Rule to clean up all including output files
distclean: clean
	rm -f output.md $(AST_FILE)

# Phony targets
.PHONY: all clean distclean run

