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
#include <unordered_map>
#include <utility> // std::pair

#include "Graph.h"

int global_bus_id = 0;

/************************* Bus  **************************/
template <class T>
struct Bus
{
    T id;
    T capacity;
    std::vector<T> path;
};

/************************* Stop  **************************/
template <class T>
struct Stop
{
    T vertex_id;
    T number_of_workers;
};

/************************* Company  **************************/
template <class T>
struct Company
{
    std::string name;
    T company_vertex_id;
    std::vector<Stop<T>> bus_stops;
};

#define MAX std::numeric_limits<T>::max()

// A hash function used to hash a pair of any kind
struct pair_hash
{
    template <class T>
    std::size_t operator()(const std::pair<T, T> &p) const
    {
        auto h1 = std::hash<T>{}(p.first);
        auto h2 = std::hash<T>{}(p.second);

        return h1 ^ h2;
    }
};

/************************* Manager  **************************/

template <class T>
class Manager
{
    Graph<T> graph;
    T garage_vertex_id;
    std::vector<Bus<T>> buses;
    std::vector<Company<T>> companies;

public:
    Graph<T> &getGraph();
    T &getGarageVertexId();
    std::vector<Bus<T>> &getBuses();
    std::vector<Company<T>> &getCompanies();

    void loadTagsFile();

    std::vector<Bus<T> *> getBusesForCompany(Company<T> company, string direction);
    double distance(std::vector<Stop<T>> bus_stops, std::vector<Bus<T> *> buses,
                    T company_vertex_id,
                    std::unordered_map<std::pair<T, T>, double, pair_hash> *distances, string direction);
    std::unordered_map<std::pair<T, T>, double, pair_hash> getBusStopsDistances(
        T garage_vertex_id, std::vector<Stop<T>> bus_stops, T company_vertex_id, string direction);
    double simulatedAnnealing(Company<T> company, string direction);
    void clearBusesPaths();
    void sortBusesAscendingCapacity();
};

/**
 * .cpp
*/
template <class T>
Graph<T> &Manager<T>::getGraph()
{
    return this->graph;
}

template <class T>
T &Manager<T>::getGarageVertexId()
{
    return this->garage_vertex_id;
}

template <class T>
std::vector<Bus<T>> &Manager<T>::getBuses()
{
    return this->buses;
}

template <class T>
std::vector<Company<T>> &Manager<T>::getCompanies()
{
    return this->companies;
}

/**
 * Load companies, garage and bus stops vertices for 16x16 grid testing example
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
    for (unsigned int i = 0; i < n_tags; i++)
    {
        std::getline(tags, line);
        iss.str(line);
        iss >> tag_id;

        std::getline(tags, line);
        sscanf(line.c_str(), "%d", &n_nodes);
        for (unsigned int j = 0; j < n_nodes; j++)
        {
            std::getline(tags, line);
            sscanf(line.c_str(), "%d", &node_id);

            if (tag_id.substr(0, tag1.length()) == tag1)
            {
                this->garage_vertex_id = node_id;
            }
            else if (tag_id.substr(0, tag2.length()) == tag2)
            {
                std::string name = tag_id.substr(tag2.length(), tag_id.length());

                Company<T> company;
                company.name = name;
                company.company_vertex_id = node_id;
                this->companies.push_back(company);
            }
            else if (tag_id.substr(0, tag3.length()) == tag3)
            {
                iss.str(line);
                Stop<T> stop;
                iss >> stop.vertex_id;
                iss >> stop.number_of_workers;
                this->companies[this->companies.size() - 1].bus_stops.push_back(stop);
            }
        }
    }
}

/************************* ALGORITHMS  **************************/
template <class T>
std::vector<Bus<T> *> Manager<T>::getBusesForCompany(Company<T> company, string direction)
{
    std::vector<Bus<T> *> buses_for_company;

    if (direction == "company")
    {
        std::vector<Bus<T> *> aux_buses;
        for (auto &bus : this->buses)
        {
            aux_buses.push_back(&bus);
        }

        std::vector<Stop<T>> bus_stops = company.bus_stops;

        // remove unavailable buses from vector
        for (unsigned int i = 0; i < aux_buses.size(); i++)
        {
            if (!aux_buses[i]->path.empty())
            {
                aux_buses.erase(aux_buses.begin() + i);
            }
        }

        // get total number of workers
        int number_of_workers = 0;
        for (auto stop : bus_stops)
        {
            number_of_workers += stop.number_of_workers;
        }

        // get total capacity of all available buses
        int total_capacity = 0;
        for (auto bus : aux_buses)
        {
            total_capacity += bus->capacity;
        }

        // if available buses can provide the service
        if (number_of_workers <= total_capacity)
        {
            // get minimum number of buses
            while (number_of_workers > 0)
            {
                for (unsigned int i = 0; i < aux_buses.size(); i++)
                {
                    if (aux_buses[i]->capacity >= number_of_workers || i == aux_buses.size() - 1)
                    {
                        number_of_workers -= aux_buses[i]->capacity;
                        buses_for_company.push_back(aux_buses[i]);
                        aux_buses.erase(aux_buses.begin() + i);
                        break;
                    }
                }
            }
        }
    }
    else if (direction == "garage")
    {
        for (auto &bus : this->buses)
        {
            if (!bus.path.empty())
            {
                if (bus.path[0] == company.company_vertex_id ||
                    bus.path[bus.path.size() - 1] == company.company_vertex_id)
                {
                    buses_for_company.push_back(&bus);
                }
            }
        }
    }

    return buses_for_company;
}

