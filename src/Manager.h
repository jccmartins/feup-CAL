/*
 * Manager.h
 */
#ifndef MANAGER_H_
#define MANAGER_H_

#include <vector>
#include <limits>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

template <class T>
class Graph;

#define MAX std::numeric_limits<T>::max()

/************************* Manager  **************************/

template <class T>
class Manager
{
    Graph<T> graph;
    std::vector<T> companies_vertices_ids;
    T garage_vertex_id;
    std::vector<T> bus_stops_vertices_ids;
    T bus_capacity = MAX;

public:
    Manager();

    Graph<T> getGraph() const;
    std::vector<T> getCompaniesVerticesIds() const;
    T getGarageVertexId() const;
    std::vector<T> getBusStopsVerticesIds() const;
    T getBusCapacity() const;

    void loadTagsFile();
};

/**
 * .cpp
*/

template <class T>
Manager<T>::Manager()
{
    this->graph.loadFile();
    loadTagsFile();
}

template <class T>
Graph<T> Manager<T>::getGraph() const
{
    return this->graph;
}

template <class T>
std::vector<T> Manager<T>::getCompaniesVerticesIds() const
{
    return this->companies_vertices_ids;
}

template <class T>
T Manager<T>::getGarageVertexId() const
{
    return this->garage_vertex_id;
}

template <class T>
std::vector<T> Manager<T>::getBusStopsVerticesIds() const
{
    return this->bus_stops_vertices_ids;
}

template <class T>
T Manager<T>::getBusCapacity() const
{
    return this->bus_capacity;
}

/**
 * Load companies, garage and bus stops vertices
*/
template <class T>
void Manager<T>::loadTagsFile()
{
    std::ifstream tags("../resources/Mapas-20200424/GridGraphs/16x16/tags.txt");

    std::string line, tag_id;
    std::istringstream iss;
    unsigned int node_id, n_nodes, n_tags;

    std::string tag1("building=company");
    std::string tag2("bus=garage");
    std::string tag3("bus=stop");

    // read num of nodes
    std::getline(tags, line);
    iss.str(line);
    iss >> n_tags;

    // load tags
    std::cout << "ntags " << n_tags << std::endl;
    for (int i = 0; i < n_tags; i++)
    {
        std::getline(tags, line);
        tag_id = line;
        std::cout << "tagid " << tag_id << std::endl;

        std::getline(tags, line);
        sscanf(line.c_str(), "%d", &n_nodes);
        std::cout << "nnodes " << n_nodes << std::endl;
        for (int j = 0; j < n_nodes; j++)
        {
            std::getline(tags, line);
            sscanf(line.c_str(), "%d", &node_id);
            
            std::cout << "node_id " << node_id << std::endl;

            if (tag_id.substr(0,tag1.length()) == tag1)
            {
                std::cout << "added\n";
                this->companies_vertices_ids.push_back(node_id);
            }
            else if (tag_id.substr(0,tag2.length()) == tag2)
            {
                std::cout << "added\n";

                this->garage_vertex_id = node_id;
            }
            else if (tag_id.substr(0,tag3.length()) == tag3)
            {
                std::cout << "added1\n";

                this->bus_stops_vertices_ids.push_back(node_id);
            }
        }
    }
}

#endif /* MANAGER_H_ */
