#pragma once
#include <string>
#include <vector>
#include "Token.h"
#include "Predicate.h"

using namespace std;

class Query {
public:
  Query(vector<Token> tokens, int i){
    index = i;
    query = new Predicate(tokens, index);
    index = query->getIndex();
    match(tokens[index], Q_MARK);
  }
  ~Query(){}

  void match(Token token, int type){
    if (token.getType() == type){
      index++;
    } else  {
      throw(token);
    }
  }

  int getIndex(){
    return index;
  }

  string toString(){
    string output = "";
    output += query->toString();
    output += "?";
    return output;
  }

  enum tokenType {COMMA, PERIOD, Q_MARK, LEFT_PAREN, RIGHT_PAREN, COLON, COLON_DASH, MULTIPLY, ADD, SCHEMES,	FACTS, RULES, QUERIES, ID, STRING, COMMENT, WHITESPACE, UNDEFINED, EOTF};

private:
  int index;
  Predicate *query;
};
