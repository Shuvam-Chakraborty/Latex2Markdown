# LaTeX to Markdown Converter

This project is a LaTeX to Markdown converter built using Lex and Yacc. The converter reads LaTeX files, generates an Abstract Syntax Tree (AST), writes the AST to a file, and then traverses the AST to produce the Markdown output, preserving the structure and formatting as closely as possible.

## Features

- **Preamble Handling**: Processes the preamble part of the LaTeX file.
- **Section Handling**: Converts LaTeX sections (`\section`, `\subsection`, `\subsubsection`) to their corresponding Markdown headers (`#`, `##`, `###`).
- **Bold and Italic Text**: Translates `\textbf{}` and `\textit{}` to Markdown's `**bold**` and `*italic*` formats.
- **Images**: Converts LaTeX `\includegraphics` commands into Markdown image syntax.
- **Enumerations and Itemized Lists**: Properly formats LaTeX lists (`\begin{itemize}`, `\begin{enumerate}`) into Markdown's list syntax.
- **Tables**: Converts LaTeX `\begin{tabular}` environments to Markdown tables, maintaining column alignment.
- **Verbatim Blocks**: Converts `\begin{verbatim}` blocks to Markdown code blocks.
- **Support for Custom Commands**: Handles specific LaTeX commands like `\href` and `\par`, ensuring they are appropriately converted to Markdown.
- **Abstract Syntax Tree (AST)**: Generates an AST from the LaTeX input and writes it to `ast.tex`.
- **Makefile Integration**: Includes a `Makefile` to streamline the build and run process.
- **Scripted Execution**: Added a `run.sh` script for simplified execution, allowing users to specify input and output files directly.

## Workflow

The project uses Lex (a lexical analyzer) to tokenize LaTeX commands and Yacc (Yet Another Compiler Compiler) to parse these tokens and generate an AST. The AST is then traversed to generate the corresponding Markdown output, which is saved to `output.md`. The AST itself is saved to `ast.tex`.

## Installation and Usage

1. **Clone this repository**:
   ```bash
   git clone https://github.com/Shuvam-Chakraborty/Latex2Markdown.git
   ```

2. **Navigate to the parser directory**:
   ```bash
   cd Latex2Markdown/parser
   ```

3. **Compile and run the converter and generate AST(ast.tex) adn Markdown file(ouput.md)**:
   ```bash
   make
   ```

4. **Clean up all the intermediate files**:
   ```bash
   make clean
   ```

5. **View the output files**:

- **AST**: 
   ```bash
   cat ast.tex
   ```

- **Markdown Output**:
  ```bash
  cat output.md
  ```

6. **Clean up output files(ast.tex and output.md)**:
   ```bash
   make outclean
   ```

7. **Another way to run the converter(using 'run.sh' script) which will generate the output files(ast.tex and output.md) only**:
   ```bash
   ./run.sh input.tex output.md
   ```

8. **Create documentation using doxygen after some changes**:
   ```bash
   ./document.sh
   ```

## Contributing
If you find any issues or have suggestions for improvements, please feel free to create an issue or submit a pull request.

