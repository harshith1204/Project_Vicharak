#include "parser.h"
#include <stdexcept>
#include <iostream>

Parser::Parser(const std::vector<Token>& toks) : tokens(toks), position(0) {}

const Token& Parser::current() const {
    return tokens[position];
}

void Parser::advance() {
    if (position < tokens.size()) position++;
}

std::unique_ptr<ASTNode> Parser::parsePrimary() {
    std::cout << "Parsing Primary: " << current().value << std::endl;
    if (current().type == TokenType::NUMBER) {
        auto node = std::make_unique<NumberNode>(std::stoi(current().value));
        advance();
        return node;
    }
    throw std::runtime_error("Unexpected token in Primary: " + current().value);
}

std::unique_ptr<ASTNode> Parser::parseExpression() {
    std::cout << "Parsing Expression: " << current().value << std::endl;
    auto left = parsePrimary();
    while (current().type == TokenType::SYMBOL && (current().value == "+" || current().value == "-")) {
        std::string op = current().value;
        std::cout << "Found operator: " << op << std::endl;
        advance();
        auto right = parsePrimary();
        left = std::make_unique<BinaryOpNode>(op, std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<ASTNode> Parser::parseConditional() {
    std::cout << "Parsing Conditional: " << current().value << std::endl;
    if (current().value == "if") {
        advance();
        auto condition = parseExpression();
        if (current().value != "{") {
            throw std::runtime_error("Expected '{' after if condition, found: " + current().value);
        }
        advance();
        std::vector<std::unique_ptr<ASTNode>> thenBranch;
        while (current().value != "}") {
            std::cout << "Parsing Then Branch: " << current().value << std::endl;
            thenBranch.push_back(parseExpression());
        }
        advance();
        std::vector<std::unique_ptr<ASTNode>> elseBranch;
        if (current().value == "else") {
            advance();
            if (current().value != "{") {
                throw std::runtime_error("Expected '{' after else, found: " + current().value);
            }
            advance();
            while (current().value != "}") {
                std::cout << "Parsing Else Branch: " << current().value << std::endl;
                elseBranch.push_back(parseExpression());
            }
            advance();
        }
        return std::make_unique<ConditionalNode>(ConditionalNode{
            std::move(condition),
            std::move(thenBranch),
            std::move(elseBranch)
        });
    }
    throw std::runtime_error("Unexpected token in Conditional: " + current().value);
}

std::unique_ptr<ASTNode> Parser::parse() {
    std::cout << "Parsing Token: " << current().value << std::endl;

    if (current().type == TokenType::KEYWORD && current().value == "let") {
        advance(); // Skip "let"
        if (current().type != TokenType::IDENTIFIER) {
            throw std::runtime_error("Expected variable name after 'let'");
        }
        std::string varName = current().value;
        advance(); 
        if (current().value != "=") {
            throw std::runtime_error("Expected '=' after variable name");
        }
        advance(); 
        auto value = parseExpression();
        std::cout << "Parsed variable declaration: " << varName << std::endl;
        return value; 
    }

    if (current().type == TokenType::KEYWORD && current().value == "if") {
        return parseConditional();
    }

    return parseExpression();
}

