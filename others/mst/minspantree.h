/*
* -MinSpanTree declaration
* -Uses Prim's algorithm to find minimum spanning tree for a graph
*/
#ifndef _MINSPANTREE__H_
#define _MINSPANTREE__H_

#include <vector>
using namespace std;

class Graph;

class MinSpanTree 
{
public:
    //constructor initialized with refrence to graph
    MinSpanTree(Graph &graph):
        m_graph(graph), m_compare(CompareEdgeDistance(graph))
    {};

    //finds minimum spanning tree and return the tree edges and cost
    int findTree(vector<int> & mstedges, double &mstcost);

private:

    Graph &m_graph;

    //compare class required by std::sort for sorting edges w.r.t edge costs
    class CompareEdgeDistance
    {
    public:
        //construcot initialized with refrence to graph
        CompareEdgeDistance(Graph &graph):
            m_graph(graph)
        {}

        //compares distances for given edgeids
        bool operator()(int edgeid0, int edgeid1);

    private:
        Graph &m_graph;
    };

    CompareEdgeDistance m_compare;

};

#endif  //_MINSPANTREE__H_
