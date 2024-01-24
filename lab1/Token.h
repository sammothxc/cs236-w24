#include <string>
#include <sstream>
#pragma once
using namespace std;

enum TokenType {
    COMMA, //...
};

class Token {
    private:
        TokenType type;
        string value;
        int line;
    public:
    Token(TokenType type, string value, int line) : type(type), value(value), line(line) {}
    string typeName(TokenType type) const {
        switch(type) {
            case COMMA: return "COMMA";
            default: return "UNKNOWN";
        }
    }
    string toString() const {
        stringstream out;
        out << "(" << typeName(type) << "," << "\"" << value << "\"" << "," << line << ")";
        return out.str();
    }
};