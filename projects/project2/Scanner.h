#ifndef SCANNER_H
#define SCANNER_H

#include <vector>
#include "Token.h"

using namespace std;

class Scanner {
public:
    Scanner(std::istream& input) : input(input) {}

    Token getNextToken() {
        char c;
        std::string tokenValue;
        while (input.get(c)) {
            if (isspace(c)) {
                continue;
            } else if (c == ',') {
                return {COMMA, ","};
            } else if (c == ':') {
                if (input.peek() == '-') {
                    input.get();
                    return {COLON_DASH, ":-"};
                }
                return {COLON, ":"};
            } else if (c == '?') {
                return {Q_MARK, "?"};
            } else if (c == '(') {
                return {LEFT_PAREN, "("};
            } else if (c == ')') {
                return {RIGHT_PAREN, ")"};
            } else if (c == '.') {
                return {PERIOD, "."};
            } else if (c == '\'') {
                tokenValue += c;
                while (input.get(c) && c != '\'') {
                    tokenValue += c;
                }
                tokenValue += c;
                return {STRING, tokenValue};
            } else if (isalpha(c)) {
                tokenValue += c;
                while (input.get(c) && isalnum(c)) {
                    tokenValue += c;
                }
                input.putback(c);
                if (tokenValue == "Schemes") {
                    return {SCHEMES, "Schemes"};
                } else if (tokenValue == "Facts") {
                    return {FACTS, "Facts"};
                } else if (tokenValue == "Rules") {
                    return {RULES, "Rules"};
                } else if (tokenValue == "Queries") {
                    return {QUERIES, "Queries"};
                }
                return {ID, tokenValue};
            } else {
                throw std::runtime_error("Invalid character encountered.");
            }
        }
        return {EOF_TOKEN, ""};
    }

private:
    std::istream& input;
};

#endif
