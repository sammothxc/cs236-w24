#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "Parameter.h"
#include "String.h"
#include "Id.h"
#include "Token.h"

using namespace std;

class Expression : public Parameter {

public:
  Expression(vector<Token> tokens, int i){
    index = i;
    match(tokens[index], LEFT_PAREN);
    if (tokens[index].getType() == STRING){
      match(tokens[index], STRING);
      parameter1 = new String(tokens[index-1].getValue());
    } else if (tokens[index].getType() == ID){
      match(tokens[index], ID);
      parameter1 = new Id(tokens[index-1].getValue());
    } else {
      parameter1 = new Expression(tokens, index);
      index = parameter1->getIndex();
    }

    if (tokens[index].getType() == ADD){
      match(tokens[index], ADD);
      oper = tokens[index-1].getValue();
    } else {
      match(tokens[index], MULTIPLY);
      oper = tokens[index-1].getValue();
    }

    if (tokens[index].getType() == STRING){
      match(tokens[index], STRING);
      parameter2 = new String(tokens[index-1].getValue());
    } else if (tokens[index].getType() == ID){
      match(tokens[index], ID);
      parameter2 = new Id(tokens[index-1].getValue());
    } else {
      parameter2 = new Expression(tokens, index);
      index = parameter2->getIndex();
    }
    match(tokens[index], RIGHT_PAREN);
  }
  ~Expression(){}

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
    output += "(";
    output += parameter1->toString();
    output += oper;
    output += parameter2->toString();
    output += ")";
    return output;
  }

  enum tokenType {COMMA, PERIOD, Q_MARK, LEFT_PAREN, RIGHT_PAREN, COLON, COLON_DASH, MULTIPLY, ADD, SCHEMES,	FACTS, RULES, QUERIES, ID, STRING, COMMENT, WHITESPACE, UNDEFINED, EOTF};

private:
  int index;
  Parameter *parameter1;
  Parameter *parameter2;
  string oper;
};
