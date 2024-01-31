// Scanner.h
#include <string>
#include "Token.h"

class Scanner {
private:
    std::string input;
    int line = 1; // Track current line number
public:
    Scanner(const std::string& input) : input(input) {}

    Token scanToken() {

        if (input.empty()) {
            return Token(EOF_TOKEN, "", line);
        }
        while (!input.empty()) {
            char c = input.front();
            input = input.substr(1); // Move to the next character

            // Handle white space
            if (isspace(c)) {
                if (c == '\n') line++; // Increment line number for newlines
                continue; // Skip white space
            }

            // Handle single-character tokens
            switch (c) {
                case ',':
                    return Token(COMMA, ",", line);
                case '.':
                    return Token(PERIOD, ".", line);
                case '?':
                    return Token(Q_MARK, "?", line);
                case '(':
                    return Token(LEFT_PAREN, "(", line);
                case ')':
                    return Token(RIGHT_PAREN, ")", line);
                case ':':
                    if (!input.empty() && input.front() == '-') { // Check for ":-"
                        input = input.substr(1);
                        return Token(COLON_DASH, ":-", line);
                    }
                    return Token(COLON, ":", line);
                case '+':
                    return Token(ADD, "+", line);
                case '*':
                    return Token(MULTIPLY, "*", line);
                case '\'':
                    return scanString(); // Handle strings
                case '#':
                    return scanComment(); // Handle comments
                default:
                    if (isalpha(c)) {
                        return scanIdentifier(c); // Handle identifiers
                    }
                    // Handle undefined tokens
                    return Token(UNDEFINED, std::string(1, c), line);
            }
        }
        // If input is empty, return EOF token. But it isn't working for some reason.
        return Token(EOF_TOKEN, "", line);
    }

    Token scanString() {
        std::string value;
        value += '\''; // Add the opening single quote
        while (!input.empty()) {
            char c = input.front();
            input = input.substr(1);
            if (c == '\'') {
                if (!input.empty() && input.front() == '\'') { // Check for escaped apostrophe
                    value += c;
                    input = input.substr(1); // Skip the second apostrophe
                } else {
                    value += c; // Add the closing single quote
                    return Token(STRING, value, line);
                }
            } else {
                value += c;
            }
        }
        // If string is not terminated, return undefined token
        return Token(UNDEFINED, value, line);
    }

    Token scanComment() {
        char c;
        std::string value;
        if (!input.empty() && input.front() == '|') { // Check for block comment
            input = input.substr(1); // Skip the first '|'
            while (!input.empty()) {
                c = input.front();
                input = input.substr(1);
                if (c == '|' && !input.empty() && input.front() == '#') { // Check for end of block comment
                    input = input.substr(1); // Skip the second '|'
                    return Token(COMMENT, value, line);
                } else {
                    if (c == '\n') line++; // Increment line number for newlines in block comment
                    value += c;
                }
            }
            // If block comment is not terminated, return undefined token
            return Token(UNDEFINED, value, line);
        } else { // Line comment
            while (!input.empty() && input.front() != '\n') {
                value += input.front();
                input = input.substr(1);
            }
            return Token(COMMENT, value, line);
        }
    }

    Token scanIdentifier(char firstChar) {
        std::string value;
        value += firstChar; // Add the first character
        while (!input.empty() && (isalnum(input.front()) || input.front() == '_')) {
            value += input.front();
            input = input.substr(1);
        }
        // Check if the identifier is a keyword
        if (value == "Schemes") return Token(SCHEMES, value, line);
        if (value == "Facts") return Token(FACTS, value, line);
        if (value == "Rules") return Token(RULES, value, line);
        if (value == "Queries") return Token(QUERIES, value, line);
        // Otherwise, it's a regular identifier
        return Token(ID, value, line);
    }
};
