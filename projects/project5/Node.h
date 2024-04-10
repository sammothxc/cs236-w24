// ADDED IN PROJECT 5
#pragma once
#include <set>

class Node {
    public:
    std::set<int> adjacencies;
    int id;
    bool isVisited;
    bool isSelfDependent;
    Node() {
        isVisited = false;
    }
};