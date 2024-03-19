#pragma once
#include <string>
#include <vector>
#include "Token.h"
#include "Predicate.h"

using namespace std;

class Rule {
public:
  Rule(vector<Token> tokens, int i){
    index = i;
    match(tokens[index], ID);
    head_id = tokens[index-1].getValue();
    match(tokens[index], LEFT_PAREN);
    match(tokens[index], ID);
    head_id_list.push_back(tokens[index-1].getValue());
    while(tokens[index].getType() != RIGHT_PAREN){
      match(tokens[index], COMMA);
      match(tokens[index], ID);
      head_id_list.push_back(tokens[index-1].getValue());
    }
    match(tokens[index], RIGHT_PAREN);
    match(tokens[index], COLON_DASH);
    Predicate p = Predicate(tokens, index);
    predicate_list.push_back(p);
    index = p.getIndex();
    while(tokens[index].getType() != PERIOD){
      match(tokens[index], COMMA);
      Predicate p = Predicate(tokens, index);
      predicate_list.push_back(p);
      index = p.getIndex();
    }
    match(tokens[index], PERIOD);
  }
  ~Rule(){}

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
    output += head_id+"(";
    output += head_id_list[0];
    for (int i = 1; i < head_id_list.size(); i++){
      output += "," + head_id_list[i];
    }
    output += ") :- ";
    output += predicate_list[0].toString();
    for (int i = 1; i<predicate_list.size(); i++){
      output += "," + predicate_list[i].toString();
    }
    output += ".";
    return output;
  }

  enum tokenType {COMMA, PERIOD, Q_MARK, LEFT_PAREN, RIGHT_PAREN, COLON, COLON_DASH, MULTIPLY, ADD, SCHEMES,	FACTS, RULES, QUERIES, ID, STRING, COMMENT, WHITESPACE, UNDEFINED, EOTF};

private:
  int index;
  string head_id;
  vector<string> head_id_list;
  vector<Predicate> predicate_list;
};
