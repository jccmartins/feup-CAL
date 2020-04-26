#include "Manager.h"
#include "Graph.h"

int main()
{
	Manager<unsigned int> *manager = new Manager<unsigned int>();

	// vector<unsigned int> companies = manager->getCompaniesVerticesIds();
	// cout << "Companies:\n";
	// for (auto company : companies)
	// {
	// 	cout << company << " ";
	// }
	// cout << endl;

	// unsigned int garage = manager->getGarageVertexId();
	// cout << "Garage:\n";
	// cout << garage << endl;

	// vector<unsigned int> bus_stops = manager->getBusStopsVerticesIds();
	// cout << "Bus stops:\n";
	// for (auto stop : bus_stops)
	// {
	// 	cout << stop << " ";
	// }

	// initialize GraphViewer
	GraphViewer *gv;
	unsigned int height, width, dynamic, size, dashed, curved;
	string node_color, edge_color;

	dynamic = false;
	dashed = false;
	curved = false;
	width = 1366;
	height = 768;
	size = 1;
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

	// graph.dijkstraShortestPath(90379359);
	// unsigned int i = 0;
	// string color;
	// while (i < graph.getVertexSet().size())
	// {
	// 	if (graph.getVertexSet()[i]->getPath() != NULL)
	// 	{
	// 		vector<unsigned int> path = graph.getPathTo(graph.getVertexSet()[i]->getInfo());
	// 		if (path.size() > 100)
	// 		{
	// 			for (int i = 0; i < path.size(); i++)
	// 			{
	// 				if (i == 0)
	// 				{
	// 					color = WHITE;
	// 				}
	// 				else if (i == path.size() - 1)
	// 				{
	// 					color = BLACK;
	// 				}
	// 				else
	// 				{
	// 					color = RED;
	// 				}
	// 				gv->setVertexColor(path[i], color);
	// 			}
	// 			break;
	// 		}
	// 	}
	// 	++i;
	// }

	manager->getGraph().drawGraph(gv);

	getchar();

	return 0;
}
