#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <memory>
#include <string>

class ASTNode {
public:
    virtual ~ASTNode() = default;
};

class LetNode : public ASTNode {
public:
    std::string identifier;
    std::unique_ptr<ASTNode> value;

    LetNode(const std::string& id, std::unique_ptr<ASTNode> val);
};

class NumberNode : public ASTNode {
public:
    int value;

    NumberNode(int val);
};

class IfNode : public ASTNode {
public:
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<ASTNode> thenBranch;
    std::unique_ptr<ASTNode> elseBranch;

    IfNode(std::unique_ptr<ASTNode> cond,
           std::unique_ptr<ASTNode> thenBranch,
           std::unique_ptr<ASTNode> elseBranch);
};

class BinaryOpNode : public ASTNode {
public:
    std::unique_ptr<ASTNode> left;
    std::string op; 
    std::unique_ptr<ASTNode> right;

    BinaryOpNode(std::unique_ptr<ASTNode> left,
                 const std::string& op,
                 std::unique_ptr<ASTNode> right);
};


class Parser {
public:
    Parser(Lexer& lexer);
    std::unique_ptr<ASTNode> parse(); 

    Token getCurrentToken() const;

private:
    Lexer& lexer;
    Token currentToken; 

    void consume(TokenType type); 
    std::unique_ptr<ASTNode> primary(); 
    std::unique_ptr<ASTNode> comparison();
    std::unique_ptr<ASTNode> expression();
};

#endif 
