#include "lexer.h"
#include <cctype>
#include <iostream>

Lexer::Lexer(const std::string& src) : source(src), position(0) {}

char Lexer::peek() const {
    return position < source.size() ? source[position] : '\0';
}

char Lexer::advance() {
    return source[position++];
}

void Lexer::skipWhitespace() {
    while (isspace(peek())) advance();
}

Token Lexer::parseNumber() {
    std::string value;
    while (isdigit(peek())) value += advance();
    return {TokenType::NUMBER, value};
}

Token Lexer::parseIdentifier() {
    std::string value;
    while (isalnum(peek())) value += advance();
    if (value == "let" || value == "if" || value == "else") {
        return {TokenType::KEYWORD, value};
    }
    return {TokenType::IDENTIFIER, value};
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    while (position < source.size()) {
        skipWhitespace();
        if (isdigit(peek())) {
            tokens.push_back(parseNumber());
        } else if (isalpha(peek())) {
            tokens.push_back(parseIdentifier());
        } else {
            tokens.push_back({TokenType::SYMBOL, std::string(1, advance())});
        }
    }
    tokens.push_back({TokenType::END_OF_FILE, ""});

    for (const auto& token : tokens) {
        std::cout << "Token: Type=" << static_cast<int>(token.type) << ", Value=" << token.value << std::endl;
    }

    return tokens;
}