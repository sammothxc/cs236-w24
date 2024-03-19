#pragma once
#include <string>
#include <vector>
#include <set>
#include <iostream>
#include "Token.h"
#include "Fact.h"

using namespace std;

class Facts {
public:
  Facts(vector<Token> tokens, int i){
    index = i;
    match(tokens[index], FACTS);
    match(tokens[index], COLON);
    while(tokens[index].getType() != RULES){
      Fact f = Fact(tokens, index);
      facts.push_back(f);
      set<string> d = f.getDomain();
      domain.insert(d.begin(), d.end());
      index = f.getIndex();
    }
  }
  ~Facts(){}

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

  vector<Fact> getFacts(){
    return facts;
  }

  string outputDomain(){
    string output = "";
    output += "Domain("+to_string(domain.size())+"):";
    for (set<string>::iterator it=domain.begin(); it!=domain.end(); ++it){
      output += "\n  "+ *it;
    }
    return output;
  }

  string toString(){
    string output = "";
    output += "Facts("+to_string(facts.size())+"):\n";
    for (int i = 0; i < facts.size(); i++){
      output += "  "+facts[i].toString()+"\n";
    }
    return output;
  }

  enum tokenType {COMMA, PERIOD, Q_MARK, LEFT_PAREN, RIGHT_PAREN, COLON, COLON_DASH, MULTIPLY, ADD, SCHEMES,	FACTS, RULES, QUERIES, ID, STRING, COMMENT, WHITESPACE, UNDEFINED, EOTF};


private:
  vector<Fact> facts;
  set<string> domain;
  int index;
};
