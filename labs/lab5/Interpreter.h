// Interpreter.h
#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "Graph.h"
#include "Rule.h"
#include <vector>
#include <iostream>

class Interpreter {
public:
    static Graph makeGraph(const std::vector<Rule>& rules) {
        Graph graph(rules.size());
        for (size_t i = 0; i < rules.size(); ++i) {
            const Rule& fromRule = rules[i];
            std::cout << "from rule R" << i << ": " << fromRule.toString() << std::endl;
            for (const Predicate& bodyPredicate : fromRule.getBodyPredicates()) {
                std::cout << "from body predicate: " << bodyPredicate.toString() << std::endl;
                for (size_t j = 0; j < rules.size(); ++j) {
                    const Rule& toRule = rules[j];
                    std::cout << "to rule R" << j << ": " << toRule.toString() << std::endl;
                    if (fromRule.getHeadPredicate().getName() == bodyPredicate.getName() &&
                        toRule.getHeadPredicate().getName() == bodyPredicate.getName()) {
                        graph.addEdge(i, j);
                        std::cout << "dependency found: (R" << i << ",R" << j << ")" << std::endl;
                    }
                }
            }
        }
        return graph;
    }
};

#endif // INTERPRETER_H
