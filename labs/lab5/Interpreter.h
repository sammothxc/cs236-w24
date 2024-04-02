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

        // Loop over rules
        for (size_t i = 0; i < rules.size(); ++i) {
            const Rule& fromRule = rules[i];
            std::cout << "from rule R" << i << ": " << fromRule.toString() << std::endl;

            // Loop over predicates in the body
            for (const Predicate& bodyPredicate : fromRule.getBodyPredicates()) {
                std::cout << "from body predicate: " << bodyPredicate.toString() << std::endl;
            }

            // Loop over rules again to find dependencies
            for (size_t j = 0; j < rules.size(); ++j) {
                const Rule& toRule = rules[j];

                // Avoid self-dependencies
                if (i != j) {
                    // Find matching names and add edge
                    if (fromRule.getHeadPredicate().getName() == toRule.getHeadPredicate().getName()) {
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
