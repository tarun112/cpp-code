/*
*  class Graph definitions & implementation.
*  -Methods to initialize and process graph are implemented.
*/

#include <cassert>
#include <chrono>
#include <random>
#include <fstream>
#include <string>
#include "graph.h"
using namespace std;

//constructor initialized with data from input file.
//throws exception if cannot read input file.
Graph::Graph(string inputFile):
    m_numNodes(0), m_numEdges(0)
{
    ifstream infile(inputFile.c_str());
    if (infile.is_open())
    {
        string line;
        infile >> m_numNodes ;

        int nodeid0;
        int nodeid1;
        int cost;
        while ( !infile.eof() )
        {
            infile >> nodeid0 >> nodeid1 >> cost;
            int res = insertEdge(nodeid0, nodeid1, cost);
            if (res == e_Ok)
                m_numEdges++;
        }
        infile.close();
    }
    else
        throw string("Cannot open input file: ") + inputFile;
}


//Graph constructor. initialized with number of nodes
Graph::Graph(int numnodes):
    m_numNodes(numnodes), m_numEdges(0)
{}

//randomly adds edges to the graph till density limit is reached
//and associates a random distance with each edge.
int Graph::initialize(double edgedensity, pair<double,double> distancerange) 
{
	
    assert(edgedensity <= 1.0);

    //total possible num of edges is N*(N-1)/2
    int possiblenumedges = (m_numNodes * (m_numNodes - 1)) / 2;
    double density = 0.0;
  
    //these are stl uniform random generators, seeded with current time.
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();	
    default_random_engine generator(seed);

    //random real(double) distribution for distance. initialized with distance range.
    uniform_real_distribution<double> distRangeDistribution(distancerange.first, distancerange.second);
	
    //random int distribution for nodeids.  id range from 0 to N-1
    uniform_int_distribution<int> nodeDistribution(0, m_numNodes-1);

    //loop to populate edges while checking for limiting density
    while (density < edgedensity)
    {
        //randomly find two nodes for the edge
        int nodeid0 = nodeDistribution(generator);
        int nodeid1 = nodeDistribution(generator);

        //if both nodes are same than it is a loop, which is discarded in this case
        if (nodeid0 == nodeid1)
	    continue;

        //get a random distance within specified range
        double distance = distRangeDistribution(generator);

        //insert the edge between node0 & node1
        int res = 0;
        res = insertEdge(nodeid0, nodeid1, distance);
        if (res == e_EdgeAlreadyPresent)
	    continue;

        //since graph is undirectional, insert the edge between node1 & node0
        res = insertEdge(nodeid1, nodeid0, distance);

        //increment num of edges
        m_numEdges++;

        density = static_cast<double>(m_numEdges) / possiblenumedges;
    }

    //initialize all distances associated with nodes as INFINITY
    for (int i=0; i<m_numNodes; i++)
        m_nodeidDistMap[i] = INFINITY_DOUBLE;

    return e_Ok;
}

//associate the given distance(cost) with all nodes
int Graph::initializeNodesDistance(double distance) 
{
    for (int i=0; i<m_numNodes; i++)
        m_nodeidDistMap[i] = distance;

    return e_Ok;
}

//inserts an edge with given distance from nodeid0 to nodeid1
int Graph::insertEdge(int nodeid0, int nodeid1, double distance)
{
    list<EdgeDistancePair> & edgelist = m_adjacencyEdgeMap[nodeid0];
			
    //check if nodeid1 already exists in the list.  
    //using stl find_if with lamda comparison.
    auto edgelistiterator = 
        find_if(edgelist.begin(), edgelist.end(), 
                [nodeid1](EdgeDistancePair const &tmp) {
                    return tmp.first == nodeid1;
                }
        );

    //if edge is present then it is not added again
    if (edgelistiterator != edgelist.end())
        return e_EdgeAlreadyPresent;

    //else insert the edge-distance in the list
    edgelist.push_back(EdgeDistancePair(nodeid1, distance));

    return e_Ok;
}

