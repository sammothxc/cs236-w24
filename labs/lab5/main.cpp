// main.cpp

#include <iostream>
#include "Interpreter.h"

int main() {
    // Predicate names for fake rules
    // First is name for head predicate
    // Second is names for body predicates
    std::pair<std::string, std::vector<std::string>> ruleNames[] = {
        { "A", { "B" } },
        { "B", { "B", "A" } },
    };

    std::vector<Rule> rules;

    for (auto& rulePair : ruleNames) {
        std::string headName = rulePair.first;
        Rule rule = Rule(Predicate(headName));
        std::vector<std::string> bodyNames = rulePair.second;
        for (auto& bodyName : bodyNames)
            rule.addBodyPredicate(Predicate(bodyName));
        rules.push_back(rule);
    }

    Graph graph = Interpreter::makeGraph(rules);
    std::cout << graph.toString();

    return 0;
}
