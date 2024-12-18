#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <memory>
#include <string>

// Abstract Syntax Tree (AST) Node Base Class
class ASTNode {
public:
    virtual ~ASTNode() = default;
};

// AST Node for variable declarations and assignments
class LetNode : public ASTNode {
public:
    std::string identifier;
    std::unique_ptr<ASTNode> value;

    LetNode(const std::string& id, std::unique_ptr<ASTNode> val);
};

// AST Node for numeric literals
class NumberNode : public ASTNode {
public:
    int value;

    NumberNode(int val);
};

// AST Node for "if-else" statements
class IfNode : public ASTNode {
public:
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<ASTNode> thenBranch;
    std::unique_ptr<ASTNode> elseBranch;

    IfNode(std::unique_ptr<ASTNode> cond,
           std::unique_ptr<ASTNode> thenBranch,
           std::unique_ptr<ASTNode> elseBranch);
};

// AST Node for binary operations
class BinaryOpNode : public ASTNode {
public:
    std::unique_ptr<ASTNode> left;
    std::string op; // Operator (e.g., "+", "-", "<")
    std::unique_ptr<ASTNode> right;

    BinaryOpNode(std::unique_ptr<ASTNode> left,
                 const std::string& op,
                 std::unique_ptr<ASTNode> right);
};

// Parser class
class Parser {
public:
    Parser(Lexer& lexer);
    std::unique_ptr<ASTNode> parse(); // Parse the input and return an AST

    // Expose the current token for checking purposes
    Token getCurrentToken() const;

private:
    Lexer& lexer;
    Token currentToken; // Tracks the current token being processed

    void consume(TokenType type); // Consume the current token and move to the next one
    std::unique_ptr<ASTNode> primary(); // Parse primary expressions (numbers, identifiers)
    std::unique_ptr<ASTNode> comparison(); // Parse comparison expressions
    std::unique_ptr<ASTNode> expression(); // Parse binary operations
};

#endif // PARSER_H
