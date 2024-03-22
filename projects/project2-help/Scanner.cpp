#include <string>
#include <vector>
#include <iostream>
#include "Scanner.h"

using namespace std;

Scanner::Scanner(){}
Scanner::~Scanner(){
  tokens.clear();
  file.close();
}

//Uses state machines to retreive tokens
vector<Token> Scanner::scan(string input){
  file.open(input);
  line = 1;
  while (in != -1){
    start = 0;
    in = file.peek();
    isWhiteSpace();
    switch(in){
      case -1:
        type = EOTF;
        value = "";
        start = line;
        break;
      case ':':
        if (colonDashMachine(":") > 0){
          type = COLON_DASH;
          value = getValue(colonDashMachine(":"));
          start = line;
          break;
        } else {
          type = COLON;
          value = getValue(colonMachine(":"));
          start = line;
          break;
        }
      case ',':
        type = COMMA;
        value = getValue(commaMachine(","));
        start = line;
        break;
      case '.':
        type = PERIOD;
        value = getValue(periodMachine("."));
        start = line;
        break;
      case '?':
        type = Q_MARK;
        value = getValue(qMarkMachine("?"));
        start = line;
        break;
      case '(':
        type = LEFT_PAREN;
        value = getValue(leftParanMachine("("));
        start = line;
        break;
      case ')':
        type = RIGHT_PAREN;
        value = getValue(rightParanMachine(")"));
        start = line;
        break;
      case '*':
        type = MULTIPLY;
        value = getValue(multiplyMachine("*"));
        start = line;
        break;
      case '+':
        type = ADD;
        value = getValue(addMachine("+"));
        start = line;
        break;
      case '\'':
        stringMachine();
        break;
      case '#':
        commentMachine();
        break;
      default:
        scanForKeywords();
    }
    if ( type != COMMENT ){
      Token t = Token(type,value,start);
      tokens.push_back(t);
    }
  }
  return tokens;
}

//Output Functions
string Scanner::outputAllTokens(){
  string output = "";
  for(vector<Token>::size_type i = 0; i < tokens.size(); i++){
    output += tokens[i].output() + "\n";
  }
  output += "Total Tokens = "+outputTokenCount();
  return output;
}

string Scanner::outputTokenCount(){
  return to_string(tokens.size());
}


// Helper Functions
string Scanner::getValue(int length){
  int iter = 0;
  string value = "";
  while(iter < length){
    value += file.get();
    iter++;
  }
  return value;
}

void Scanner::isWhiteSpace(){
  while (isspace(in)){
    if (in == '\n'){
      line++;
    }
    file.get();
    in = file.peek();
  }
}

// Finite State Machines (returns length of token value if valid or -1 if not)
int Scanner::colonDashMachine(const string input){
  file.get();
  if (file.peek() == '-'){
    file.unget();
    return 2;
  } else {
    file.unget();
  }
  return 0;
}
void Scanner::scanForKeywords(){
  keyword = "";
  if (isalpha(in)){
    while (isalpha(in) || isdigit(in)){
      keyword += in;
      file.get();
      in = file.peek();
    }
    if (keyword == "Schemes"){
      type = SCHEMES;
      value = keyword;
      start = line;
    } else if (keyword == "Rules"){
      type = RULES;
      value = keyword;
      start = line;
    } else if (keyword == "Facts"){
      type = FACTS;
      value = keyword;
      start = line;
    } else if (keyword == "Queries"){
      type = QUERIES;
      value = keyword;
      start = line;
    } else {
      type = ID;
      value = keyword;
      start = line;
    }
  } else {
    file.get();
    keyword += in;
    type = UNDEFINED;
    value = keyword;
    start = line;
  }
}
void Scanner::stringMachine(){
  str = "";
  start = line;
  str += in;
  file.get();
  in = file.peek();
  while (in != -1){
    if (in == '\n'){ line++; }
    if (in == '\''){
      str += in;
      file.get();
      if (file.peek() == '\''){
        in = file.peek();
      } else {
        type = STRING;
        value = str;
        return;
      }
    }
    str += in;
    file.get();
    in = file.peek();
  }
  in = 0;
  type = UNDEFINED;
  value = str;
}
void Scanner::commentMachine(){
  str = "";
  start = line;
  str += in;
  file.get();
  in = file.peek();
  if (file.peek() == '|'){
    str += in;
    file.get();
    in = file.peek();
    while (in != -1){
      if (in == '\n'){ line ++; }
      if (in == '|'){
        str += in;
        file.get();
        in = file.peek();
        if (in == '#'){
          str += in;
          file.get();
          type = COMMENT;
          value = str;
          return;
        }
      } else {
        str += in;
        file.get();
        in = file.peek();
      }
    }
    in = 0;
    type = UNDEFINED;
    value = str;
  } else {
    int x;
    while (in != '\n' && in != -1){
      x = in;
      str += in;
      file.get();
      in = file.peek();
    }
    if (in == -1){
      in = x;
    }
    type = COMMENT;
    value = str;
  }
}
int Scanner::commaMachine(const string input){
  if (input == ",")
    return 1;
  return -1;
}
int Scanner::periodMachine(const string input){
  if (input == ".")
    return 1;
  return -1;
}
int Scanner::qMarkMachine(const string input){
  if (input == "?")
    return 1;
  return -1;
}
int Scanner::leftParanMachine(const string input){
  if (input == "(")
    return 1;
  return -1;
}
int Scanner::rightParanMachine(const string input){
  if (input == ")")
    return 1;
  return -1;
}
int Scanner::colonMachine(const string input){
  if (input == ":")
    return 1;
  return -1;
}
int Scanner::multiplyMachine(const string input){
  if (input == "*")
    return 1;
  return -1;
}
int Scanner::addMachine(const string input){
  if (input == "+")
    return 1;
  return -1;
}
