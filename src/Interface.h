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
        std::cout << "3 - View Companies\n";
        std::cout << "4 - View Buses\n";
        std::cout << "5 - Add/Remove/Change Company\n";
        std::cout << "6 - Add/Remove Bus\n";
        std::cout << "Any other key - Exit\n\n";
        std::cout << "Option: ";

        int option = -1;
        std::cin >> option;

        switch (option)
        {
        case 1:
            if (gv == NULL)
            {
                initializeGraphViewer();
            }
            openGraphViewerWindow();
            manager->getGraph().drawGraph(gv);
            setLocationsColorsAndLabels();
            break;
        case 2:
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
            break;
        case 3:

            break;
        case 4:

            break;
        case 5:

            break;
        case 6:

            break;
        default:
            done = true;
        }
    }
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
