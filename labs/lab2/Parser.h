#include <vector>
#include <iostream>
#include "Token.h"
using namespace std;

class Parser {
private:
    vector<Token> tokens;
public:
    Parser(const vector<Token>& tokens) : tokens(tokens) { }

    TokenType tokenType() const {
        return tokens.at(0).getType();
    }

    void advanceToken() {
    tokens.erase(tokens.begin());
    }

    void throwError() {
        cout << "error" << endl;
    }

    void match(TokenType t) {
        cout << "match: " << t << endl;
        if (tokenType() == t) {
            advanceToken();
        } else {
            throwError();
        }
    }
};