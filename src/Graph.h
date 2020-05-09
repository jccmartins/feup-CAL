/*
 * Graph.h
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
#include <list>
#include <limits>
#include <cmath>
#include <string>
#include <sstream>
#include <fstream>
#include "MutablePriorityQueue.h"
#include "lib/graphviewer.h"

template <class T>
class Edge;
template <class T>
class Graph;
template <class T>
class Vertex;

#define INF std::numeric_limits<double>::max()

/************************* Vertex  **************************/

template <class T>
class Vertex
{
	T info;					   // content of the vertex
	vector<Edge<T>> edges_out; // outgoing edges

	double x, y; // x and y coordinates

	double dist = 0;
	Vertex<T> *path = NULL;
	int queueIndex = 0; // required by MutablePriorityQueue

	bool visited = false;	 // auxiliary field
	bool processing = false; // auxiliary field

	void addEdge(Vertex<T> *dest, double w);

public:
	Vertex(T in, double x, double y);
	T getInfo() const;
	double getDist() const;
	Vertex *getPath() const;

	bool operator<(Vertex<T> &vertex) const; // // required by MutablePriorityQueue
	friend class Graph<T>;
	friend class MutablePriorityQueue<Vertex<T>>;
};

template <class T>
Vertex<T>::Vertex(T in, double x, double y) : info(in), x(x), y(y) {}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template <class T>
void Vertex<T>::addEdge(Vertex<T> *d, double w)
{
	edges_out.push_back(Edge<T>(d, w));
}

template <class T>
bool Vertex<T>::operator<(Vertex<T> &vertex) const
{
	return this->dist < vertex.dist;
}

template <class T>
T Vertex<T>::getInfo() const
{
	return this->info;
}

template <class T>
double Vertex<T>::getDist() const
{
	return this->dist;
}

template <class T>
Vertex<T> *Vertex<T>::getPath() const
{
	return this->path;
}

/********************** Edge  ****************************/

template <class T>
class Edge
{
	Vertex<T> *dest; // destination vertex
	double weight;	 // edge weight
public:
	Edge(Vertex<T> *d, double w);
	friend class Graph<T>;
	friend class Vertex<T>;
};

template <class T>
Edge<T>::Edge(Vertex<T> *d, double w) : dest(d), weight(w) {}

/*************************** Graph  **************************/

template <class T>
class Graph
{
	vector<Vertex<T> *> vertexSet; // vertex set

	Vertex<T> *initSingleSource(const T &orig);
	bool relax(Vertex<T> *v, Vertex<T> *w, double weight);

public:
	Graph();
	Vertex<T> *findVertex(const T &in) const;
	bool addVertex(const T &in, double x, double y);
	bool addEdge(const T &sourc, const T &dest);
	int getNumVertex() const;
	vector<Vertex<T> *> getVertexSet() const;

	void dijkstraShortestPath(const T &s);
	vector<T> getPathTo(const T &dest) const;

	void loadNodesAndEdges(string city_name);
	void drawGraph(GraphViewer *gv);
};

template <class T>
Graph<T>::Graph() {}

/**
 * Initializes single-source shortest path data (path, dist).
 * Receives the content of the source vertex and returns a pointer to the source vertex.
 * Used by all single-source shortest path algorithms.
 */
template <class T>
Vertex<T> *Graph<T>::initSingleSource(const T &origin)
{
	for (auto v : vertexSet)
	{
		v->dist = INF;
		v->path = nullptr;
	}
	auto s = findVertex(origin);
	s->dist = 0;
	return s;
}

/**
 * Analyzes an edge in single-source shortest path algorithm.
 * Returns true if the target vertex was relaxed (dist, path).
 * Used by all single-source shortest path algorithms.
 */
template <class T>
bool Graph<T>::relax(Vertex<T> *v, Vertex<T> *w, double weight)
{
	if (v->dist + weight < w->dist)
	{
		w->dist = v->dist + weight;
		w->path = v;
		return true;
	}
	else
		return false;
}

template <class T>
int Graph<T>::getNumVertex() const
{
	return vertexSet.size();
}

template <class T>
vector<Vertex<T> *> Graph<T>::getVertexSet() const
{
	return vertexSet;
}

/*
 * Auxiliary function to find a vertex with a given content.
 */
template <class T>
Vertex<T> *Graph<T>::findVertex(const T &in) const
{
	for (auto v : vertexSet)
		if (v->info == in)
			return v;
	return NULL;
}

