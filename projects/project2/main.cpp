#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include "Token.h"
#include "Parser.h"
#include "Scanner.h"

using namespace std;

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
