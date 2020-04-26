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

#include "Company.h"

template <class T>
class Graph;

template <class T>
struct Bus{
    T capacity;
};

#define MAX std::numeric_limits<T>::max()

/************************* Manager  **************************/

template <class T>
class Manager
{
    Graph<T> graph;
    T garage_vertex_id;
    std::vector<Bus<T>> buses;
    std::vector<Company<T>> companies;

public:
    Manager();

    Graph<T> getGraph() const;
    T getGarageVertexId() const;
    std::vector<Bus<T>> getBuses() const;
    std::vector<Company<T>> getCompanies() const;

    void loadTagsFile();

    void simulatedAnnealing();
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
std::vector<Bus<T>> Manager<T>::getBuses() const
{
    return this->buses;
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
                iss.str(line);
                Stop<T> *stop = new Stop<T>();
                iss >> stop->vertex_id;
                iss >> stop->number_of_workers;
                std::cout << "stop vertex " << stop->vertex_id << " n workers " << stop->number_of_workers << std::endl;
                this->companies[this->companies.size() - 1].bus_stops->push_back(stop);
            }
        }
    }
    std::cout << "end\n";
}

template <class T>
void Manager<T>::simulatedAnnealing()
{
    unsigned int num_iterations = 1000000;


}

#endif /* MANAGER_H_ */
