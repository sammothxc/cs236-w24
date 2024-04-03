#ifndef NODE_H
#define NODE_H

#include <set>
#include <string>
#include <algorithm>

class Node {
private:
    std::set<int> adjacentNodeIDs;

public:
    void addEdge(int adjacentNodeID) {
        adjacentNodeIDs.insert(adjacentNodeID);
    }

    std::string toString() const {
        std::string result;
        for (auto it = adjacentNodeIDs.begin(); it != adjacentNodeIDs.end(); ++it) {
            if (it != adjacentNodeIDs.begin()) {
                result += ",";
            }
            result += "R" + std::to_string(*it);
        }
        return result;
    }
};

#endif // NODE_H
