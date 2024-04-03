#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <utility>
#include "Graph.h"
#include "Rule.h"
#include "Predicate.h"

class Interpreter {
public:
    static Graph makeGraph(const std::vector<Rule>& rules) {
        Graph graph(rules.size());

        // Loop over rules
        for (size_t i = 0; i < rules.size(); ++i) {
            const Rule& fromRule = rules[i];

            // Loop over predicates in the body
            for (const Predicate& bodyPredicate : fromRule.getBodyPredicates()) {

                // Loop over rules again
                for (size_t j = 0; j < rules.size(); ++j) {
                    const Rule& toRule = rules[j];

                    // Find matching names and add edge
                    if (fromRule.getHeadPredicate().getName() == bodyPredicate.getName() &&
                        toRule.getHeadPredicate().getName() == bodyPredicate.getName()) {
                        graph.addEdge(i, j);
                    }
                }
            }
        }

        return graph;
    }
};

int main() {
    // predicate names for fake rules
    // first is name for head predicate
    // second is names for body predicates
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
