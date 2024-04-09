#include "src/dungeon/dungeon.cpp"
#include <iostream>

int main() {
    unsigned int width = 10;
    unsigned int height = 10;

    Graph graph(width, height);
    graph.GenerateBaseGraph();
    printGraph(width, height);

    return 0;
}
