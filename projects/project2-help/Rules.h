#pragma once
#include <string>
#include <vector>
#include <set>
#include <iostream>
#include "Token.h"
#include "Rule.h"

using namespace std;

class Rules {
public:
  Rules(vector<Token> tokens, int i){
    index = i;
    match(tokens[index], RULES);
    match(tokens[index], COLON);
    while(tokens[index].getType() != QUERIES){
      Rule r = Rule(tokens, index);
      rules.push_back(r);
      index = r.getIndex();
    }
  }
  ~Rules(){}

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

  vector<Rule> getRules(){
    return rules;
  }

  string toString(){
    string output = "";
    output += "Rules("+to_string(rules.size())+"):\n";
    for (int i = 0; i < rules.size(); i++){
      output += "  "+rules[i].toString()+"\n";
    }
    return output;
  }

  enum tokenType {COMMA, PERIOD, Q_MARK, LEFT_PAREN, RIGHT_PAREN, COLON, COLON_DASH, MULTIPLY, ADD, SCHEMES,	FACTS, RULES, QUERIES, ID, STRING, COMMENT, WHITESPACE, UNDEFINED, EOTF};


private:
  vector<Rule> rules;
  int index;
};
