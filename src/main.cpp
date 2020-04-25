#include <fstream>
#include <string>
#include <sstream>
#include <stdio.h>
#include "graphviewer.h"
#include "Graph.h"

int main()
{
	Graph<unsigned int> graph;
	graph.loadFile();

	// graph.dijkstraShortestPath(474541486);
	// vector<unsigned int> path = graph.getPathTo(417998888);

	// initialize GraphViewer
	GraphViewer *gv;
	unsigned int height, width, dynamic, size, dashed, curved;
	string node_color, edge_color;

	dynamic = false;
	dashed = false;
	curved = false;
	width = 1366;
	height = 768;
	size = 10;
	node_color = GREEN;
	edge_color = BLUE;

	gv = new GraphViewer(width, height, dynamic);
	//    gv->setBackground(background_path);
	gv->createWindow(width, height);
	gv->defineEdgeDashed(dashed);
	gv->defineEdgeCurved(curved);
	gv->defineVertexColor(node_color);
	gv->defineEdgeColor(edge_color);
	gv->defineVertexSize(size);

	// for (int i = 0; i < path.size(); i++)
	// {
	// 	cout << path[i] << endl;
	// 	gv->setVertexColor(path[i], RED);
	// }

	graph.drawGraph(gv);

	getchar();

	return 0;
}
