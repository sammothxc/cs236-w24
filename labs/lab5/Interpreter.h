#include "Graph.h"
#include "Rule.h"
#include <vector>
#include <iostream>
#include <unordered_map>

class Interpreter {
public:
    static Graph makeGraph(const std::vector<Rule>& rules) {
        Graph graph(rules.size());
        std::unordered_map<std::string, int> ruleIndices;

        // Associate rule names with indices
        for (size_t i = 0; i < rules.size(); ++i) {
            const std::string& ruleName = rules[i].getHeadPredicate().getName();
            ruleIndices[ruleName] = i;
        }

        // Create edges based on rule dependencies
        for (size_t i = 0; i < rules.size(); ++i) {
            const Rule& fromRule = rules[i];
            for (const Predicate& bodyPredicate : fromRule.getBodyPredicates()) {
                const std::string& bodyName = bodyPredicate.getName();
                auto it = ruleIndices.find(bodyName);
                if (it != ruleIndices.end()) {
                    int toNodeId = it->second;
                    graph.addEdge(i, toNodeId);
                }
            }
        }
        return graph;
    }
};