/*
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
template <class T>
bool Graph<T>::addVertex(const T &in, double x, double y)
{
	if (findVertex(in) != NULL)
		return false;
	vertexSet.push_back(new Vertex<T>(in, x, y));
	return true;
}

/*
 * Adds an edge to a graph (this), given the contents of the source and
 * destination vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest)
{
	auto v1 = findVertex(sourc);
	auto v2 = findVertex(dest);
	if (v1 == NULL || v2 == NULL)
		return false;
	double weight = sqrt(pow(v1->x - v2->x, 2) + pow(v1->y - v2->y, 2));
	v1->addEdge(v2, weight);
	return true;
}

/**
 * Dijkstra algorithm.
 */
template <class T>
void Graph<T>::dijkstraShortestPath(const T &origin)
{
	auto s = initSingleSource(origin);
	MutablePriorityQueue<Vertex<T>> q;
	q.insert(s);
	while (!q.empty())
	{
		auto v = q.extractMin();
		for (auto edge : v->edges_out)
		{
			auto oldDist = edge.dest->dist;
			if (relax(v, edge.dest, edge.weight))
			{
				if (oldDist == INF)
					q.insert(edge.dest);
				else
					q.decreaseKey(edge.dest);
			}
		}
	}
}

template <class T>
vector<T> Graph<T>::getPathTo(const T &dest) const
{
	vector<T> res;
	auto current = findVertex(dest);
	while (current != NULL)
	{
		res.insert(res.begin(), current->info);
		current = current->path;
	}
	return res;
}

/**
 * Load vertices and edges from .txt files and store them in the graph
*/
template <class T>
void Graph<T>::loadNodesAndEdges(string city_name)
{
	std::string nodes_filename, edges_filename;
	std::ifstream nodes, edges;
	int edgeType;

	if (city_name == "testing")
	{
		nodes_filename = "resources/Mapas-20200424/GridGraphs/16x16/nodes.txt";
		edges_filename = "resources/Mapas-20200424/GridGraphs/16x16/edges.txt";
	}
	else
	{
		string city_name_lowercase = city_name;
		city_name_lowercase[0] = tolower(city_name_lowercase[0]);

		nodes_filename = "resources/Mapas-20200424/PortugalMaps/PortugalMaps/" + city_name + "/nodes_x_y_" + city_name_lowercase + ".txt";
		edges_filename = "resources/Mapas-20200424/PortugalMaps/PortugalMaps/" + city_name + "/edges_" + city_name_lowercase + ".txt";
	}

	// UNDIRECTED com grids, DIRECTED com maps
	// professor deu-nos permissao para usar UNDIRECTED em todos os mapas devido à má conetividade
	edgeType = EdgeType::UNDIRECTED;

	nodes.open(nodes_filename);
	if (!nodes.good())
	{
		std::cout << "Unable to access file " << nodes_filename << std::endl;
		return;
	}

	edges.open(edges_filename);
	if (!edges.good())
	{
		std::cout << "Unable to access file " << edges_filename << std::endl;
		return;
	}

	std::string line;
	std::istringstream iss;
	unsigned int node_id, n_nodes, n_edges, node_id_origin, node_id_destination;
	double x, y;

	// read num of nodes
	std::getline(nodes, line);
	iss.str(line);
	iss >> n_nodes;

	// load nodes
	for (unsigned int i = 0; i < n_nodes; i++)
	{
		std::getline(nodes, line);
		sscanf(line.c_str(), "(%d, %lf, %lf)", &node_id, &x, &y);
		addVertex(node_id, x, y);
	}

	// read num of edges
	std::getline(edges, line);
	sscanf(line.c_str(), "%d", &n_edges);

	//load edges
	for (unsigned int i = 0; i < n_edges; i++)
	{
		std::getline(edges, line);
		sscanf(line.c_str(), "(%d, %d)", &node_id_origin, &node_id_destination);
		if (edgeType == EdgeType::UNDIRECTED)
		{
			addEdge(node_id_destination, node_id_origin);
		}
		addEdge(node_id_origin, node_id_destination);
	}

	nodes.close();
	edges.close();
}

/** 
 * Draw all vertices and edges of the map
*/
template <class T>
void Graph<T>::drawGraph(GraphViewer *gv)
{
	double relative_x, relative_y;
	unsigned int edge_id = 0;

	// add vertices
	for (unsigned int i = 0; i < vertexSet.size(); i++)
	{
		Vertex<T> *vertex = vertexSet[i];

		if (i == 0)
		{
			relative_x = vertex->x;
			relative_y = vertex->y;
		}

		gv->addNode(vertex->info, vertex->x - relative_x, vertex->y - relative_y);
		gv->setVertexSize(vertex->info, 16);
	}

	// add edges
	for (unsigned int i = 0; i < vertexSet.size(); i++)
	{
		Vertex<T> *vertex = vertexSet[i];

		for (auto edge : vertex->edges_out)
		{
			gv->addEdge(edge_id, vertex->info, edge.dest->info, EdgeType::DIRECTED);
			++edge_id;
		}
	}

	gv->rearrange();
}

#endif /* GRAPH_H_ */
