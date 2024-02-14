#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <string>

using namespace std;

enum TokenType {
    SCHEMES, FACTS, RULES, QUERIES, ID, STRING, COLON, LEFT_PAREN, RIGHT_PAREN,
    COMMA, PERIOD, COLON_DASH, Q_MARK, EOF_TOKEN, UNKNOWN
};

class Token {
public:
    TokenType type;
    string value;
    int line;

    Token(TokenType t, const string& v, int l) : type(t), value(v), line(l) {}
};
