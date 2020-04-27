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
#include <algorithm>

#include "Company.h"

template <class T>
class Graph;

template <class T>
struct Bus
{
    T id;
    T capacity;
    std::vector<int> *path;
};

#define MAX std::numeric_limits<T>::max()

/************************* Manager  **************************/

template <class T>
class Manager
{
    Graph<T> graph;
    T garage_vertex_id;
    std::vector<Bus<T> *> buses;
    std::vector<Company<T>> companies;

public:
    Manager();

    Graph<T> getGraph() const;
    T getGarageVertexId() const;
    std::vector<Bus<T> *> getBuses() const;
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
    // initialize graph
    this->graph.loadFile();
    // initialize garage vertex id and companies info
    loadTagsFile();
    // initializes buses
    std::vector<int> capacities = {20, 30, 10, 2, 5, 1, 40};
    for (unsigned int i = 0; i < capacities.size(); i++)
    {
        Bus<T> *bus = new Bus<T>();
        bus->id = i;
        bus->capacity = capacities[i];
        buses.push_back(bus);
    }
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
std::vector<Bus<T> *> Manager<T>::getBuses() const
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

/************************* ALGORITHMS  **************************/
template <class T>
std::vector<Bus<T> *> getBusesForCompany(std::vector<Stop<T> *> bus_stops, std::vector<Bus<T> *> buses)
{
    std::cout << "getbusesforcompany begin\n";
    std::vector<Bus<T> *> buses_for_company;

    // remove unavailable buses from vector
    for (unsigned int i = 0; i < buses.size(); i++)
    {
        if (buses[i]->path != NULL)
        {
            buses.erase(buses.begin() + i);
        }
    }

    for (auto bus : buses)
    {
        std::cout << "available buses\n";
        std::cout << "bus id " << bus->id << " capacity " << bus->capacity << std::endl;
    }

    // get total number of workers
    int number_of_workers = 0;
    for (auto stop : bus_stops)
    {
        std::cout << "stop n workers " << stop->number_of_workers << std::endl;
        number_of_workers += stop->number_of_workers;
    }
    std::cout << "num workers " << number_of_workers << std::endl;

    // get total capacity of all available buses
    int total_capacity = 0;
    for (auto bus : buses)
    {
        total_capacity += bus->capacity;
    }
    std::cout << "available buses total capacity " << total_capacity << std::endl;
    // if available buses can provide the service
    if (number_of_workers <= total_capacity)
    {
        // get minimum number of buses
        while (number_of_workers > 0)
        {
            for (unsigned int i = 0; i < buses.size(); i++)
            {
                if (buses[i]->capacity >= number_of_workers || i == buses.size() - 1)
                {
                    number_of_workers -= buses[i]->capacity;
                    std::cout << "num workers " << number_of_workers << std::endl;
                    buses_for_company.push_back(buses[i]);
                    buses.erase(buses.begin() + i);
                    break;
                }
            }
        }
    }

    std::cout << "buses for company\n";
    for (auto bus : buses_for_company)
    {
        std::cout << "bus id " << bus->id << " capacity " << bus->capacity << std::endl;
    }

    std::cout << "getbusesforcompany end\n";

    return buses_for_company;
}

template <class T>
bool greater_capacity(const Bus<T> *bus1, const Bus<T> *bus2)
{
    return bus1->capacity < bus2->capacity;
}

template <class T>
void Manager<T>::simulatedAnnealing()
{
    // re-initialize buses
    for (auto bus : buses)
    {
        bus->path = NULL;
    }

    // sort buses in ascending order of capacity
    std::sort(buses.begin(), buses.end(), greater_capacity<T>);
    for (auto bus : buses)
    {
        std::cout << "bus id " << bus->id << " capacity " << bus->capacity << std::endl;
    }

    unsigned int num_iterations = 1000000;
    float temperature;
    std::vector<Stop<T> *> bus_stops;
    for (auto company : companies)
    {
        if (getBusesForCompany(*company.getBusStops(), this->buses).empty())
        {
            std::cout << "Not enough buses to provide transportation to " << company.getName() << " workers\n";
        }
        else
        {
            // bus_stops = *company.getBusStops();
            // temperature = 500;
            // for (unsigned int i = 0; i < num_iterations; i++)
            // {
            //     // distance();
            //     // temperature = temperature((float)(i + 1) / num_iterations);
            //     std::cout << temperature << std::endl;
            // }
        }

        std::cout << "PRESS ANY KEY\n";
        getchar();
    }
}

#endif /* MANAGER_H_ */
