// main.cpp
#include "Graph.h"
#include <iostream>

int main() {
    Node node;
    node.addEdge(4);
    node.addEdge(8);
    node.addEdge(2);
    std::cout << node.toString() << std::endl;

    Graph graph(3);
    graph.addEdge(1, 2);
    graph.addEdge(1, 0);
    graph.addEdge(0, 1);
    graph.addEdge(1, 1);
    std::cout << graph.toString();

    return 0;
}