template <class T>
bool greater_capacity(const Bus<T> bus1, const Bus<T> bus2)
{
    return bus1.capacity < bus2.capacity;
}

double probability(double delta_distance, double temperature)
{
    /**
     * probability function to decide wether to change the solution or not
     * as delta distance decreases the probability is higher which means better solutions are favoured
     * as temperature decreases the probability to change to a better solution is higher
     * and the probability to change to a worse solution is lower 
     * which means that as the temperature reaches 0 
     * the algorithm will behave similar to hill climbing (only changing to better solutions)
    */
    return (1 / (1 + exp(delta_distance / temperature)));
}

template <class T>
std::vector<Stop<T>> randomNeighbour(std::vector<Stop<T>> bus_stops)
{
    if (bus_stops.size() > 1)
    {
        unsigned int index1, index2;
        do
        {
            index1 = rand() % (bus_stops.size());
            index2 = rand() % (bus_stops.size());
        } while (index1 == index2);
        std::swap(bus_stops[index1], bus_stops[index2]);
    }

    return bus_stops;
}

template <class T>
std::vector<vector<T>> getBusesPaths(std::vector<Stop<T>> bus_stops, std::vector<Bus<T> *> buses)
{
    vector<vector<T>> buses_paths;

    // fill with empty vectors equal to the number of buses
    for (unsigned int i = 0; i < buses.size(); i++)
    {
        buses_paths.push_back(vector<T>());
    }

    T bus_capacity;
    for (unsigned int i = 0; i < buses.size(); i++)
    {
        bus_capacity = buses[i]->capacity;
        while (bus_capacity > 0)
        {
            bus_capacity -= bus_stops[0].number_of_workers;
            buses_paths[i].push_back(bus_stops[0].vertex_id);
            if (bus_capacity < 0)
            {
                bus_stops[0].number_of_workers = abs(bus_capacity);
            }
            else
            {
                bus_stops.erase(bus_stops.begin());

                if (bus_stops.empty())
                {
                    break;
                }
            }
        }
    }

    return buses_paths;
}

template <class T>
double Manager<T>::distance(std::vector<Stop<T>> bus_stops, std::vector<Bus<T> *> buses, T company_vertex_id,
                            std::unordered_map<std::pair<T, T>, double, pair_hash> *distances, string direction)
{
    double total_distance = 0;

    vector<vector<T>> buses_paths = getBusesPaths(bus_stops, buses);
    for (auto &path : buses_paths)
    {
        if (direction == "company")
        {
            path.insert(path.begin(), this->garage_vertex_id);
            path.push_back(company_vertex_id);
        }
        else if (direction == "garage")
        {
            path.insert(path.begin(), company_vertex_id);
            path.push_back(this->garage_vertex_id);
        }
        for (unsigned int i = 0; i < path.size() - 1; i++)
        {
            // if distances map doesn't contain a certain value because of graph connectivity
            if (distances->find({path[i], path[i + 1]}) == distances->end())
            {
                return -1;
            }

            total_distance += distances->at({path[i], path[i + 1]});
        }
    }

    return total_distance;
}

/**
 * Calculates and stores distances between bus stops and garage and company vertices
 * and stores them in an unordered map
 */
