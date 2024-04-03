#ifndef GRAPH_H
#define GRAPH_H

#include <map>
#include <string>
#include "Node.h"

class Graph {
private:
    std::map<int, Node> nodes;

public:
    Graph(int size) {
        for (int nodeID = 0; nodeID < size; nodeID++) {
            nodes[nodeID] = Node();
        }
    }

    void addEdge(int fromNodeID, int toNodeID) {
        nodes[fromNodeID].addEdge(toNodeID);
    }

    std::string toString() const {
        std::string result;
        for (const auto& pair : nodes) {
            int nodeID = pair.first;
            const Node& node = pair.second;
            result += "R" + std::to_string(nodeID) + ":" + node.toString() + "\n";
        }
        return result;
    }
};

#endif // GRAPH_H
