#include <iostream>
#include <vector>
#include "Parser.h"
#include "Token.h"
using namespace std;

int main() {

  vector<Token> tokens = {
    Token(ID,"Ned",2),
    Token(LEFT_PAREN,"(",2),
    Token(RIGHT_PAREN,")",2),
  };

  Parser p = Parser(tokens);
  p.match(ID);
  p.match(LEFT_PAREN);
  p.match(ID);         // intentional error
  p.match(RIGHT_PAREN);

}