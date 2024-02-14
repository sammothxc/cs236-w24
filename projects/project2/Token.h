#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum TokenType {
    SCHEMES, FACTS, RULES, QUERIES, ID, STRING, COLON, LEFT_PAREN, RIGHT_PAREN,
    COMMA, PERIOD, COLON_DASH, Q_MARK, EOF_TOKEN, UNKNOWN
};

class Token {
public:
    TokenType type;
    std::string value;
    int line;

    Token(TokenType t, const std::string& v, int l);
};

#endif