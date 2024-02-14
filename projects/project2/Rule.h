#ifndef RULE_H
#define RULE_H

#include <string>
#include <vector>
#include "Predicate.h"

class Rule {
public:
    Predicate headPredicate;
    std::vector<Predicate> bodyPredicates;

    Rule(const Predicate& head, const std::vector<Predicate>& body);

    std::string toString() const;
};

#endif