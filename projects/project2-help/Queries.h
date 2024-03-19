#pragma once
#include <string>
#include <vector>
#include <set>
#include <iostream>
#include "Token.h"
#include "Query.h"

using namespace std;

class Queries {
public:
  Queries(vector<Token> tokens, int i){
    index = i;
    match(tokens[index], QUERIES);
    match(tokens[index], COLON);
    Query q = Query(tokens, index);
    queries.push_back(q);
    index = q.getIndex();
    while(tokens[index].getType() != EOTF){
      Query q = Query(tokens, index);
      queries.push_back(q);
      index = q.getIndex();
    }
    match(tokens[index], EOTF);
  }
  ~Queries(){}

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

  vector<Query> getQueries(){
    return queries;
  }

  string toString(){
    string output = "";
    output += "Queries("+to_string(queries.size())+"):\n";
    for (int i = 0; i < queries.size(); i++){
      output += "  "+queries[i].toString()+"\n";
    }
    return output;
  }

  enum tokenType {COMMA, PERIOD, Q_MARK, LEFT_PAREN, RIGHT_PAREN, COLON, COLON_DASH, MULTIPLY, ADD, SCHEMES,	FACTS, RULES, QUERIES, ID, STRING, COMMENT, WHITESPACE, UNDEFINED, EOTF};


private:
  vector<Query> queries;
  int index;
};
