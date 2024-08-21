# LaTeX to Markdown Converter

This project is a LaTeX to Markdown converter built using Lex and Yacc. The converter reads LaTeX files, generates an Abstract Syntax Tree (AST), writes the AST to a file, and then traverses the AST to produce the Markdown output, preserving the structure and formatting as closely as possible.

## Project Structure

The `Latex2Markdown` repository contains the following components:

- **README.md**: Overview and documentation of the project.

- **parser**: Contains the core files and scripts for processing LaTeX files:
  - `Doxyfile`: Configuration file for generating documentation with Doxygen.
  - `doc/`: Directory for storing generated documentation.
  - `input.tex`: Sample LaTeX input file for testing the parser.
  - `latexmarkdown.l`: Lex file for tokenizing LaTeX commands.
  - `latexmarkdown.h`: Header file defining structures and functions for the parser.
  - `latexmarkdown.y`: Bison file for generating the Abstract Syntax Tree (AST) and Markdown output.
  - `Makefile`: Automates the build and execution process.
  - `document.sh`: Script for generating documentation.
  - `run.sh`: Script to simplify running the code.

- **testing**: Contains files and scripts for unit testing:
  - `CMakeLists.txt`: Configuration file for setting up the build system with CMake.
  - `coverage/`: Directory for storing code coverage reports.
  - `test_latexmarkdown.cpp`: Unit tests implemented using GTest.
  - `build/`: Directory for building the testing components.
  - `latexmarkdown.h`: Header file for testing.
  - `build_and_test.sh`: Script to build and run the tests.
  - `read_coverage_report.sh`: Script to read and display the coverage report.

## Features

1. **Lexer Tokenization**  
   The Lex file scans and tokenizes various LaTeX commands, identifying key elements like sections, text formatting, images, lists, tables, and custom commands for further processing.

2. **Bison-based AST Creation**  
   The Bison file builds an Abstract Syntax Tree (AST) from the tokenized LaTeX input. It then traverses the tree to generate the corresponding Markdown output, ensuring proper conversion of LaTeX structures into Markdown syntax.

3. **Automated Build and Execution**  
   The project has a `Makefile` and a `run.sh` script, enabling streamlined compilation, execution, and output generation. Users can easily build the parser and check the outputs using these files.

4. **Comprehensive Documentation**  
   The project leverages Doxygen for generating detailed documentation, covering all modules, functions, and features, ensuring that the codebase is well-documented and easy to understand.

5. **Unit Testing and Code Coverage**  
   Unit tests are implemented using GTest with CMake support. Code coverage is verified with `lcov`, ensuring that the code is thoroughly tested and meets quality standards.

## Workflow

The project uses Lex (a lexical analyzer) to tokenize LaTeX commands and Yacc (Yet Another Compiler Compiler) to parse these tokens and generate an AST. The AST is then traversed to generate the corresponding Markdown output, which is saved to `output.md`. The AST itself is saved to `ast.tex`.

## Important Commands

### Clone GitHub Repository
   ```bash
   git clone https://github.com/Shuvam-Chakraborty/Latex2Markdown.git
   ```

### Make Parser using Makefile
1. **Navigate to the parser directory**:
   ```bash
   cd ~/Latex2Markdown/parser
   ```

2. **Compile and run the converter and generate AST(ast.tex) and Markdown file(ouput.md)**:
   ```bash
   make
   ```

3. **Clean up all the intermediate files**:
   ```bash
   make clean
   ```

4. **View the Output Files**:

- **AST**: 
   ```bash
   cat ast.tex
   ```

- **Markdown Output**:
  ```bash
  cat output.md
  ```

5. **Clean up Output Files(ast.tex and output.md)**:
   ```bash
   make outclean
   ```

### Compile and Run the Converter using Script(run.sh)
1. Generate the Output Files(ast.tex and output.md):
   ```bash
   cd ~/Latex2Markdown/parser
   ./run.sh input.tex output.md
   ```

2. **View the Output Files**:

- **AST**:
   ```bash
   cat ast.tex
   ```

- **Markdown Output**:
  ```bash
  cat output.md
  ``` 
3. **Clean up Output Files(ast.tex and output.md)**:
   ```bash
   make outclean
   ```

### Create Documentation using doxygen:
   ```bash
   cd ~/Latex2Markdown/parser
   ./document.sh
   ```

### Unit testing and Coverage report check:
   ```bash
   cd ~/Latex2Markdown/testing
   ./build_and_test.sh
   ./read_coverage_report.sh
   ```

## Contributing
If you find any issues or have suggestions for improvements, please feel free to create an issue or submit a pull request.

