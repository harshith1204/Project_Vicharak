# SimpleLang Compiler

SimpleLang is a custom programming language with a basic compiler that translates SimpleLang code into assembly-like instructions. This project includes a lexer, parser, and code generator to process SimpleLang code and generate corresponding assembly.

## Project Structure

- **main.cpp**: The entry point of the application, responsible for reading input files, processing the source code, and writing the output assembly file.
- **lexer.h / lexer.cpp**: Implements the lexer to tokenize the source code into meaningful symbols (tokens).
- **parser.h / parser.cpp**: Implements the parser to generate an Abstract Syntax Tree (AST) from the tokens.
- **codegen.h / codegen.cpp**: Implements the code generator to convert the AST into assembly instructions.
- **simplelang**: Example source code written in SimpleLang.
- **test.simplelang**: A sample input file for testing.
- **output.asm**: Generated assembly output for the provided SimpleLang input.

## Features

- Supports variable declarations (`let` keyword) and assignments.
- Implements basic arithmetic operations (`+`, `-`).
- Includes conditional statements (`if-else`).
- Outputs assembly-like instructions to a file.

## How to Build

1. Ensure you have a C++ compiler installed (e.g., g++).
2. Run the following commands in your terminal:
   ```sh
   g++ -std=c++17 -o simplelang main.cpp lexer.cpp parser.cpp codegen.cpp
   ```

## Usage

Run the program with the following syntax:
```sh
./simplelang <input_file.simplelang> <output_file.asm>
```
Example:
```sh
./simplelang test.simplelang output.asm
```

## Example

Input (`test.simplelang`):
```simplelang
let x = 5;
let y = x + 10;
if x < y {
  let z = y - x;
} else {
  let z = x - y;
}
```

Output (`output.asm`):
```asm
ldi r0 5
sta x
ldi r0 10
push r0
lda x
add r0 r1
sta y
lda x
push r0
lda y
sub r0 r1
jmp_if_less THEN_LABEL
sta z
jmp END_LABEL
THEN_LABEL:
lda y
push r0
lda x
sub r0 r1
sta z
END_LABEL:
```

