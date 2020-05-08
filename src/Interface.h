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
    void setLocationsColorsAndLabels() const;
    void chooseMap();
    void initializeGraphViewer();
    void menu();
    void companiesMenu();
    void manageCompanyMenu(Company<T> &company);
    void addCompanyMenu();
    void removeCompanyMenu();
    void printBusesRoutes(Company<T> company) const;
    void resetVerticesColor() const;
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

/**
 * Set colors for companies, garage and bus stops vertices
 */
template <class T>
void Interface<T>::setLocationsColorsAndLabels() const
{
    // set company vertex color
    vector<Company<int>> companies = manager->getCompanies();
    for (unsigned int i = 0; i < companies.size(); i++)
    {
        std::cout << "Company: " << companies[i].getName() << " vertex id: " << companies[i].getCompanyVertexId() << endl;
        gv->setVertexColor(companies[i].getCompanyVertexId(), COMPANY_COLOR);
        std::string company_label = companies[i].getName() + " - " + std::to_string(companies[i].getCompanyVertexId());
        std::cout << "label " << company_label << endl;
        gv->setVertexLabel(companies[i].getCompanyVertexId(), company_label);

        // set bus stops vertices color
        vector<Stop<int>> bus_stops = companies[i].getBusStops();
        cout << "Bus stops: \n";
        for (auto stop : bus_stops)
        {
            cout << stop.vertex_id << " " << stop.number_of_workers << std::endl;
            gv->setVertexColor(stop.vertex_id, BUS_STOP_COLOR);
        }
    }

    // set garage vertex color
    unsigned int garage = manager->getGarageVertexId();
    std::cout << "Garage: " << garage << endl;
    gv->setVertexColor(garage, GARAGE_COLOR);
    std::string garage_label = "GARAGE - " + std::to_string(garage);
    gv->setVertexLabel(garage, garage_label);
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
        menu();
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
        std::cout << "input option " << option << "\n";

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
            setLocationsColorsAndLabels();
        }
        break;
        case 2:
        {
            manager->sortBusesAscendingCapacity();

            manager->clearBusesPaths();

            std::cout << "To work:\n";
            for (auto &company : manager->getCompanies())
            {
                manager->simulatedAnnealing(company, "company");
                printBusesRoutes(company);
            }

            std::cout << "Back home:\n";
            for (auto &company : manager->getCompanies())
            {
                manager->simulatedAnnealing(company, "garage");
                printBusesRoutes(company);
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
        }
        break;
        default:
            done = true;
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
        unsigned int company_index = 1;
        for (auto &company : manager->getCompanies())
        {
            std::cout << company_index << " - ";
            std::cout << "Company: " << company.getName() << "\n";
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

        if (option > 0)
        {
            if (option <= (int)manager->getCompanies().size())
            {
                manageCompanyMenu(manager->getCompanies()[option - 1]);
            }
            else if (option == (int)manager->getCompanies().size() + 1)
            {
                addCompanyMenu();
            }
            else if (option == (int)manager->getCompanies().size() + 2)
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
        for (auto &company : manager->getCompanies())
        {
            std::cout << "Company: " << company.getName() << "\n";
            std::cout << "  Location Vertex Id: " << company.getCompanyVertexId() << "\n";
            std::cout << "  Bus Stops:\n";
            for (auto &stop : company.getBusStops())
            {
                std::cout << "      Stop:\n";
                std::cout << "          Vertex Id: " << stop.vertex_id << "\n";
                std::cout << "          Number of Workers: " << stop.number_of_workers << "\n";
            }
        }
        std::cout << "1 - Add/Update/Remove Bus Stop\n";
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
                    std::vector<Stop<T>> &company_bus_stops = company.getBusStops();
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
}

template <class T>
void Interface<T>::removeCompanyMenu()
{
}

template <class T>
void Interface<T>::printBusesRoutes(Company<T> company) const
{
    for (Bus<int> bus : manager->getBuses())
    {
        if (!bus.path.empty())
        {
            if (bus.path[0] == company.getCompanyVertexId() ||
                bus.path[bus.path.size() - 1] == company.getCompanyVertexId())
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

                // reset colors
                resetVerticesColor();
                setLocationsColorsAndLabels();
                gv->rearrange();
            }
        }
    }
}

template <class T>
void Interface<T>::resetVerticesColor() const
{
    for (Vertex<T> *vertex : manager->getGraph().getVertexSet())
    {
        gv->setVertexColor(vertex->getInfo(), VERTEX_COLOR);
    }
}

#endif /* INTERFACE_H_ */
