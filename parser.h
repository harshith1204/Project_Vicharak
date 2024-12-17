#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <memory>
#include <vector>

struct ASTNode {
    virtual ~ASTNode() = default;
};

struct NumberNode : public ASTNode {
    int value;
    NumberNode(int val) : value(val) {}
};

struct BinaryOpNode : public ASTNode {
    std::string op;
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;

    BinaryOpNode(std::string o, std::unique_ptr<ASTNode> l, std::unique_ptr<ASTNode> r)
        : op(std::move(o)), left(std::move(l)), right(std::move(r)) {}
};

struct ConditionalNode : public ASTNode {
    std::unique_ptr<ASTNode> condition;
    std::vector<std::unique_ptr<ASTNode>> thenBranch;
    std::vector<std::unique_ptr<ASTNode>> elseBranch;

    ConditionalNode(std::unique_ptr<ASTNode> cond,
                    std::vector<std::unique_ptr<ASTNode>> thenBr,
                    std::vector<std::unique_ptr<ASTNode>> elseBr)
        : condition(std::move(cond)),
          thenBranch(std::move(thenBr)),
          elseBranch(std::move(elseBr)) {}
};

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens);
    std::unique_ptr<ASTNode> parse();

private:
    const std::vector<Token>& tokens;
    size_t position;

    const Token& current() const;
    void advance();

    std::unique_ptr<ASTNode> parsePrimary();
    std::unique_ptr<ASTNode> parseExpression();
    std::unique_ptr<ASTNode> parseConditional();
};

#endif