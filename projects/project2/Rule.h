#pragma once
#include <string>
#include <vector>
#include "Predicate.h"

class Rule {
private:
    Predicate headPredicate;
    std::vector<Predicate> predicates;

public:
    Rule(const Predicate& head, const std::vector<Predicate>& preds) : headPredicate(head), predicates(preds) {}

    std::string toString() const {
        std::string result = headPredicate.toString() + " :- ";
        for (size_t i = 0; i < predicates.size(); ++i) {
            if (i > 0) result += ",";
            result += predicates[i].toString();
        }
        result += ".";
        return result;
    }
};