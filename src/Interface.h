/*
 * Interface.h
 */
#ifndef INTERFACE_H_
#define INTERFACE_H_

#include "Manager.h"
#include "lib/graphviewer.h"

#define WIDTH 1000
#define HEIGHT 500

#define VERTEX_COLOR LIGHT_GRAY
#define EDGE_COLOR BLUE

#define GARAGE_COLOR CYAN
#define COMPANY_COLOR GREEN
#define BUS_STOP_COLOR ORANGE

/************************* Interface  **************************/

template <class T>
class Interface
{
    Manager<T> *manager;
    GraphViewer *gv;

public:
    Interface(Manager<T> *manager);

    void openGraphViewerWindow() const;
    void setGarageColorAndLabel() const;
    void setLocationsColorsAndLabels(Company<T> company) const;
    void chooseMap();
    void initializeGraphViewer();
    void pickGarageVertexId();
    void setFirstBus();
    void menu();
    void companiesMenu();
    void manageCompanyMenu(Company<T> &company);
    void addCompanyMenu();
    void removeCompanyMenu();
    void printBusesRoutes(Company<T> company) const;
    void resetVerticesColorAndLabel() const;
    void manageBuses();
};

/**
 * .cpp
*/

template <class T>
Interface<T>::Interface(Manager<T> *manager) : manager(manager)
{
}

template <class T>
void Interface<T>::openGraphViewerWindow() const
{
    unsigned int size, dashed, curved;
    dashed = false;
    curved = false;
    size = 1;
    gv->createWindow(WIDTH, HEIGHT);
    gv->defineEdgeDashed(dashed);
    gv->defineEdgeCurved(curved);
    gv->defineVertexColor(VERTEX_COLOR);
    gv->defineEdgeColor(EDGE_COLOR);
    gv->defineVertexSize(size);
}

template <class T>
void Interface<T>::setGarageColorAndLabel() const
{
    // set garage vertex color
    unsigned int garage = manager->getGarageVertexId();
    std::cout << "Garage: " << garage << endl;
    gv->setVertexColor(garage, GARAGE_COLOR);
    std::string garage_label = "GARAGE - " + std::to_string(garage);
    gv->setVertexLabel(garage, garage_label);

    gv->rearrange();
}

/**
 * Set colors for companies, garage and bus stops vertices
 */
template <class T>
void Interface<T>::setLocationsColorsAndLabels(Company<T> company) const
{
    // set company vertex color
    std::cout << "Company: " << company.name << " vertex id: " << company.company_vertex_id << endl;
    gv->setVertexColor(company.company_vertex_id, COMPANY_COLOR);
    std::string company_label = company.name + " - " + std::to_string(company.company_vertex_id);
    std::cout << "label " << company_label << endl;
    gv->setVertexLabel(company.company_vertex_id, company_label);

    // set bus stops vertices color
    vector<Stop<int>> bus_stops = company.bus_stops;
    cout << "Bus stops: \n";
    for (auto stop : bus_stops)
    {
        cout << stop.vertex_id << " " << stop.number_of_workers << std::endl;
        gv->setVertexColor(stop.vertex_id, BUS_STOP_COLOR);
    }

    gv->rearrange();
}

template <class T>
void Interface<T>::chooseMap()
{
    std::cout << "===============================\n";
    std::cout << "BosHBus: Workers Transportation\n";
    std::cout << "===============================\n";
    std::cout << "Choose a map:\n";
    std::cout << "1 - Porto\n";
    std::cout << "2 - 16x16 Grid for testing purposes\n";
    std::cout << "Any other key - Exit\n\n";
    std::cout << "Option: ";

    int option;
    std::cin >> option;
    if (cin.fail())
    {
        option = -1;
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    string city_name;

    switch (option)
    {
    case 1:
        city_name = "Porto";
        break;
    case 2:
        city_name = "testing";
    default:
        break;
    }

    if (!city_name.empty())
    {
        manager->getGraph().loadNodesAndEdges(city_name);
        if (city_name == "testing")
        {
            manager->loadTagsFile();
        }

        // pick garage vertex id
        pickGarageVertexId();

        setFirstBus();

        menu();
    }
}

template <class T>
void Interface<T>::pickGarageVertexId()
{
    bool done = false;
    while (!done)
    {
        std::cout << "Garage Vertex Id (0 - " << manager->getGraph().getNumVertex() - 1 << "): ";

        int index;
        std::cin >> index;
        if (!cin.fail() && index >= 0 && index < manager->getGraph().getNumVertex())
        {
            manager->getGarageVertexId() = index;
            done = true;
        }
        else
        {
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }

            std::cout << "You must pick a valid garage vertex id\n";
        }
    }
}

