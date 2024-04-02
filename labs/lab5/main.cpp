// main.cpp

#include <iostream>
#include "Node.h"

int main() {
    Node node;
    node.addEdge(4);
    node.addEdge(8);
    node.addEdge(2);
    std::cout << node.toString() << std::endl;

    return 0;
}
