#ifndef RULE_H
#define RULE_H

#include <string>
#include <vector>
#include "Predicate.h"

class Rule {
public:
    Rule(Predicate headPredicate) : headPredicate(headPredicate) {}

    void addPredicate(Predicate predicate) {
        predicates.push_back(predicate);
    }

    std::string toString() const {
        std::string result = headPredicate.toString() + " :- ";
        for (size_t i = 0; i < predicates.size(); ++i) {
            result += predicates[i].toString();
            if (i != predicates.size() - 1) {
                result += ",";
            }
        }
        result += ".";
        return result;
    }

private:
    Predicate headPredicate;
    std::vector<Predicate> predicates;
};

#endif