#include <fstream>
#include <string>
#include <sstream>
#include <stdio.h>
#include "graphviewer.h"
#include "Graph.h"

int main() {
    Graph<unsigned int> graph;
    graph.loadFile();
    graph.drawGraph();
    getchar();

    return 0;
}


