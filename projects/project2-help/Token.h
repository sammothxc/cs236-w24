#pragma once
#include <string>
#include <vector>

using namespace std;
// info on a Token to test
class Token {
public:
  Token(int t, string v, int lN);
  ~Token();

  int getType();
  string getValue();
  int getLineNumber();
  string output();

private:
  int type;
  string value;
  int lineNumber;
  string typeLiteral[19] = {"COMMA", "PERIOD", "Q_MARK", "LEFT_PAREN", "RIGHT_PAREN", "COLON", "COLON_DASH", "MULTIPLY", "ADD", "SCHEMES",
	"FACTS", "RULES", "QUERIES", "ID", "STRING", "COMMENT", "WHITESPACE", "UNDEFINED", "EOF"};
};
