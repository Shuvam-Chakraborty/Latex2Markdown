# LaTeX to Markdown Converter

This project is a LaTeX to Markdown converter built using Lex and Yacc. The converter reads LaTeX files and generates Markdown output, preserving the structure and formatting as closely as possible.

## Features

- **Section Handling**: Converts LaTeX sections (`\section`, `\subsection`, `\subsubsection`) to their corresponding Markdown headers (`#`, `##`, `###`).
- **Bold and Italic Text**: Translates `\textbf{}` and `\textit{}` to Markdown's `**bold**` and `*italic*` formats.
- **Images**: Converts LaTeX `\includegraphics` commands into Markdown image syntax.
- **Enumerations and Itemized Lists**: Properly formats LaTeX lists (`\begin{itemize}`, `\begin{enumerate}`) into Markdown's list syntax.
- **Tables**: Converts LaTeX `\begin{tabular}` environments to Markdown tables, maintaining column alignment.
- **Verbatim Blocks**: Converts `\begin{verbatim}` blocks to Markdown code blocks.
- **Support for Custom Commands**: Handles specific LaTeX commands like `\href` and `\par`, ensuring they are appropriately converted to Markdown.

## How It Works

The project uses Lex (a lexical analyzer) to tokenize LaTeX commands and Yacc (Yet Another Compiler Compiler) to parse these tokens and generate the corresponding Markdown output. The generated output is saved to a file named `output.md`.

## Installation

1. Clone this repository:
   ```bash
	git clone https://github.com/Shuvam-Chakraborty/Latex2Markdown.git
   ```
2. Compile the Lex and Yacc files:
   ```bash
	flex latexmarkdown.l
	yacc -d latexmarkdown.y
	gcc lex.yy.c y.tab.c -o latexmarkdown -ll
   ```
3. Run the converter on a LaTeX file:
   ```bash
	./latexmarkdown < input.tex > output.md
   ```
4. Visualize the output:
   ```bash
	cat output.md
   ```

## Usage

**Input**: Provide a LaTeX file as input to the converter.
**Output**: The converted Markdown output will be saved to output.md in the current directory.

## Contributing

If you find any issues or have suggestions for improvements, please feel free to create an issue or submit a pull request.

