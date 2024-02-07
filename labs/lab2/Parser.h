#include vector
#include Token.h

class Parser {
 private:
  vector<Token> tokens;
 public:
  Parser(const vector<Token>& tokens) : tokens(tokens) { }
};