#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include "Token.h"
#include "Parser.h"
#include "Scanner.h"

using namespace std;

class Scanner {
private:
    vector<Token> tokens;
    int line = 1;

    bool isAlpha(char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
    }

    bool isDigit(char c) {
        return c >= '0' && c <= '9';
    }

    bool isSpace(char c) {
        return c == ' ' || c == '\t' || c == '\n' || c == '\r';
    }

    void skipWhitespace(const string& input, size_t& pos) {
        while (pos < input.size() && isSpace(input[pos])) {
            if (input[pos] == '\n') line++;
            pos++;
        }
    }

    void parseToken(const string& input, size_t& pos) {
        skipWhitespace(input, pos);

        if (pos >= input.size()) return;

        char currentChar = input[pos];

        switch (currentChar) {
            case ':':
                if (pos + 1 < input.size() && input[pos + 1] == '-') {
                    tokens.emplace_back(COLON_DASH, ":-", line);
                    pos += 2;
                } else {
                    tokens.emplace_back(COLON, ":", line);
                    pos++;
                }
                break;
            case '(':
                tokens.emplace_back(LEFT_PAREN, "(", line);
                pos++;
                break;
            case ')':
                tokens.emplace_back(RIGHT_PAREN, ")", line);
                pos++;
                break;
            case ',':
                tokens.emplace_back(COMMA, ",", line);
                pos++;
                break;
            case '.':
                tokens.emplace_back(PERIOD, ".", line);
                pos++;
                break;
            case '?':
                tokens.emplace_back(Q_MARK, "?", line);
                pos++;
                break;
            default:
                if (isAlpha(currentChar)) {
                    string id;
                    while (pos < input.size() && (isAlpha(input[pos]) || isDigit(input[pos]))) {
                        id += input[pos];
                        pos++;
                    }
                    if (id == "Schemes") tokens.emplace_back(SCHEMES, id, line);
                    else if (id == "Facts") tokens.emplace_back(FACTS, id, line);
                    else if (id == "Rules") tokens.emplace_back(RULES, id, line);
                    else if (id == "Queries") tokens.emplace_back(QUERIES, id, line);
                    else tokens.emplace_back(ID, id, line);
                } else if (currentChar == '\'') {
                    string str;
                    pos++; // skip opening quote
                    while (pos < input.size() && input[pos] != '\'') {
                        str += input[pos];
                        pos++;
                    }
                    if (pos < input.size()) pos++; // skip closing quote
                    tokens.emplace_back(STRING, str, line);
                } else {
                    tokens.emplace_back(UNKNOWN, string(1, currentChar), line);
                    pos++;
                }
        }
    }

public:
    Scanner(const string& input) {
        size_t pos = 0;
        while (pos < input.size()) {
            parseToken(input, pos);
        }
        tokens.emplace_back(EOF_TOKEN, "", line);
    }

    const vector<Token>& getTokens() const {
        return tokens;
    }
};

class Parameter {
public:
    string value;

    Parameter(const string& v) : value(v) {}

    string toString() const {
        return value;
    }
};

class Predicate {
public:
    string name;
    vector<Parameter> parameters;

    Predicate(const string& n, const vector<Parameter>& p) : name(n), parameters(p) {}

    string toString() const {
        string result = name + "(";
        for (size_t i = 0; i < parameters.size(); ++i) {
            result += parameters[i].toString();
            if (i != parameters.size() - 1) result += ",";
        }
        result += ")";
        return result;
    }
};

class Rule {
public:
    Predicate headPredicate;
    vector<Predicate> bodyPredicates;

    Rule(const Predicate& head, const vector<Predicate>& body) : headPredicate(head), bodyPredicates(body) {}

    string toString() const {
        string result = headPredicate.toString() + " :- ";
        for (size_t i = 0; i < bodyPredicates.size(); ++i) {
            result += bodyPredicates[i].toString();
            if (i != bodyPredicates.size() - 1) result += ",";
        }
        result += ".";
        return result;
    }
};

class DatalogProgram {
public:
    vector<Predicate> schemes;
    vector<Predicate> facts;
    vector<Rule> rules;
    vector<Predicate> queries;
    set<string> domain;

    void addScheme(const Predicate& scheme) {
        schemes.push_back(scheme);
    }

    void addFact(const Predicate& fact) {
        facts.push_back(fact);
        for (const auto& param : fact.parameters) {
            if (param.value[0] == '\'') domain.insert(param.value);
        }
    }

    void addRule(const Rule& rule) {
        rules.push_back(rule);
    }

    void addQuery(const Predicate& query) {
        queries.push_back(query);
    }

    string toString() const {
        string result = "Success!\n";
        result += "Schemes(" + to_string(schemes.size()) + "):\n";
        for (const auto& scheme : schemes) {
            result += "  " + scheme.toString() + "\n";
        }
        result += "Facts(" + to_string(facts.size()) + "):\n";
        for (const auto& fact : facts) {
            result += "  " + fact.toString() + "\n";
        }
        result += "Rules(" + to_string(rules.size()) + "):\n";
        for (const auto& rule : rules) {
            result += "  " + rule.toString() + "\n";
        }
        result += "Queries(" + to_string(queries.size()) + "):\n";
        for (const auto& query : queries) {
            result += "  " + query.toString() + "\n";
        }
        result += "Domain(" + to_string(domain.size()) + "):\n";
        for (const auto& value : domain) {
            result += "  " + value + "\n";
        }
        return result;
    }
};

int main() {
    string input = "Schemes:\n  snap(S,N,A,P)\n  HasSameAddress(X,Y)\n\nFacts:\n  snap('12345','C. Brown','12 Apple','555-1234').\n  snap('33333','Snoopy','12 Apple','555-1234').\n\nRules:\n  HasSameAddress(X,Y) :- snap(A,X,B,C),snap(D,Y,B,E).\n\nQueries:\n  HasSameAddress('Snoopy',Who)?";

    try {
        Scanner scanner(input);
        Parser parser(scanner.getTokens());
        DatalogProgram program = parser.parse();
        cout << program.toString() << endl;
    } catch (const exception& e) {
        cout << "Failure!" << endl;
        cout << "  (" << e.what() << ")" << endl;
    }

    return 0;
}
