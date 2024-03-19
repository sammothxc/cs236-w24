#pragma once
#include <string>
#include <vector>
#include "Token.h"

using namespace std;

class Scheme {
public:
  Scheme(vector<Token> tokens, int i){
    index = i;
    match(tokens[index], ID);
    id = tokens[index-1].getValue();
    match(tokens[index], LEFT_PAREN);
    match(tokens[index], ID);
    id_list.push_back(tokens[index-1].getValue());
    while (tokens[index].getType() != RIGHT_PAREN){
      match(tokens[index], COMMA);
      match(tokens[index], ID);
      id_list.push_back(tokens[index-1].getValue());
    }
    match(tokens[index], RIGHT_PAREN);
  }
  ~Scheme(){}

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
    output += id+"("+id_list[0];
    for (int i = 1; i < id_list.size(); i++){
      output += ","+id_list[i];
    }
    output += ")";
    return output;
  }

  enum tokenType {COMMA, PERIOD, Q_MARK, LEFT_PAREN, RIGHT_PAREN, COLON, COLON_DASH, MULTIPLY, ADD, SCHEMES,	FACTS, RULES, QUERIES, ID, STRING, COMMENT, WHITESPACE, UNDEFINED, EOTF};

private:
  int index;
  string id;
  vector<string> id_list;
};