template <class T>
void Interface<T>::setFirstBus()
{
    bool done = false;
    while (!done)
    {
        std::cout << "First Bus Capacity (greater than 0): ";

        int capacity;
        std::cin >> capacity;
        if (!cin.fail() && capacity > 0)
        {
            Bus<T> bus;
            bus.capacity = capacity;
            manager->getBuses().push_back(bus);
            done = true;
        }
        else
        {
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }

            std::cout << "You must pick a valid bus capacity\n";
        }
    }
}

template <class T>
void Interface<T>::initializeGraphViewer()
{
    // initialize graphviewer
    unsigned int dynamic;
    dynamic = false;

    gv = new GraphViewer(WIDTH, HEIGHT, dynamic);
}

/**
 * Menu interface
*/
template <class T>
void Interface<T>::menu()
{
    bool done = false;
    while (!done)
    {
        std::cout << "===============================\n";
        std::cout << "BosHBus: Workers Transportation\n";
        std::cout << "===============================\n";
        std::cout << "OPTIONS:\n";
        std::cout << "1 - Open Map\n";
        std::cout << "2 - View routes\n";
        std::cout << "3 - Manage Companies\n";
        std::cout << "4 - Manage Buses\n";
        std::cout << "Any other key - Exit\n\n";
        std::cout << "Option: ";

        int option;
        std::cin >> option;
        if (cin.fail())
        {
            option = -1;
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        switch (option)
        {
        case 1:
        {
            if (gv == NULL)
            {
                initializeGraphViewer();
            }
            openGraphViewerWindow();
            manager->getGraph().drawGraph(gv);
            setGarageColorAndLabel();
        }
        break;
        case 2:
        {
            manager->sortBusesAscendingCapacity();
            manager->clearBusesPaths();

            double distance;
            for (auto &company : manager->getCompanies())
            {
                if (company.bus_stops.size() > 0)
                {
                    setLocationsColorsAndLabels(company);

                    std::cout << "To work:\n";
                    distance = manager->simulatedAnnealing(company, "company");
                    if (distance == -1)
                    {
                        std::cout << "This trip is not possible due to graph connectivity\n";
                    }
                    else
                    {
                        printBusesRoutes(company);
                    }

                    std::cout << "Back home:\n";
                    setLocationsColorsAndLabels(company);
                    distance = manager->simulatedAnnealing(company, "garage");
                    if (distance == -1)
                    {
                        std::cout << "This trip is not possible due to graph connectivity\n";
                    }
                    else
                    {
                        printBusesRoutes(company);
                    }

                    // reset colors and labels
                    resetVerticesColorAndLabel();
                    setGarageColorAndLabel();
                }
            }
        }
        break;
        case 3:
        {
            companiesMenu();
        }
        break;
        case 4:
        {
            manageBuses();
        }
        break;
        default:
            done = true;
        }
    }
}

template <class T>
void Interface<T>::manageBuses()
{
    bool done = false;
    while (!done)
    {
        std::cout << "=====\n";
        std::cout << "Buses\n";
        std::cout << "=====\n";
        int index = 0;
        for (auto &bus : manager->getBuses())
        {
            std::cout << "Bus " << index << ":\n";
            std::cout << "  Capacity: " << bus.capacity << "\n";
            ++index;
        }
        std::cout << "\n1 - Add Bus\n";
        std::cout << "2 - Remove Bus\n";
        std::cout << "Any other key - Back to Menu\n";
        std::cout << "Option: ";

        int option;
        std::cin >> option;
        if (cin.fail())
        {
            option = -1;
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        std::vector<Bus<T>> &buses = manager->getBuses();

        switch (option)
        {
        case 1:
        {
            std::cout << "=======\n";
            std::cout << "Add Bus\n";
            std::cout << "=======\n";
            std::cout << "Any other key - Cancel Operation\n";
            std::cout << "New Bus Capacity (greater than 0): ";

            int capacity;
            std::cin >> capacity;
            if (cin.fail())
            {
                capacity = -1;
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }

            if (capacity > 0)
            {
                Bus<T> bus;
                bus.capacity = capacity;
                buses.push_back(bus);
            }
        }
        break;
        case 2:
        {
            std::cout << "==========\n";
            std::cout << "Remove Bus\n";
            std::cout << "==========\n";
            std::cout << "Any other key - Cancel Operation\n";
            std::cout << "Bus Index (0 - " << buses.size() - 1 << "): ";

            std::cin >> index;

            if (!cin.fail() && index >= 0 && index < (int)buses.size())
            {
                buses.erase(buses.begin() + index);
            }
            else if (cin.fail())
            {
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
        break;
        default:
            done = true;
            break;
        }
    }
}

template <class T>
void Interface<T>::companiesMenu()
{
    bool done = false;
    while (!done)
    {
        std::cout << "=========\n";
        std::cout << "Companies\n";
        std::cout << "=========\n";
        int company_index = 0;
        for (auto &company : manager->getCompanies())
        {
            std::cout << company_index << " - ";
            std::cout << "Company: " << company.name << "\n";
            ++company_index;
        }
        std::cout << "\n";
        std::cout << company_index << " - Add Company\n";
        ++company_index;
        std::cout << company_index << " - Remove Company\n";
        ++company_index;
        std::cout << "Any other key - Back To Menu\n\n";
        std::cout << "Option: ";

        int option;
        cin >> option;
        if (cin.fail())
        {
            option = -1;
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        if (option >= 0)
        {
            if (option < (int)manager->getCompanies().size())
            {
                manageCompanyMenu(manager->getCompanies()[option]);
            }
            else if (option == (int)manager->getCompanies().size())
            {
                addCompanyMenu();
            }
            else if (option == (int)manager->getCompanies().size() + 1)
            {
                removeCompanyMenu();
            }
        }
        else
        {
            done = true;
        }
    }
}

template <class T>
void Interface<T>::manageCompanyMenu(Company<T> &company)
{
    bool done = false;
    while (!done)
    {
        std::cout << "Company: " << company.name << "\n";
        std::cout << "  Location Vertex Id: " << company.company_vertex_id << "\n";
        std::cout << "  Bus Stops:\n";
        for (auto &stop : company.bus_stops)
        {
            std::cout << "      Stop:\n";
            std::cout << "          Vertex Id: " << stop.vertex_id << "\n";
            std::cout << "          Number of Workers: " << stop.number_of_workers << "\n";
        }

        std::cout << "\n1 - Add/Update/Remove Bus Stop\n";
        std::cout << "Any other key - Back to Companies\n";
        std::cout << "Option: ";

        int option;
        std::cin >> option;
        if (cin.fail())
        {
            option = -1;
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        switch (option)
        {
        case 1:
        {
            std::cout << "=========================\n";
            std::cout << "Add/Update/Remove a bus stop\n";
            std::cout << "=========================\n";
            std::cout << "INSTRUCTIONS:\n";
            std::cout << "Pick a valid vertex id\n";
            std::cout << "Choose number of workers\n";
            std::cout << "Vertex id is a bus stop and number of workers > 0: bus stop will be UPDATED\n";
            std::cout << "Vertex id is a bus stop and number of workers == 0: bus stop will be REMOVED\n";
            std::cout << "Vertex id is NOT a bus stop and number of workers > 0: bus stop will be ADDED\n";
            std::cout << "Vertex id is NOT a bus stop and number of workers == 0: nothing will change\n";
            std::cout << "\nAny other key - Cancel Operation\n";
            std::cout << "Bus Stop Vertex Id (0 - " << manager->getGraph().getNumVertex() - 1 << "): ";

            int vertex_id;
            std::cin >> vertex_id;

            if (!cin.fail() && vertex_id >= 0 && vertex_id < manager->getGraph().getNumVertex())
            {
                std::cout << "\nAny other key - Cancel Operation";
                std::cout << "\nNumber of workers: ";
                int number_of_workers;
                std::cin >> number_of_workers;
                if (cin.fail())
                {
                    number_of_workers = -1;
                    cin.clear();
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }

                if (number_of_workers >= 0)
                {
                    // check if company already has bus stop with the same vertex id
                    unsigned int index = 0;
                    std::vector<Stop<T>> &company_bus_stops = company.bus_stops;
                    for (auto &stop : company_bus_stops)
                    {
                        // if company already has bus stop with the same vertex id
                        if (stop.vertex_id == vertex_id)
                        {
                            break;
                        }

                        ++index;
                    }

                    if (index < company_bus_stops.size())
                    {
                        // update bus stop
                        if (number_of_workers > 0)
                        {
                            company_bus_stops[index].number_of_workers = number_of_workers;
                        }
                        // remove bus stop
                        else if (number_of_workers == 0)
                        {
                            company_bus_stops.erase(company_bus_stops.begin() + index);
                        }
                    }
                    else
                    {
                        // add new bus stop
                        if (number_of_workers > 0)
                        {
                            Stop<T> stop;
                            stop.vertex_id = vertex_id;
                            stop.number_of_workers = number_of_workers;
                            company_bus_stops.push_back(stop);
                        }
                    }
                }
            }
            else if (cin.fail())
            {
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
        break;
        default:
            done = true;
            break;
        }
    }
}

template <class T>
void Interface<T>::addCompanyMenu()
{
    std::cout << "===============\n";
    std::cout << "Add New Company\n";
    std::cout << "===============\n";
    std::cout << "New company name: ";

    std::string name;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, name);

    std::cout << "New company Vertex Id (0 - " << manager->getGraph().getNumVertex() - 1 << "): ";
    int company_vertex_id;
    std::cin >> company_vertex_id;

    if (!cin.fail() && company_vertex_id >= 0 && company_vertex_id < manager->getGraph().getNumVertex())
    {
        Company<T> company;
        company.name = name;
        company.company_vertex_id = company_vertex_id;
        manager->getCompanies().push_back(company);
    }
    else if (cin.fail())
    {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

template <class T>
void Interface<T>::removeCompanyMenu()
{
    std::vector<Company<T>> &companies = manager->getCompanies();

    std::cout << "===============\n";
    std::cout << "Remove Company\n";
    std::cout << "===============\n";
    std::cout << "Company index (0 - " << companies.size() - 1 << "): ";

    int index;
    std::cin >> index;

    if (!cin.fail() && index >= 0 && index < (int)companies.size())
    {
        companies.erase(companies.begin() + index);
    }
    else if (cin.fail())
    {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

template <class T>
void Interface<T>::printBusesRoutes(Company<T> company) const
{
    for (Bus<int> bus : manager->getBuses())
    {
        if (!bus.path.empty())
        {
            if (bus.path[0] == company.company_vertex_id ||
                bus.path[bus.path.size() - 1] == company.company_vertex_id)
            {
                std::cout << "bus capacity " << bus.capacity << std::endl;
                std::cout << "bus path size " << bus.path.size() << std::endl;
                for (unsigned int i = 0; i + 1 < bus.path.size(); i++)
                {
                    std::cout << bus.path[i] << " ";
                    if (i + 2 == bus.path.size())
                    {
                        std::cout << bus.path[i + 1] << " ";
                    }
                    manager->getGraph().dijkstraShortestPath(bus.path[i]);
                    manager->getGraph().getPathTo(bus.path[i + 1]);
                    Vertex<int> *vertex = manager->getGraph().findVertex(bus.path[i + 1]);
                    Vertex<int> *path = vertex->getPath();
                    while (path->getPath() != NULL)
                    {
                        gv->setVertexColor(path->getInfo(), RED);
                        path = path->getPath();
                    }
                }
                std::cout << "\n";
                gv->rearrange();
                getchar();

                // reset colors and labels
                resetVerticesColorAndLabel();
                setGarageColorAndLabel();
                setLocationsColorsAndLabels(company);
            }
        }
    }
}

template <class T>
void Interface<T>::resetVerticesColorAndLabel() const
{
    for (Vertex<T> *vertex : manager->getGraph().getVertexSet())
    {
        gv->setVertexColor(vertex->getInfo(), VERTEX_COLOR);
        gv->setVertexLabel(vertex->getInfo(), std::to_string(vertex->getInfo()));
    }

    gv->rearrange();
}

#endif /* INTERFACE_H_ */
