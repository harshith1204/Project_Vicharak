#include "codegen.h"
#include <iostream>

void AssemblyGenerator::generate(const ASTNode& node) {
    if (const LetNode* letNode = dynamic_cast<const LetNode*>(&node)) {
        if (const NumberNode* numNode = dynamic_cast<const NumberNode*>(letNode->value.get())) {
            output << "ldi r0 " << numNode->value << "\n"; 
            output << "sta " << letNode->identifier << "\n"; 
        } else if (const BinaryOpNode* binOp = dynamic_cast<const BinaryOpNode*>(letNode->value.get())) {
            generate(*binOp->left);
            output << "push r0\n"; 
            generate(*binOp->right);
            output << "pop r1\n"; 
            if (binOp->op == "+") {
                output << "add r0 r1\n"; 
            } else if (binOp->op == "-") {
                output << "sub r0 r1\n"; 
            }
            output << "sta " << letNode->identifier << "\n"; 
        }
    } else if (const IfNode* ifNode = dynamic_cast<const IfNode*>(&node)) {
        generate(*ifNode->condition); 
        output << "jmp_if_less THEN_LABEL\n"; 
        generate(*ifNode->elseBranch);
        output << "jmp END_LABEL\n";
        output << "THEN_LABEL:\n";
        generate(*ifNode->thenBranch);
        output << "END_LABEL:\n";
    } else if (const BinaryOpNode* binOp = dynamic_cast<const BinaryOpNode*>(&node)) {
        generate(*binOp->left);
        output << "push r0\n";
        generate(*binOp->right);
        output << "pop r1\n";
        if (binOp->op == "+") {
            output << "add r0 r1\n";
        } else if (binOp->op == "-") {
            output << "sub r0 r1\n";
        }
    }
}
