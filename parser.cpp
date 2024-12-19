#include "parser.h"
#include <stdexcept>


LetNode::LetNode(const std::string& id, std::unique_ptr<ASTNode> val)
    : identifier(id), value(std::move(val)) {}

NumberNode::NumberNode(int val) : value(val) {}

IfNode::IfNode(std::unique_ptr<ASTNode> cond,
               std::unique_ptr<ASTNode> thenBranch,
               std::unique_ptr<ASTNode> elseBranch)
    : condition(std::move(cond)),
      thenBranch(std::move(thenBranch)),
      elseBranch(std::move(elseBranch)) {}

BinaryOpNode::BinaryOpNode(std::unique_ptr<ASTNode> left,
                           const std::string& op,
                           std::unique_ptr<ASTNode> right)
    : left(std::move(left)), op(op), right(std::move(right)) {}

Parser::Parser(Lexer& lexer) : lexer(lexer), currentToken(lexer.nextToken()) {}

Token Parser::getCurrentToken() const {
    return currentToken;
}

void Parser::consume(TokenType type) {
    if (currentToken.type == type) {
        currentToken = lexer.nextToken();
    } else {
        throw std::runtime_error("Unexpected token: " + currentToken.value);
    }
}

std::unique_ptr<ASTNode> Parser::primary() {
    if (currentToken.type == TokenType::NUMBER) {
        int numValue = std::stoi(currentToken.value);
        consume(TokenType::NUMBER);
        return std::make_unique<NumberNode>(numValue);
    }
    if (currentToken.type == TokenType::IDENTIFIER) {
        std::string id = currentToken.value;
        consume(TokenType::IDENTIFIER);
        return std::make_unique<LetNode>(id, nullptr); 
    }
    throw std::runtime_error("Unexpected token in primary: " + currentToken.value);
}

std::unique_ptr<ASTNode> Parser::comparison() {
    auto left = primary();
    if (currentToken.type == TokenType::LESS) {
        consume(TokenType::LESS);
        auto right = primary();
        return std::make_unique<BinaryOpNode>(std::move(left), "<", std::move(right));
    }
    return left;
}

std::unique_ptr<ASTNode> Parser::expression() {
    auto left = comparison();
    while (currentToken.type == TokenType::PLUS || currentToken.type == TokenType::MINUS) {
        std::string op = currentToken.value;
        if (currentToken.type == TokenType::PLUS) {
            consume(TokenType::PLUS);
        } else if (currentToken.type == TokenType::MINUS) {
            consume(TokenType::MINUS);
        }
        auto right = comparison();
        left = std::make_unique<BinaryOpNode>(std::move(left), op, std::move(right));
    }
    return left;
}

std::unique_ptr<ASTNode> Parser::parse() {
    if (currentToken.type == TokenType::LET) {
        consume(TokenType::LET);
        if (currentToken.type != TokenType::IDENTIFIER) {
            throw std::runtime_error("Expected identifier");
        }
        std::string id = currentToken.value;
        consume(TokenType::IDENTIFIER);
        consume(TokenType::ASSIGN);
        auto value = expression();
        consume(TokenType::SEMICOLON);
        return std::make_unique<LetNode>(id, std::move(value));
    } else if (currentToken.type == TokenType::IF) {
        consume(TokenType::IF);
        auto condition = expression();
        consume(TokenType::LBRACE);
        auto thenBranch = parse();
        consume(TokenType::RBRACE);
        std::unique_ptr<ASTNode> elseBranch = nullptr;
        if (currentToken.type == TokenType::ELSE) {
            consume(TokenType::ELSE);
            consume(TokenType::LBRACE);
            elseBranch = parse();
            consume(TokenType::RBRACE);
        }
        return std::make_unique<IfNode>(std::move(condition), std::move(thenBranch), std::move(elseBranch));
    } else if (currentToken.type == TokenType::IDENTIFIER) {
        std::string id = currentToken.value;
        consume(TokenType::IDENTIFIER);
        if (currentToken.type == TokenType::ASSIGN) {
            consume(TokenType::ASSIGN);
            auto value = expression();
            consume(TokenType::SEMICOLON);
            return std::make_unique<LetNode>(id, std::move(value));
        } else {
            throw std::runtime_error("Unexpected token after identifier: " + currentToken.value);
        }
    }
    throw std::runtime_error("Unexpected token in parse: " + currentToken.value);
}
