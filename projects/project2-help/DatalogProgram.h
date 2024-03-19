#pragma once
#include <string>
#include <vector>
#include <set>
#include "Token.h"
#include "Schemes.h"
#include "Facts.h"
#include "Rules.h"
#include "Queries.h"

using namespace std;

class DatalogProgram {
public:
  DatalogProgram(vector<Token> tokens){
    schemes = new Schemes(tokens);
    index = schemes->getIndex();
    facts = new Facts(tokens, index);
    index = facts->getIndex();
    rules = new Rules(tokens, index);
    index = rules->getIndex();
    queries = new Queries(tokens, index);
  }
  ~DatalogProgram(){}

  // Enumerated values for the token types
  enum tokenType {COMMA, PERIOD, Q_MARK, LEFT_PAREN, RIGHT_PAREN, COLON, COLON_DASH, MULTIPLY, ADD, SCHEMES,	FACTS, RULES, QUERIES, ID, STRING, COMMENT, WHITESPACE, UNDEFINED, EOTF};

  string toString(){
    string output = "";
    output += schemes->toString();
    output += facts->toString();
    output += rules->toString();
    output += queries->toString();
    output += facts->outputDomain();
  	return output;
  }

private:
  Schemes *schemes;
  Facts *facts;
  Rules *rules;
  Queries *queries;
  int index = 0;

};
