/*
* -Graph class declaration. Graph is made up of nodes & edges.
* -'nodes' are identified by int ids from 0 to N-1 where 
*  N is the number of nodes in the graph.
* -Edges are stored as edge list
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
const double INFINITY = numeric_limits<double>::max();

class Graph 
{

public:
    //initialized with number of nodes
    Graph(int numNodes);

    //randomly adds edges to the graph till density limit is reached
    //and associates a random distance with each edge.
    int initialize(double edgedensity, pair<double,double> distancerange);

    //returns number of nodes in the graph
    int getNumNodes();

    //returns node ids in the vector provided.
    int getNodes(vector<int>& nodeidvec);

    //for the given nodeid returns all the neighbour nodes ids in the list provided.
    int getNodeNeighbours(int nodeid, list<int>& nodeidlist);

    //for the nodeid set the given distance
    int setNodeDistance(int nodeid, double distance); 

    //for the given node returns the distance
    double getNodeDistance(int nodeid); 

    //for the edge between nodeid0, nodeid1 returns the distance
    double getEdgeDistance(int nodeid0, int nodeid1);

private: 
    //inserts an edge with given distance from nodeid0 to nodeid1
    int insertEdge(int nodeid0, int nodeid1, double distance);

private:
    //data structures to store distances with nodes and to
    //associate nodeids with them.
    typedef pair<int, double> NodeDistancePair;
    typedef map<int, NodeDistancePair> NodeNodeDistancePairMap;
    NodeNodeDistancePairMap m_nodeMap;

    //data structures to store distance with edges
    //edges are between the map-key which is a nodeid and 
    //the map-value which is a list of nodeids with distance
    typedef pair<int, double> EdgeDistancePair;
    typedef	map<int, list<EdgeDistancePair>> EdgeMap;
    EdgeMap m_edgeMap;

    //number of nodes in the graph
    int m_numNodes;

    //number of undirectional edges in the graph
    int m_numEdges;

    //some error codes used while processing graph
    static const int e_EdgeAlreadyPresent = 1;
    static const int e_Ok = 0;
};


#endif  //_GRAPH__H_

