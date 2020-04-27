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
                Stop<T> stop;
                iss >> stop.vertex_id;
                iss >> stop.number_of_workers;
                std::cout << "stop vertex " << stop.vertex_id << " n workers " << stop.number_of_workers << std::endl;
                this->companies[this->companies.size() - 1].bus_stops->push_back(stop);
            }
        }
    }
    std::cout << "end\n";
}

/************************* ALGORITHMS  **************************/
template <class T>
std::vector<Bus<T> *> getBusesForCompany(std::vector<Stop<T>> bus_stops, std::vector<Bus<T> *> buses)
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
        std::cout << "stop n workers " << stop.number_of_workers << std::endl;
        number_of_workers += stop.number_of_workers;
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

unsigned int probability(double delta_distance, double temperature)
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
    unsigned int index1, index2;
    do
    {
        index1 = rand() % (bus_stops.size());
        index2 = rand() % (bus_stops.size());
    } while (index1 == index2);
    std::cout << "index1 " << index1 << std::endl;
    std::cout << "index2 " << index2 << std::endl;
    std::swap(bus_stops[index1], bus_stops[index2]);
    return bus_stops;
}

template <class T>
void Manager<T>::simulatedAnnealing()
{
    srand(time(NULL));

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
    double temperature, temperature_decrease_rate, delta_distance;
    std::vector<Stop<T>> bus_stops;
    std::vector<Stop<T>> new_bus_stops;
    double r, prob;
    for (auto company : companies)
    {
        if (getBusesForCompany(*company.getBusStops(), this->buses).empty())
        {
            std::cout << "Not enough buses to provide transportation to " << company.getName() << " workers\n";
        }
        else
        {
            std::cout << "old bus stops\n";
            for (auto stop : *company.getBusStops())
            {
                std::cout << stop.vertex_id << " ";
            }
            std::cout << std::endl;
            bus_stops = randomNeighbour(*company.getBusStops());
            std::cout << "new bus stops\n";
            for (auto stop : bus_stops)
            {
                std::cout << stop.vertex_id << " ";
            }
            std::cout << std::endl;
            // temperature initial value
            temperature = 5000;
            temperature_decrease_rate = (double)temperature / num_iterations;
            std::cout << temperature_decrease_rate << std::endl;
            getchar();
            // for (unsigned int i = 0; i < 5; i++)
            // {
            //     new_bus_stops = randomNeighbour(bus_stops);
            //     delta_distance = (double)distance(new_bus_stops, &this->buses) - distance(bus_stops, &this->buses);

            //     r = ((double)rand() / (RAND_MAX));
            //     prob = probability(delta_distance, temperature);
            //     std::cout << "random " << r << std::endl;
            //     std::cout << "prob " << prob << std::endl;
            //     if (r < prob)
            //     {
            //         bus_stops = new_bus_stops;
            //     }
            //     std::cout << temperature << std::endl;
            //     temperature -= temperature_decrease_rate;
            //     getchar();
            // }
        }

        // atribuir o path às buses
        std::cout << "PRESS ANY KEY\n";
        getchar();
    }
}

#endif /* MANAGER_H_ */
