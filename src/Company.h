/*
 * Company.h
 */
#ifndef COMPANY_H_
#define COMPANY_H_

#include <string>
#include <vector>

template <class T>
class Vertex;

/************************* Company  **************************/

template <class T>
class Company
{
    std::string name;
    T company_vertex_id;
    std::vector<T> bus_stops_vertices_ids;

public:
    Company(std::string name, T company_vertex_id);

    std::string getName() const;
    T getCompanyVertexId() const;
    std::vector<T> getBusStopsVerticesIds() const;

    friend class Manager<T>;
};

template <class T>
Company<T>::Company(std::string name, T company_vertex_id)
    : name(name), company_vertex_id(company_vertex_id) {}

template <class T>
std::string Company<T>::getName() const
{
    return this->name;
}

template <class T>
T Company<T>::getCompanyVertexId() const
{
    return this->company_vertex_id;
}

template <class T>
std::vector<T> Company<T>::getBusStopsVerticesIds() const
{
    return this->bus_stops_vertices_ids;
}

#endif /* COMPANY_H_ */
