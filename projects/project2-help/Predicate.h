#pragma once
#include <string>
#include <vector>
#include "Token.h"
#include "Parameter.h"
#include "Id.h"
#include "String.h"
#include "Expression.h"

using namespace std;

class Predicate {
public:
  Predicate(vector<Token> tokens, int i){
    index = i;
    match(tokens[index], ID);
    id = tokens[index-1].getValue();
    match(tokens[index], LEFT_PAREN);
    if (tokens[index].getType() == STRING){
      match(tokens[index], STRING);
      Parameter *p = new String(tokens[index-1].getValue());
      parameter_list.push_back(p);
    } else if (tokens[index].getType() == ID){
      match(tokens[index], ID);
      Parameter *p = new Id(tokens[index-1].getValue());
      parameter_list.push_back(p);
    } else {
      Parameter *p = new Expression(tokens, index);
      parameter_list.push_back(p);
      index = p->getIndex();
    }
    while (tokens[index].getType() != RIGHT_PAREN){
      match(tokens[index], COMMA);
      if (tokens[index].getType() == STRING){
        match(tokens[index], STRING);
        Parameter *p = new String(tokens[index-1].getValue());
        parameter_list.push_back(p);
      } else if (tokens[index].getType() == ID){
        match(tokens[index], ID);
        Parameter *p = new Id(tokens[index-1].getValue());
        parameter_list.push_back(p);
      } else {
        Parameter *p = new Expression(tokens, index);
        parameter_list.push_back(p);
        index = p->getIndex();
      }
    }
    match(tokens[index], RIGHT_PAREN);
  }
  ~Predicate(){}

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
    output += id + "(";
    output += parameter_list[0]->toString();
    for (int i = 1; i<parameter_list.size(); i++){
      output += "," + parameter_list[i]->toString();
    }
    output += ")";
    return output;
  }

  enum tokenType {COMMA, PERIOD, Q_MARK, LEFT_PAREN, RIGHT_PAREN, COLON, COLON_DASH, MULTIPLY, ADD, SCHEMES,	FACTS, RULES, QUERIES, ID, STRING, COMMENT, WHITESPACE, UNDEFINED, EOTF};

private:
  int index;
  string id;
  vector<Parameter*> parameter_list;
};
