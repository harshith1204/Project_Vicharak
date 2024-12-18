#include <iostream>
#include <fstream>
#include <sstream>
#include "lexer.h"
#include "parser.h"
#include "codegen.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: ./simplelang <input_file.simplelang> <output_file.asm>\n";
        return 1;
    }

    const char* inputFileName = argv[1];
    const char* outputFileName = argv[2];

    std::ifstream inputFile(inputFileName);
    if (!inputFile.is_open()) {
        std::cerr << "Could not open the input file.\n";
        return 1;
    }

    std::ofstream outputFile(outputFileName);
    if (!outputFile.is_open()) {
        std::cerr << "Could not open the output file.\n";
        return 1;
    }

    std::stringstream buffer;
    buffer << inputFile.rdbuf();
    inputFile.close();

    Lexer lexer(buffer.str());
    Parser parser(lexer);
    AssemblyGenerator generator(outputFile); 

    try {
        while (true) {
            auto ast = parser.parse();
            generator.generate(*ast);

            if (parser.getCurrentToken().type == TokenType::END) {
                break;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }

    outputFile.close();
    return 0;
}
