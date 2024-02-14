#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include "Token.h"
#include "DatalogProgram.h"

class Parser {
private:
    std::vector<Token> tokens;
    size_t currentTokenIndex;

    Token getNextToken();
    void match(TokenType expectedType);
    Predicate parsePredicate();
    std::vector<Predicate> parsePredicateList();
    Rule parseRule();

public:
    Parser(const std::vector<Token>& t);

    DatalogProgram parse();
};

#endif