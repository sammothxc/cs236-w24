#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <utility>
#include "Graph.h"
#include "Node.h"

class Rule {
private:
    std::string headPredicate;
    std::vector<std::string> bodyPredicates;

public:
    Rule(const std::string& head) : headPredicate(head) {}

    void addBodyPredicate(const std::string& body) {
        bodyPredicates.push_back(body);
    }

    std::string getHeadPredicate() const {
        return headPredicate;
    }

    std::vector<std::string> getBodyPredicates() const {
        return bodyPredicates;
    }
};

class Interpreter {
public:
    static Graph makeGraph(const std::vector<Rule>& rules) {
        Graph graph(rules.size());

        // Loop over rules
        for (size_t i = 0; i < rules.size(); ++i) {
            const Rule& fromRule = rules[i];

            // Print "from" rule
            std::cout << "from rule R" << i << ": " << fromRule.getHeadPredicate() << "() :- ";
            for (const auto& body : fromRule.getBodyPredicates()) {
                std::cout << body << "()";
                if (&body != &fromRule.getBodyPredicates().back()) {
                    std::cout << ",";
                }
            }
            std::cout << std::endl;

            // Loop over body predicates
            for (const auto& bodyPredicate : fromRule.getBodyPredicates()) {
                std::cout << "from body predicate: " << bodyPredicate << "()" << std::endl;

                // Loop over rules again
                for (size_t j = 0; j < rules.size(); ++j) {
                    const Rule& toRule = rules[j];

                    // Print "to" rule
                    std::cout << "to rule R" << j << ": " << toRule.getHeadPredicate() << "() :- ";
                    for (const auto& body : toRule.getBodyPredicates()) {
                        std::cout << body << "()";
                        if (&body != &toRule.getBodyPredicates().back()) {
                            std::cout << ",";
                        }
                    }
                    std::cout << std::endl;

                    // Check for matching predicate names
                    if (fromRule.getHeadPredicate() != toRule.getHeadPredicate()) {
                        bool dependencyFound = false;
                        for (const auto& toBodyPredicate : toRule.getBodyPredicates()) {
                            if (bodyPredicate == toBodyPredicate) {
                                graph.addEdge(i, j);
                                dependencyFound = true;
                                break;
                            }
                        }
                        if (dependencyFound) {
                            std::cout << "dependency found: (R" << i << ",R" << j << ")" << std::endl;
                        }
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
        { "A", { "B", "C" } },
        { "B", { "A", "D" } },
        { "B", { "B" } },
        { "E", { "F", "G" } },
        { "E", { "E", "F" } },
    };

    std::vector<Rule> rules;

    for (auto& rulePair : ruleNames) {
        std::string headName = rulePair.first;
        Rule rule(headName);
        std::vector<std::string> bodyNames = rulePair.second;
        for (auto& bodyName : bodyNames)
            rule.addBodyPredicate(bodyName);
        rules.push_back(rule);
    }

    Graph graph = Interpreter::makeGraph(rules);
    std::cout << graph.toString();

    return 0;
}
