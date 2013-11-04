/*
* class ShortestPath declaration.
* -Shortest Path algorithm implementation
* -uses Dijkstra's algorithm to get the shortest path between
*  two nodes in a graph
*/

#ifndef _SHORTESTPATH__H_
#define _SHORTESTPATH__H_

#include <vector>
using namespace std;

class Graph;

//shortest path algorithm implementation
class ShortestPath 
{
public:
    //initialized with reference to graph object
    ShortestPath(Graph &graph);

    //finds shortest path from nodeid0 to nodeid1 & returns it in the vecor provided, 
    //along with the shortest path distance
    int findPath(int nodeid0, int nodeid1, vector<int>& spvector, double& spdist);

private:

    //stores reference to graph object
    Graph &m_graph;

    //error codes used with the process.
    static const int e_Ok = 0;
    static const int e_PATHNOTFOUND = 1;
};

#endif  //_SHORTESTPATH__H_