//returns number of nodes in the graph
int Graph::getNumNodes() const
{ 
    return m_numNodes; 
}

//returns node ids in the vector provided.
int Graph::getNodes(vector<int>& nodeidvector)
{
    //initialize all distances associated with nodes as INFINITY
    if (m_nodeidDistMap.size() != m_numNodes)
        initializeNodesDistance(INFINITY_DOUBLE);

    //this is more generic way of populating idvector with nodeids.
    //since nodeids are int in this case they can be done directly using for loop.
    for (auto it = m_nodeidDistMap.begin(); it != m_nodeidDistMap.end(); it++)
        nodeidvector.push_back(it->first);
	
    return e_Ok;
}

int Graph::makeEdgeid(int startnodeid, int endnodeid) const
{
    return startnodeid * EDGEID_START_NODE_MULTIPLIER + 
        endnodeid; 
}

int Graph::getEdgeNodes(int edgeid, int& startnodeid, int& endnodeid) const
{
    endnodeid = edgeid % EDGEID_START_NODE_MULTIPLIER;
    startnodeid = (edgeid - endnodeid) / EDGEID_START_NODE_MULTIPLIER;

    return e_Ok;
}

//returns edge ids in the vector provided.
int Graph::getEdges(vector<int> &edgeidvec) 
{
    //edge id 100*nodeid0+nodeid1  so for nodeid0=23,nodeid1=35 edgeid=2335
    for (auto it = m_adjacencyEdgeMap.begin(); it != m_adjacencyEdgeMap.end(); ++it)
    {
        int nodeid0 = it->first;
        list<EdgeDistancePair> &edgedistlist =  it->second;
        for (auto it2 = edgedistlist.begin(); it2 != edgedistlist.end(); ++it2)
        {
            int nodeid1 = it2->first;
            double distance = it2->second;
            int edgeid = makeEdgeid(nodeid0, nodeid1);
            m_edgeidDistMap[edgeid] = distance;
        }
    }

    for (auto it = m_edgeidDistMap.begin(); it != m_edgeidDistMap.end(); ++it)
        edgeidvec.push_back(it->first);
	
    return e_Ok;
}


//for the given nodeid returns all the neighbour nodes ids in the list provided.
int Graph::getNodeNeighbours(int nodeid, list<int>& nodeidlist) const
{
    const list<EdgeDistancePair>& edlist = m_adjacencyEdgeMap.at(nodeid);
    for(auto it = edlist.begin(); it != edlist.end(); ++it)
        nodeidlist.push_back( (*it).first );
	
    return e_Ok;
}

//for the nodeid set the given distance
int Graph::setNodeDistance(int nodeid, double distance)
{
    m_nodeidDistMap[nodeid] = distance;
    return e_Ok;
}

//for the given node returns the distance
double Graph::getNodeDistance(int nodeid) const
{
    return m_nodeidDistMap.at(nodeid);
}

//for the edge between nodeid0, nodeid1 returns the distance
double Graph::getEdgeDistance(int nodeid0, int nodeid1) const
{
    const list<EdgeDistancePair>& edlist = m_adjacencyEdgeMap.at(nodeid0);

    auto edgelistiterator = 
        find_if(edlist.begin(), edlist.end(), 
            [nodeid1](EdgeDistancePair const &tmp) {
                return tmp.first == nodeid1;
            }
        );

    //if edge is present then it is not added again
    if (edgelistiterator != edlist.end())
        return (*edgelistiterator).second;

    return 0.0;
}

//for the edgeid returns the distance
int Graph::getEdgeDistance(int edgeid, double &distance) const
{
    auto it = m_edgeidDistMap.find(edgeid);
    if (it == m_edgeidDistMap.end() )
        return e_Nok;
    
    distance =  it->second;
    return e_Ok;
}

