#pragma once
#include <string>
#include <fstream>
#include <vector>
#include "Token.h"

using namespace std;

class Scanner {
public:
  Scanner();
  ~Scanner();

  // Enumerated values for the token types
  enum tokenType {COMMA, PERIOD, Q_MARK, LEFT_PAREN, RIGHT_PAREN, COLON, COLON_DASH, MULTIPLY, ADD, SCHEMES,
	FACTS, RULES, QUERIES, ID, STRING, COMMENT, WHITESPACE, UNDEFINED, EOTF};

  // Uses state machines to retreive tokens
  vector<Token> scan(string input);

  // Output Functions
  string outputAllTokens();
  string outputTokenCount();

  // Helper Functions
  string getValue(int length);
  void isWhiteSpace();

  // Finite State Machines
  int commaMachine(const string input);
  int periodMachine(const string input);
  int qMarkMachine(const string input);
  int leftParanMachine(const string input);
  int rightParanMachine(const string input);
  int colonMachine(const string input);
  int colonDashMachine(const string input);
  int multiplyMachine(const string input);
  int addMachine(const string input);
  void stringMachine();
  void commentMachine();
  void scanForKeywords();




private:
  vector<Token> tokens;
  ifstream file;
  int type;
  string value;
  int line;
  int in;
  string keyword;
  int start;
  string str;
};
