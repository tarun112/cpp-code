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


//Graph constructor. initialized with number of nodes
Graph::Graph(int numnodes):
    m_numNodes(numnodes), m_numEdges(0)
{}

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

//make edgeid from its nodes ids
//edgeid is computed as nodeid0 * multiplier + nodeid1
int Graph::makeEdgeid(int startnodeid, int endnodeid) const
{
    return startnodeid * EDGEID_START_NODE_MULTIPLIER + endnodeid; 
}

//for edgeid return its nodes ids
int Graph::getEdgeNodes(int edgeid, int& startnodeid, int& endnodeid) const
{
    endnodeid = edgeid % EDGEID_START_NODE_MULTIPLIER;
    startnodeid = (edgeid - endnodeid) / EDGEID_START_NODE_MULTIPLIER;

    return e_Ok;
}

//returns edge ids in the vector provided.
//edge id 10000*nodeid0+nodeid1  so for nodeid0=23,nodeid1=35 edgeid=230035
int Graph::getEdges(vector<int> &edgeidvec) 
{
    //loop over edge-list adjacency map to collect and return edges
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

