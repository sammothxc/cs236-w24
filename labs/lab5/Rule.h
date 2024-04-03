// Rule.h
#ifndef RULE_H
#define RULE_H

#include "Predicate.h"
#include <vector>
#include <string>

class Rule {
private:
    Predicate headPredicate;
    std::vector<Predicate> bodyPredicates;

public:
    Rule(const Predicate& head) : headPredicate(head) {}

    void addBodyPredicate(const Predicate& body) {
        bodyPredicates.push_back(body);
    }

    Predicate getHeadPredicate() const {
        return headPredicate;
    }

    std::vector<Predicate> getBodyPredicates() const {
        return bodyPredicates;
    }

    std::string toString() const {
        std::string result = headPredicate.toString() + " :- ";
        for (size_t i = 0; i < bodyPredicates.size(); ++i) {
            if (i > 0) {
                result += ",";
            }
            result += bodyPredicates[i].toString();
        }
        return result;
    }
};

#endif // RULE_H
