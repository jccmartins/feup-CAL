/*
 * Company.h
 */
#ifndef COMPANY_H_
#define COMPANY_H_

#include <string>
#include <vector>

template <class T>
class Vertex;

template <class T>
class Manager;

template <class T>
struct Stop
{
    T vertex_id;
    T number_of_workers;
};

/************************* Company  **************************/

template <class T>
class Company
{
    std::string name;
    T company_vertex_id;
    std::vector<Stop<T>> bus_stops;

public:
    Company(std::string name, T company_vertex_id);

    std::string getName() const;
    T getCompanyVertexId() const;
    std::vector<Stop<T>> getBusStops() const;

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
std::vector<Stop<T>> Company<T>::getBusStops() const
{
    return this->bus_stops;
}

#endif /* COMPANY_H_ */