template <class T>
std::unordered_map<std::pair<T, T>, double, pair_hash> Manager<T>::getBusStopsDistances(
    T garage_vertex_id, std::vector<Stop<T>> bus_stops, T company_vertex_id, string direction)
{
    std::unordered_map<std::pair<T, T>, double, pair_hash> distances;

    double distance;
    Vertex<T> *vertex;

    for (unsigned int i = 0; i < bus_stops.size(); i++)
    {
        if (direction == "company")
        {
            // calculate and store distances between garage and all bus stops
            graph.dijkstraShortestPath(garage_vertex_id);
            vertex = graph.findVertex(bus_stops[i].vertex_id);
            distance = vertex->getDist();

            if (distance != INF)
            {
                distances[{garage_vertex_id, bus_stops[i].vertex_id}] = distance;
            }

            // calculate and store distances between all bus stops and company
            graph.dijkstraShortestPath(bus_stops[i].vertex_id);
            vertex = graph.findVertex(company_vertex_id);
            distance = vertex->getDist();

            if (distance != INF)
            {
                distances[{bus_stops[i].vertex_id, company_vertex_id}] = distance;
            }
        }
        else if (direction == "garage")
        {
            // calculate and store distances between garage and all bus stops
            graph.dijkstraShortestPath(bus_stops[i].vertex_id);
            vertex = graph.findVertex(garage_vertex_id);
            distance = vertex->getDist();

            if (distance != INF)
            {
                distances[{bus_stops[i].vertex_id, garage_vertex_id}] = distance;
            }

            // calculate and store distances between all bus stops and company
            graph.dijkstraShortestPath(company_vertex_id);
            vertex = graph.findVertex(bus_stops[i].vertex_id);
            distance = vertex->getDist();

            if (distance != INF)
            {
                distances[{company_vertex_id, bus_stops[i].vertex_id}] = distance;
            }
        }

        unsigned int aux_index = i + 1;
        while (aux_index < bus_stops.size())
        {
            // calculate and store distances between bus stops
            graph.dijkstraShortestPath(bus_stops[i].vertex_id);
            vertex = graph.findVertex(bus_stops[aux_index].vertex_id);
            distance = vertex->getDist();

            if (distance != INF)
            {
                distances[{bus_stops[i].vertex_id, bus_stops[aux_index].vertex_id}] = distance;
            }

            graph.dijkstraShortestPath(bus_stops[aux_index].vertex_id);
            vertex = graph.findVertex(bus_stops[i].vertex_id);
            distance = vertex->getDist();

            if (distance != INF)
            {
                distances[{bus_stops[aux_index].vertex_id, bus_stops[i].vertex_id}] = distance;
            }

            ++aux_index;
        }
    }

    return distances;
}

template <class T>
double Manager<T>::simulatedAnnealing(Company<T> company, string direction)
{
    unsigned int num_iterations = 100000;
    double temperature_decrease_rate = 0.01;
    double temperature, delta_distance;
    std::vector<Stop<T>> bus_stops;
    std::vector<Stop<T>> new_bus_stops;
    double r, prob;
    double current_distance = 0, new_distance = 0;
    std::unordered_map<std::pair<T, T>, double, pair_hash> distances;

    std::vector<Bus<T> *> buses_for_company = getBusesForCompany(company, direction);
    if (buses_for_company.empty())
    {
        std::cout << "Not enough buses to provide transportation to " << company.name << " workers\n";
    }
    else
    {
        bus_stops = company.bus_stops;
        distances = getBusStopsDistances(this->garage_vertex_id, bus_stops, company.company_vertex_id, direction);
        current_distance = distance(bus_stops, buses_for_company, company.company_vertex_id, &distances, direction);
        // temperature initial value
        temperature = num_iterations * temperature_decrease_rate;
        for (unsigned int i = 0; i < num_iterations; i++)
        {
            new_bus_stops = randomNeighbour(bus_stops);
            new_distance = distance(new_bus_stops, buses_for_company, company.company_vertex_id, &distances, direction);
            if (new_distance != -1)
            {
                delta_distance = new_distance - current_distance;
                r = ((double)rand() / (RAND_MAX));
                prob = probability(delta_distance, temperature);
                if (r <= prob)
                {
                    bus_stops = new_bus_stops;
                    current_distance = new_distance;
                }
            }
            temperature -= temperature_decrease_rate;
        }
    }

    // attribute paths to buses
    std::vector<vector<T>> buses_paths = getBusesPaths(bus_stops, buses_for_company);
    for (unsigned int i = 0; i < buses_for_company.size(); i++)
    {
        if (direction == "company")
        {
            buses_paths[i].insert(buses_paths[i].begin(), this->garage_vertex_id);
            buses_paths[i].push_back(company.company_vertex_id);
        }
        else if (direction == "garage")
        {
            buses_paths[i].insert(buses_paths[i].begin(), company.company_vertex_id);
            buses_paths[i].push_back(this->garage_vertex_id);
        }

        buses_for_company[i]->path = buses_paths[i];
    }

    return current_distance;
}

template <class T>
void Manager<T>::clearBusesPaths()
{
    // re-initialize buses
    for (Bus<T> &bus : buses)
    {
        bus.path.clear();
    }
}

template <class T>
void Manager<T>::sortBusesAscendingCapacity()
{
    // sort buses in ascending order of capacity
    std::sort(buses.begin(), buses.end(), greater_capacity<T>);
}

#endif /* MANAGER_H_ */
