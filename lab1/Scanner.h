#include <string>
#include <sstream>
#include <cctype>
#include "Token.h"
#pragma once
using namespace std;

class Scanner {
    private:
        string input;
    public:
        Scanner(const string& input) : input(input) { }
        Token scanToken() {
            while (!input.empty() && isspace(input.at(0))) {
                input = input.substr(1);
            }
            if (!input.empty() && input.at(0) == ',') {
                TokenType type = COMMA;
                string value = ",";
                int line = 4;
                input = input.substr(1);
                return Token(type, value, line);
        }
        return Token(UNKNOWN, "", -1);
    }
};