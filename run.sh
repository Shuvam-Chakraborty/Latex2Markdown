#!/bin/bash

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <input.tex> <output.md>"
    exit 1
fi

input_file="$1"
output_file="$2"
ast_file="ast.tex"
make
./latexmarkdown < "$input_file"
make clean
echo "Conversion complete. The Markdown output is in '$output_file' and AST is in '$ast_file'."

