#ifndef DATALOGPROGRAM_H
#define DATALOGPROGRAM_H

#include <vector>
#include <set>
#include "Predicate.h"
#include "Rule.h"

class DatalogProgram {
public:
    void addScheme(Predicate scheme) {
        schemes.push_back(scheme);
    }

    void addFact(Predicate fact) {
        facts.push_back(fact);
    }

    void addRule(Rule rule) {
        rules.push_back(rule);
    }

    void addQuery(Predicate query) {
        queries.push_back(query);
    }

    void addDomainValue(const std::string& value) {
        domainValues.insert(value);
    }

    std::string toString() const {
        std::string result = "Schemes(" + std::to_string(schemes.size()) + "):\n";
        for (const auto& scheme : schemes) {
            result += "  " + scheme.toString() + "\n";
        }

        result += "Facts(" + std::to_string(facts.size()) + "):\n";
        for (const auto& fact : facts) {
            result += "  " + fact.toString() + "\n";
        }

        result += "Rules(" + std::to_string(rules.size()) + "):\n";
        for (const auto& rule : rules) {
            result += "  " + rule.toString() + "\n";
        }

        result += "Queries(" + std::to_string(queries.size()) + "):\n";
        for (const auto& query : queries) {
            result += "  " + query.toString() + "\n";
        }

        result += "Domain(" + std::to_string(domainValues.size()) + "):\n";
        for (const auto& value : domainValues) {
            result += "  " + value + "\n";
        }

        return result;
    }

private:
    std::vector<Predicate> schemes;
    std::vector<Predicate> facts;
    std::vector<Rule> rules;
    std::vector<Predicate> queries;
    std::set<std::string> domainValues;
};

#endif