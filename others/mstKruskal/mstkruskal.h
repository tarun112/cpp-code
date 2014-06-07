/*
* -Minimun spanning tree using Kruskal's algorithm
*/
#ifndef _MSTKRUSKAL__H_
#define _MSTKRUSKAL__H_

#include "graph.h"

//enum to return messages & error status
enum Status
{
    NOK = -1,
    OK = 0,
    DISJOINT = 1
};

//declaration of class
class MinSpanTreeKruskal
{
public:
    //constructor initialized with reference to graph
    MinSpanTreeKruskal(Graph &graph) :
        m_graph(graph)
    {};

    //uses Kruskal's algorithm to find minimum spanning tree over graph
    //returns tree edges and cost
    int findTree(vector<int>& mstedges, double& mstcost);

private:
    Graph   &m_graph;   //reference to graph for tree
};


#endif  //_MSTKRUSKAL__H_