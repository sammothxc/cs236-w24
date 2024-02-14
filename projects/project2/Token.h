#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum TokenType {
    SCHEMES, FACTS, RULES, QUERIES,
    ID, STRING, COMMA, COLON, COLON_DASH, Q_MARK, LEFT_PAREN, RIGHT_PAREN, PERIOD, EOF_TOKEN
};

struct Token {
    TokenType type;
    std::string value;
};

#endif