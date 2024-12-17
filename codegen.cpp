#include "codegen.h"
#include <iostream>

CodeGen::CodeGen(std::unique_ptr<ASTNode> astNode) : ast(std::move(astNode)) {}

std::vector<std::string> CodeGen::generateAssembly() {
    std::vector<std::string> instructions;
    traverseAST(ast.get(), instructions);
    for (const auto& instr : instructions) {
        std::cout << "Generated Assembly: " << instr << std::endl;
    }
    return instructions;
}

void CodeGen::traverseAST(ASTNode* node, std::vector<std::string>& instructions) {
    if (auto num = dynamic_cast<NumberNode*>(node)) {
        instructions.push_back("ldi A " + std::to_string(num->value));
    } else if (auto binOp = dynamic_cast<BinaryOpNode*>(node)) {
        traverseAST(binOp->left.get(), instructions);
        instructions.push_back("push A");
        traverseAST(binOp->right.get(), instructions);
        instructions.push_back("pop B");
        if (binOp->op == "+") instructions.push_back("add");
        else if (binOp->op == "-") instructions.push_back("sub");
    } else if (auto cond = dynamic_cast<ConditionalNode*>(node)) {
        std::string elseLabel = "ELSE_LABEL";
        std::string endLabel = "END_LABEL";
        traverseAST(cond->condition.get(), instructions);
        instructions.push_back("jnc " + elseLabel);
        for (const auto& stmt : cond->thenBranch) {
            traverseAST(stmt.get(), instructions);
        }
        instructions.push_back("jmp " + endLabel);
        instructions.push_back(elseLabel + ":");
        for (const auto& stmt : cond->elseBranch) {
            traverseAST(stmt.get(), instructions);
        }
        instructions.push_back(endLabel + ":");
    }
}
