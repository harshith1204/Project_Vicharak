#include "lexer.h"
#include <cctype>

Lexer::Lexer(const std::string& source) : source(source), position(0) {}

Token Lexer::nextToken() {
    while (position < source.size()) {
        char current = source[position];

        if (isspace(current)) {
            position++;
            continue;
        }

        if (isalpha(current)) {
            return identifier();
        }

        if (isdigit(current)) {
            return number();
        }

        switch (current) {
            case '+': position++; return {TokenType::PLUS, "+"};
            case '-': position++; return {TokenType::MINUS, "-"};
            case '<': position++; return {TokenType::LESS, "<"};
            case '=': position++; return {TokenType::ASSIGN, "="};
            case '{': position++; return {TokenType::LBRACE, "{"};
            case '}': position++; return {TokenType::RBRACE, "}"};
            case ';': position++; return {TokenType::SEMICOLON, ";"};
            default:
                throw std::runtime_error("Unexpected character: " + std::string(1, current));
        }
    }
    return {TokenType::END, ""};
}

Token Lexer::identifier() {
    size_t start = position;
    while (position < source.size() && isalnum(source[position])) {
        position++;
    }
    std::string id = source.substr(start, position - start);
    if (id == "let") return {TokenType::LET, id};
    if (id == "if") return {TokenType::IF, id};
    if (id == "else") return {TokenType::ELSE, id};
    return {TokenType::IDENTIFIER, id};
}

Token Lexer::number() {
    size_t start = position;
    while (position < source.size() && isdigit(source[position])) {
        position++;
    }
    return {TokenType::NUMBER, source.substr(start, position - start)};
}
