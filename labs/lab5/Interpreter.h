#include "Graph.h"
#include "Rule.h"
#include <vector>
#include <unordered_map>

class Interpreter {
public:
    static Graph makeGraph(const std::vector<Rule>& rules) {
        Graph graph(rules.size());
        std::unordered_map<std::string, int> ruleIndices;
        int nodeIdCounter = 0;

        // Associate rule names with unique node IDs
        for (const Rule& rule : rules) {
            ruleIndices[rule.getHeadPredicate().getName()] = nodeIdCounter++;
        }

        // Create edges based on rule dependencies
        for (size_t i = 0; i < rules.size(); ++i) {
            const Rule& fromRule = rules[i];
            int fromNodeId = ruleIndices[fromRule.getHeadPredicate().getName()];
            for (const Predicate& bodyPredicate : fromRule.getBodyPredicates()) {
                const std::string& bodyName = bodyPredicate.getName();
                auto it = ruleIndices.find(bodyName);
                if (it != ruleIndices.end()) {
                    int toNodeId = it->second;
                    graph.addEdge(fromNodeId, toNodeId);
                }
            }
        }

        return graph;
    }
};
