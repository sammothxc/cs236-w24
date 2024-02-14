#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include "Token.h"
#include "DatalogProgram.h"
#include "Scanner.h"
#include "Predicate.h"

class Parser {
public:
    Parser(std::istream& input) : scanner(input) {}

    DatalogProgram parse() {
        DatalogProgram program;
        parseSchemes(program);
        parseFacts(program);
        parseRules(program);
        parseQueries(program);
        return program;
    }

private:
    void parseSchemes(DatalogProgram& program) {
        expect(SCHEMES);
        expect(COLON);
        parseSchemeList(program);
    }

    void parseSchemeList(DatalogProgram& program) {
        if (match(ID)) {
            program.addScheme(parseScheme());
            parseSchemeList(program);
        }
    }

    Predicate parseScheme() {
        std::string name = expect(ID).value;
        expect(LEFT_PAREN);
        Parameter parameter(ID, expect(ID).value);
        std::vector<Parameter> parameters;
        parameters.push_back(parameter);
        parseIdList(parameters);
        expect(RIGHT_PAREN);
        return Predicate(name, parameters);
    }

    void parseIdList(std::vector<Parameter>& parameters) {
        if (match(COMMA)) {
            expect(ID);
            Parameter parameter(ID, expect(ID).value);
            parameters.push_back(parameter);
            parseIdList(parameters);
        }
    }

    void parseFacts(DatalogProgram& program) {
        expect(FACTS);
        expect(COLON);
        parseFactList(program);
    }

    void parseFactList(DatalogProgram& program) {
        if (match(STRING)) {
            program.addFact(parseFact());
            parseFactList(program);
        }
    }

    Predicate parseFact() {
        std::string name = expect(ID).value;
        expect(LEFT_PAREN);
        Parameter parameter(STRING, expect(STRING).value);
        std::vector<Parameter> parameters;
        parameters.push_back(parameter);
        parseStringList(parameters);
        expect(RIGHT_PAREN);
        expect(PERIOD);
        return Predicate(name, parameters);
    }

    void parseStringList(std::vector<Parameter>& parameters) {
        if (match(COMMA)) {
            expect(STRING);
            Parameter parameter(STRING, expect(STRING).value);
            parameters.push_back(parameter);
            parseStringList(parameters);
        }
    }

    void parseRules(DatalogProgram& program) {
        expect(RULES);
        expect(COLON);
        parseRuleList(program);
    }

    void parseRuleList(DatalogProgram& program) {
        if (match(ID)) {
            program.addRule(parseRule());
            parseRuleList(program);
        }
    }

    Rule parseRule() {
        Predicate headPredicate = parseHeadPredicate();
        expect(COLON_DASH);
        Predicate predicate = parsePredicate();
        std::vector<Predicate> predicates;
        predicates.push_back(predicate);
        parsePredicateList(predicates);
        expect(PERIOD);
        Rule rule(headPredicate);
        for (const auto& pred : predicates) {
            rule.addPredicate(pred);
        }
        return rule;
    }

    Predicate parseHeadPredicate() {
        std::string name = expect(ID).value;
        expect(LEFT_PAREN);
        Parameter parameter(ID, expect(ID).value);
        std::vector<Parameter> parameters;
        parameters.push_back(parameter);
        parseIdList(parameters);
        expect(RIGHT_PAREN);
        return Predicate(name, parameters);
    }

    Predicate parsePredicate() {
        std::string name = expect(ID).value;
        expect(LEFT_PAREN);
        Parameter parameter = parseParameter();
        std::vector<Parameter> parameters;
        parameters.push_back(parameter);
        parseParameterList(parameters);
        expect(RIGHT_PAREN);
        return Predicate(name, parameters);
    }

    void parseParameterList(std::vector<Parameter>& parameters) {
        if (match(COMMA)) {
            Parameter parameter = parseParameter();
            parameters.push_back(parameter);
            parseParameterList(parameters);
        }
    }

    void parsePredicateList(std::vector<Predicate>& predicates) {
        if (match(COMMA)) {
            Predicate predicate = parsePredicate();
            predicates.push_back(predicate);
            parsePredicateList(predicates);
        }
    }

    Parameter parseParameter() {
        if (match(STRING)) {
            return Parameter(STRING, previousToken.value);
        } else if (match(ID)) {
            return Parameter(ID, previousToken.value);
        } else {
            throw std::runtime_error("Expected parameter.");
        }
    }

    void parseQueries(DatalogProgram& program) {
        expect(QUERIES);
        expect(COLON);
        parseQueryList(program);
    }

    void parseQueryList(DatalogProgram& program) {
        if (match(ID)) {
            program.addQuery(parsePredicate());
            expect(Q_MARK);
            parseQueryList(program);
        }
    }

    Token expect(TokenType expectedType) {
        Token token = scanner.getNextToken();
        if (token.type != expectedType) {
            throw std::runtime_error("Expected token of type " + getTokenTypeName(expectedType));
        }
        return token;
    }

    bool match(TokenType expectedType) {
        Token token = scanner.getNextToken();
        if (token.type == expectedType) {
            previousToken = token;
            return true;
        }
        scanner.putBackToken(token);
        return false;
    }

    std::string getTokenTypeName(TokenType type) {
        switch (type) {
            case SCHEMES:
                return "SCHEMES";
            case FACTS:
                return "FACTS";
            case RULES:
                return "RULES";
            case QUERIES:
                return "QUERIES";
            case ID:
                return "ID";
            case STRING:
                return "STRING";
            case COMMA:
                return "COMMA";
            case COLON:
                return "COLON";
            case COLON_DASH:
                return "COLON_DASH";
            case Q_MARK:
                return "Q_MARK";
            case LEFT_PAREN:
                return "LEFT_PAREN";
            case RIGHT_PAREN:
                return "RIGHT_PAREN";
            case PERIOD:
                return "PERIOD";
            case EOF_TOKEN:
                return "EOF";
            default:
                return "UNKNOWN";
        }
    }

    Scanner scanner;
    Token previousToken;
};

#endif