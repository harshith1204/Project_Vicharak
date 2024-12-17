#ifndef CODEGEN_H
#define CODEGEN_H

#include "parser.h"
#include <vector>
#include <string>

class CodeGen {
public:
    explicit CodeGen(std::unique_ptr<ASTNode> ast);
    std::vector<std::string> generateAssembly();

private:
    std::unique_ptr<ASTNode> ast;
    void traverseAST(ASTNode* node, std::vector<std::string>& instructions);
};

#endif