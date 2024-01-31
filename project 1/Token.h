#pragma once
#include <string>

enum TokenType {
    COMMA,
    PERIOD,
    Q_MARK,
    LEFT_PAREN,
    RIGHT_PAREN,
    COLON,
    COLON_DASH,
    MULTIPLY,
    ADD,
    SCHEMES,
    FACTS,
    RULES,
    QUERIES,
    ID,
    STRING,
    COMMENT,
    UNDEFINED,
    EOF_TOKEN // Need to debug this
};

class Token {
private:
    TokenType type;
    std::string value;
    int line;
public:
    Token(TokenType type, const std::string& value, int line) : type(type), value(value), line(line) {}

    TokenType getType() const { return type; } // Getter method for type

    std::string typeName() const {
        switch(type) {
            case COMMA: return "COMMA";
            case PERIOD: return "PERIOD";
            case Q_MARK: return "Q_MARK";
            case LEFT_PAREN: return "LEFT_PAREN";
            case RIGHT_PAREN: return "RIGHT_PAREN";
            case COLON: return "COLON";
            case COLON_DASH: return "COLON_DASH";
            case MULTIPLY: return "MULTIPLY";
            case ADD: return "ADD";
            case SCHEMES: return "SCHEMES";
            case FACTS: return "FACTS";
            case RULES: return "RULES";
            case QUERIES: return "QUERIES";
            case ID: return "ID";
            case STRING: return "STRING";
            case COMMENT: return "COMMENT";
            case UNDEFINED: return "UNDEFINED";
            case EOF_TOKEN: return "EOF";
            default: return "UNKNOWN";
        }
    }

    std::string toString() const {
        return "(" + typeName() + ",\"" + value + "\"," + std::to_string(line) + ")";
    }
};
