#include <fstream>
#include <string>
#include <sstream>
#include <stdio.h>
#include "graphviewer.h"

void drawGraphFromFile(std::string name, unsigned int port);

int main() {
    drawGraphFromFile("Porto", 7772);
    getchar();

    return 0;
}

void drawGraphFromFile(std::string city_name, unsigned int port){
    string city_name_lowercase = city_name;
    city_name_lowercase[0] = tolower(city_name_lowercase[0]);
    std::ifstream nodes("../resources/Mapas-20200424/PortugalMaps/PortugalMaps/"+city_name+"/nodes_x_y_"+city_name_lowercase+".txt");
    std::ifstream edges("../resources/Mapas-20200424/PortugalMaps/PortugalMaps/"+city_name+"/edges_"+city_name_lowercase+".txt");
//    std::ifstream window("../resources/graphs/"+name+"/window.txt");
    std::string line, background_path;
    std::istringstream iss;
    unsigned int node_id, n_nodes, n_edges, height, width, node_id_origin, node_id_destination, type, scale, dynamic, thickness, size, dashed, curved;
    float x, y;
    char label[256], icon_path[256], flow[256], weight[256];
    string node_color, edge_color;

//    window >> width >> height >> dynamic >> scale >> dashed >> curved >> background_path;
    dynamic = false;
    dashed = false;
    curved = false;
    width = 1000;
    height = 500;
    scale = 500;
    size = 10;
    node_color = GREEN;
    edge_color = BLUE;

    GraphViewer *gv = new GraphViewer(width, height, dynamic, port);
//    gv->setBackground(background_path);
    gv->createWindow(width, height);
    gv->defineEdgeDashed(dashed);
    gv->defineEdgeCurved(curved);
    gv->defineVertexColor(node_color);
    gv->defineEdgeColor(edge_color);
    gv->defineVertexSize(size);

    // read num of nodes
    std::getline(nodes, line);
    iss.str(line);
    iss >> n_nodes;

    // draw nodes
    cout << "nr nodes: " << n_nodes << endl;
    float relative_x = 0, relative_y = 0;
    for(int i = 0; i < n_nodes;i++) {
        std::getline(nodes, line);
        sscanf( line.c_str(), "(%d, %f, %f)", &node_id, &x, &y);
        printf("%d %f %f\n", node_id,x,y);
        if(i == 0){
            relative_x = x;
            relative_y = y;
        }
        gv->addNode(node_id , x-relative_x, y-relative_y);
    }

    // read num of edges
    std::getline(edges, line);
    sscanf( line.c_str(), "%d", &n_edges);

    //draw edges
    cout << "nr edges: " << n_edges << endl;
    for(int i = 0; i < n_edges ; i++) {
        std::getline(edges, line);
        sscanf( line.c_str(), "(%d, %d)", &node_id_origin, &node_id_destination);
        cout << node_id_origin << " " << node_id_destination << "\n";
        gv->addEdge(i, node_id_origin, node_id_destination, EdgeType::DIRECTED);
    }

    cout << "rearrange\n";
    gv->rearrange();
}


