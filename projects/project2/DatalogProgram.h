#ifndef DATALOGPROGRAM_H
#define DATALOGPROGRAM_H

#include <vector>
#include <set>
#include "Predicate.h"
#include "Rule.h"

class DatalogProgram {
public:
    std::vector<Predicate> schemes;
    std::vector<Predicate> facts;
    std::vector<Rule> rules;
    std::vector<Predicate> queries;
    std::set<std::string> domain;

    void addScheme(const Predicate& scheme);
    void addFact(const Predicate& fact);
    void addRule(const Rule& rule);
    void addQuery(const Predicate& query);

    std::string toString() const;
};

#endif