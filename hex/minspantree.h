/*
* -MinSpanTree declaration
* -Uses Prim's algorithm to find minimum spanning tree for a graph
*/
#ifndef _MINSPANTREE__H_
#define _MINSPANTREE__H_

#include <vector>
using namespace std;

class Graph;
class Hex;

//class MinSpanTree declaration
class MinSpanTree 
{
public:
    //constructor initialized with refrence to graph and hex
    MinSpanTree(Graph &graph, Hex &hex):
        m_hex(hex), m_graph(graph), m_compare(CompareEdgeDistance(graph))
    {};

    //finds minimum spanning tree for all graph and return the tree edges and cost
    int findTree(vector<int> & mstedges, double &mstcost);

    //finds minimum spanning tree for given startnodeid in subset of graph 
    //  of its color
    //returns tree edges and cost
    int findTree(int startnodeid, vector<int> & mstedges, double &mstcost);

    //finds minimum spanning tree for given startnodeid, edgeids and nodeids 
    //  as subset of graph 
    //returns tree edges and cost
    int findTree(int startnodeid, vector<int>& edgeidvec, 
        vector<int>& nodeidvec, vector<int> & mstedges, double &mstcost);

private:
    Graph  &m_graph;  //graph for mst
    Hex  &m_hex;    //mst for hex

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
