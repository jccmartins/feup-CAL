#include "Manager.h"
#include "Graph.h"
#include "lib/graphviewer.h"
#include "Company.h"

#include <algorithm>    // std::sort

int main()
{
	Manager<int> *manager = new Manager<int>();
	std::cout << "continei\n";

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
	node_color = LIGHT_GRAY;
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
	
	manager->simulatedAnnealing();
	cout << "A\n";

	// set company vertex color to black
	vector<Company<int>> companies = manager->getCompanies();
	for (unsigned int i = 0; i < companies.size(); i++)
	{
		std::cout << "Company: " << companies[i].getName() << " vertex id: " << companies[i].getCompanyVertexId() << endl;
		gv->setVertexColor(companies[i].getCompanyVertexId(), GREEN);
		std::string company_label = companies[i].getName() + " - " + std::to_string(companies[i].getCompanyVertexId());
		std::cout << "label " << company_label << endl;
		gv->setVertexLabel(companies[i].getCompanyVertexId(), company_label);

		// set bus stops vertices color to orange
		vector<Stop<int>> bus_stops = *companies[i].getBusStops();
		cout << "Bus stops: ";
		for (auto stop : bus_stops)
		{
			cout << stop.vertex_id << " " << stop.number_of_workers << std::endl;
			gv->setVertexColor(stop.vertex_id, ORANGE);
		}
	}

	// set garage vertex color to cyan
	unsigned int garage = manager->getGarageVertexId();
	std::cout << "Garage: " << garage << endl;
	gv->setVertexColor(garage, CYAN);
	std::string garage_label = "GARAGE - " + std::to_string(garage);
	gv->setVertexLabel(garage, garage_label);

	getchar();

	return 0;
}
