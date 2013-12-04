/*
* -Graph class declaration. Graph is made up of nodes & edges.
* -'nodes' are identified by int ids from 0 to N-1 where 
*     N is the number of nodes in the graph.
* -Edges are stored as adjacency edge list
*/

#ifndef _GRAPH__H_
#define _GRAPH__H_

#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <limits>

using namespace std;

//INFINITY definition as double max
const double INFINITY_DOUBLE = numeric_limits<double>::max();

//multiplier used to create edgeid from its nodes ids.
const int EDGEID_START_NODE_MULTIPLIER = 10000;

//class Graph 
//encapsulates all variable & methods w.r.t node and edge ids (int in this case)
class Graph 
{

public:
    //initialized with number of nodes
    Graph(int numNodes);

    //Graph(const Graph& graph);

    //set given distance with all nodes
    int initializeNodesDistance(double distance); 

    //returns number of nodes in the graph
    int getNumNodes() const;

    //returns node ids in the vector provided.
    int getNodes(vector<int>& nodeidvec);

    //for given nodeid returns all the neighbour nodes ids in the list provided.
    int getNodeNeighbours(int nodeid, list<int>& nodeidlist) const;

    //for nodeid set the given distance
    int setNodeDistance(int nodeid, double distance); 

    //for given node returns the distance
    double getNodeDistance(int nodeid) const; 

    //returns edge ids in the vector provided.
    int getEdges(vector<int> &edgeidvec);

    //for edge between nodeid0, nodeid1 returns the distance
    double getEdgeDistance(int nodeid0, int nodeid1) const;

    //for edgeid returns the distance
    int getEdgeDistance(int edgeid, double &distance) const;

    //for edgeid returns startnode, endnode
    int getEdgeNodes(int edgeid, int& startnodeid, int& endnodeid) const;

    //inserts an edge with given distance from nodeid0 to nodeid1
    int insertEdge(int nodeid0, int nodeid1, double distance);

    //makes an int edgeid for given start and end nodes int ids.
    int makeEdgeid(int startnodeid, int endnodeid) const;

private:
    //data structures to store distances with nodes and to
    //associate nodeids with them.
    typedef map<int, double> NodeidDistanceMap;
    NodeidDistanceMap m_nodeidDistMap;

    //adjacency edge list
    //data structures to store distance with edges
    //edges are between the map-key which is a nodeid and 
    //the map-value which is a list of nodeids with distance
    typedef pair<int, double> EdgeDistancePair;
    typedef map<int, list<EdgeDistancePair>> AdjacencyEdgeMap;
    AdjacencyEdgeMap m_adjacencyEdgeMap;

    //edgeid-distance map
    typedef map<int, double> EdgeidDistanceMap;
    EdgeidDistanceMap m_edgeidDistMap;


    //number of nodes in the graph
    int m_numNodes;

    //number of undirectional edges in the graph
    int m_numEdges;

public:
    //some error codes used while processing graph
    enum StatGraph 
    {
        e_EdgeAlreadyPresent = 1,
        e_Ok = 0,
        e_Nok = -1
    };
   
};


#endif  //_GRAPH__H_

