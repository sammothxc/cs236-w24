#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "Token.h"
#include "Scheme.h"

using namespace std;

class Schemes {
public:
  Schemes(vector<Token> tokens){
    match(tokens[index], SCHEMES);
    match(tokens[index], COLON);
    Scheme s = Scheme(tokens, index);
    schemes.push_back(s);
    index = s.getIndex();
    while(tokens[index].getType() != FACTS){
      Scheme s = Scheme(tokens, index);
      schemes.push_back(s);
      index = s.getIndex();
    }
  }
  ~Schemes(){}

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

  vector<Scheme> getSchemes(){
    return schemes;
  }

  string toString(){
    string output = "";
    output += "Schemes("+to_string(schemes.size())+"):\n";
    for (int i = 0; i < schemes.size(); i++){
      output += "  "+schemes[i].toString()+"\n";
    }

    return output;
  }

  enum tokenType {COMMA, PERIOD, Q_MARK, LEFT_PAREN, RIGHT_PAREN, COLON, COLON_DASH, MULTIPLY, ADD, SCHEMES,	FACTS, RULES, QUERIES, ID, STRING, COMMENT, WHITESPACE, UNDEFINED, EOTF};


private:
  vector<Scheme> schemes;
  int index = 0;
};
