#pragma once
#include <string>
#include <vector>
#include <set>
#include "Token.h"

using namespace std;

class Fact {
public:
  Fact(vector<Token> tokens, int i){
    index = i;
    match(tokens[index], ID);
    id = tokens[index-1].getValue();
    match(tokens[index], LEFT_PAREN);
    match(tokens[index], STRING);
    string_list.push_back(tokens[index-1].getValue());
    domain.insert(tokens[index-1].getValue());
    while(tokens[index].getType() != RIGHT_PAREN){
      match(tokens[index], COMMA);
      match(tokens[index], STRING);
      string_list.push_back(tokens[index-1].getValue());
      domain.insert(tokens[index-1].getValue());
    }
    match(tokens[index], RIGHT_PAREN);
    match(tokens[index], PERIOD);
  }
  ~Fact(){}

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

  set<string> getDomain(){
    return domain;
  }

  string toString(){
    string output = "";
    output += id+"("+string_list[0];
    for (int i = 1; i < string_list.size(); i++){
      output += ","+string_list[i];
    }
    output += ").";

    return output;
  }

  enum tokenType {COMMA, PERIOD, Q_MARK, LEFT_PAREN, RIGHT_PAREN, COLON, COLON_DASH, MULTIPLY, ADD, SCHEMES,	FACTS, RULES, QUERIES, ID, STRING, COMMENT, WHITESPACE, UNDEFINED, EOTF};

private:
  int index;
  string id;
  vector<string> string_list;
  set<string> domain;
};
