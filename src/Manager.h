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

template <class T>
class Company;

#define MAX std::numeric_limits<T>::max()

/************************* Manager  **************************/

template <class T>
class Manager
{
    Graph<T> graph;
    T garage_vertex_id;
    T bus_capacity = MAX;
    T number_of_buses = 1;
    std::vector<Company<T>> companies;

public:
    Manager();

    Graph<T> getGraph() const;
    T getGarageVertexId() const;
    T getBusCapacity() const;
    T getNumberOfBuses() const;
    std::vector<Company<T>> getCompanies() const;

    void loadTagsFile();

    // std::vector<T> simulatedAnnealing();
};

/**
 * .cpp
*/

template <class T>
Manager<T>::Manager()
{
    this->graph.loadFile();
    std::cout << "loaded nodes\n";
    loadTagsFile();
}

template <class T>
Graph<T> Manager<T>::getGraph() const
{
    return this->graph;
}

template <class T>
T Manager<T>::getGarageVertexId() const
{
    return this->garage_vertex_id;
}

template <class T>
T Manager<T>::getBusCapacity() const
{
    return this->bus_capacity;
}

template <class T>
T Manager<T>::getNumberOfBuses() const
{
    return this->number_of_buses;
}

template <class T>
std::vector<Company<T>> Manager<T>::getCompanies() const
{
    return this->companies;
}

/**
 * Load companies, garage and bus stops vertices
*/
template <class T>
void Manager<T>::loadTagsFile()
{
    std::string filename("resources/Mapas-20200424/GridGraphs/16x16/tags.txt");
    std::ifstream tags(filename);
    if (!tags.good())
    {
        std::cout << "Unable to access file " << filename << std::endl;
        return;
    }

    std::string line, tag_id;
    std::istringstream iss;
    unsigned int node_id, n_nodes, n_tags;

    std::string tag1("bus=garage");
    std::string tag2("company=");
    std::string tag3("bus=stop");

    // read number of tags
    std::getline(tags, line);
    iss.str(line);
    iss >> n_tags;

    // load tags
    std::cout << "ntags " << n_tags << std::endl;
    for (unsigned int i = 0; i < n_tags; i++)
    {
        std::getline(tags, line);
        iss.str(line);
        iss >> tag_id;
        std::cout << "tagid " << tag_id << std::endl;

        std::getline(tags, line);
        sscanf(line.c_str(), "%d", &n_nodes);
        std::cout << "nnodes " << n_nodes << std::endl;
        for (unsigned int j = 0; j < n_nodes; j++)
        {
            std::getline(tags, line);
            sscanf(line.c_str(), "%d", &node_id);

            std::cout << "node_id " << node_id << std::endl;

            if (tag_id.substr(0, tag1.length()) == tag1)
            {
                this->garage_vertex_id = node_id;
            }
            else if (tag_id.substr(0, tag2.length()) == tag2)
            {
                std::string name = tag_id.substr(tag2.length(), tag_id.length());
                std::cout << "name " << name << std::endl;

                this->companies.push_back(Company<T>(name, node_id));
            }
            else if (tag_id.substr(0, tag3.length()) == tag3)
            {
                std::cout << "busstop\n";
                this->companies[this->companies.size() - 1].bus_stops_vertices_ids.push_back(node_id);
            }
        }
    }
}

#endif /* MANAGER_H_ */
