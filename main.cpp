#include "lexer.h"
#include "parser.h"
#include "codegen.h"
#include <fstream>
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: ./compiler <input.simplelang>" << std::endl;
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    if (!inputFile) {
        std::cerr << "Error: Unable to open file " << argv[1] << std::endl;
        return 1;
    }

    std::string sourceCode((std::istreambuf_iterator<char>(inputFile)),
                           std::istreambuf_iterator<char>());

    Lexer lexer(sourceCode);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto ast = parser.parse();

    CodeGen codegen(std::move(ast));
    auto assembly = codegen.generateAssembly();

    std::ofstream asmFile("output.asm");
    if (!asmFile) {
        std::cerr << "Error: Unable to create output.asm" << std::endl;
        return 1;
    }
    for (const auto& line : assembly) {
        asmFile << line << std::endl;
    }

    std::cout << "Assembly generated in output.asm" << std::endl;
    return 0;
}
