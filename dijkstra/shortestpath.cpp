/*
* -ShotestPath methods definition & implementation.
* -implements Dijkstra's algorithm to find shortest path between
*  two nodes of a graph
*/
#include <cassert>
#include "shortestpath.h"
#include "graph.h"
#include "priorityqueue.h"

//constructor initialized with reference to graph object
ShortestPath::ShortestPath(Graph &graph):
    m_graph(graph)
{};


//finds shortest path from nodeid0 to nodeid1 & returns it in the vecor provided, 
//along with the shortest path distance
int ShortestPath::findPath(int nodeid0, int nodeid1, vector<int>& spvector, double& spdist)
{
    //get all the nodes in graph. distances associated with them are 
    //already set to INFINITY
    vector<int> nodeidvector;
    m_graph.getNodes(nodeidvector);
    if (nodeidvector.empty() )
        return e_PATHNOTFOUND;

    //set the distance with nodeid0 to 0.0
    m_graph.setNodeDistance(nodeid0, 0.0);

    //create & initialize priority queue with the nodes
    PriorityQueue pqueue(m_graph, nodeidvector);

    int nodeid = pqueue.top();
    assert(m_graph.getNodeDistance(nodeid) == 0.0);
    double nodedist = m_graph.getNodeDistance(nodeid);
    vector<int> prevnode(m_graph.getNumNodes(), -1);
    int res = e_Ok;

	//loop for processing nodes using dijkstra's algo
    while(true)
    {
        pqueue.pop();
        if (pqueue.empty() )
            break;

        if (nodeid == nodeid1)
            break;

        list<int> nodeidlist;
        m_graph.getNodeNeighbours(nodeid, nodeidlist);

        //set distance to each neighbour
        for (auto it = nodeidlist.begin(); it != nodeidlist.end(); it++)
        {
            int curnodeid = *it;
            double distance = m_graph.getEdgeDistance(nodeid, curnodeid);
            assert(distance > 0);
            double newdist = distance + nodedist;
            if (newdist < m_graph.getNodeDistance(curnodeid) )
            {
                m_graph.setNodeDistance(curnodeid, newdist);
                prevnode[curnodeid] = nodeid;
            }
        }
        pqueue.reprioritize();

        nodeid = pqueue.top();
        nodedist = m_graph.getNodeDistance(nodeid);

        if(nodedist == INFINITY )
            break;
    }

    //reverse trace to get the shortest path found.
    int curnodeid = nodeid1;
    spvector.push_back(curnodeid);
    while(true)
    {
        curnodeid = prevnode[curnodeid];
        if (curnodeid == -1) 
        {
            res = e_PATHNOTFOUND;
            break;
        }
		
        spvector.push_back(curnodeid);

        if (curnodeid == nodeid0)
            break;
    }
	
    if (res == e_Ok) 
    {
        reverse(spvector.begin(), spvector.end());
        spdist = m_graph.getNodeDistance(nodeid1);
    }

    return res;
}
