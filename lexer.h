#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <stdexcept>

enum class TokenType {
    LET, IDENTIFIER, NUMBER, PLUS, MINUS, LESS, ASSIGN, IF, ELSE,
    LBRACE, RBRACE, SEMICOLON, END
};

struct Token {
    TokenType type;
    std::string value;
};

class Lexer {
public:
    Lexer(const std::string& source);
    Token nextToken();

private:
    std::string source;
    size_t position;

    Token identifier();
    Token number();
};

#endif 
