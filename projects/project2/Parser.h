#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

class Parser {
private:
    vector<Token> tokens;
    size_t currentTokenIndex = 0;

    Token getNextToken() {
        return tokens[currentTokenIndex++];
    }

    void match(TokenType expectedType) {
        Token token = getNextToken();
        if (token.type != expectedType) {
            throw runtime_error("Unexpected token: " + token.value);
        }
    }

    Predicate parsePredicate() {
        Token idToken = getNextToken();
        match(LEFT_PAREN);
        vector<Parameter> parameters;
        if (tokens[currentTokenIndex].type != RIGHT_PAREN) {
            parameters.push_back(Parameter(getNextToken().value));
            while (tokens[currentTokenIndex].type == COMMA) {
                match(COMMA);
                parameters.push_back(Parameter(getNextToken().value));
            }
        }
        match(RIGHT_PAREN);
        return Predicate(idToken.value, parameters);
    }

    vector<Predicate> parsePredicateList() {
        vector<Predicate> predicates;
        predicates.push_back(parsePredicate());
        while (tokens[currentTokenIndex].type == COMMA) {
            match(COMMA);
            predicates.push_back(parsePredicate());
        }
        return predicates;
    }

    Rule parseRule() {
        Predicate headPredicate = parsePredicate();
        match(COLON_DASH);
        vector<Predicate> bodyPredicates = parsePredicateList();
        match(PERIOD);
        return Rule(headPredicate, bodyPredicates);
    }

public:
    Parser(const vector<Token>& t) : tokens(t) {}

    DatalogProgram parse() {
        DatalogProgram program;
        match(SCHEMES);
        match(COLON);
        while (tokens[currentTokenIndex].type == ID) {
            program.addScheme(parsePredicate());
        }
        match(FACTS);
        match(COLON);
        while (tokens[currentTokenIndex].type == ID) {
            program.addFact(parsePredicate());
            match(PERIOD);
        }
        match(RULES);
        match(COLON);
        while (tokens[currentTokenIndex].type == ID) {
            program.addRule(parseRule());
        }
        match(QUERIES);
        match(COLON);
        while (tokens[currentTokenIndex].type == ID) {
            program.addQuery(parsePredicate());
            match(Q_MARK);
        }
        return program;
    }
};