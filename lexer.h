#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

enum class TokenType {
    IDENTIFIER,
    KEYWORD,
    NUMBER,
    SYMBOL,
    END_OF_FILE
};

struct Token {
    TokenType type;
    std::string value;
};

class Lexer {
public:
    explicit Lexer(const std::string& source);
    std::vector<Token> tokenize();

private:
    std::string source;
    size_t position;

    char peek() const;
    char advance();
    void skipWhitespace();
    Token parseNumber();
    Token parseIdentifier();
};

#endif