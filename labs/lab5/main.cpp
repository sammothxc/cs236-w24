// main.cpp

#include <iostream>
#include "Graph.h"

int main() {
    Graph graph(3);
    graph.addEdge(1, 2);
    graph.addEdge(1, 0);
    graph.addEdge(0, 1);
    graph.addEdge(1, 1);
    std::cout << graph.toString();

    return 0;
}
