#include <iostream>
#include "Interpreter.h"

int main() {
    std::pair<std::string, std::vector<std::string>> ruleNames2[] = {
        {"A", {"B", "C"}},
        {"B", {"A", "D"}},
        {"B", {"B"}},
        {"E", {"F", "G"}},
        {"E", {"E", "F"}},
    };

    std::vector<Rule> rules;

    for (auto& rulePair : ruleNames2) {
        std::string headName = rulePair.first;
        Rule rule = Rule(Predicate(headName));
        std::vector<std::string> bodyNames = rulePair.second;
        for (auto& bodyName : bodyNames)
            rule.addBodyPredicate(Predicate(bodyName));
        rules.push_back(rule);
    }

    Graph graph2 = Interpreter::makeGraph(rules);
    std::cout << graph2.toString();

    return 0;
}
