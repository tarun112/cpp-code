/*
* Class PriorityQueue declaration 
* -Priority Queue implementation required for dijkstra algorithm
* -returns element according to given priorities
*/
#ifndef _PRIORITYQUEUE__H_
#define _PRIORITYQUEUE__H_

#include <vector>
#include <queue>
#include "graph.h"
using namespace std;

class PriorityQueue 
{
public:
    //initialized with reference to graph & the nodeid vector
    PriorityQueue(Graph& graph, vector<int>& invec);
	
    //returns the top element
    int top();

    //pops the top element
    void pop();

    //returns true if the queue is empty
    bool empty();

    //set changed priorities 
    void reprioritize();

private:
    //container for the queue
    vector<int> m_vec;

    //reference to graph
    Graph &m_graph;

    //class required for comparision operator
    class CompareNodeDistance 
    {
    public:
        //initialized with reference to graph
        CompareNodeDistance(Graph &graph);

        //comparision operator to compare nodeid w.r.t to their distance values
        bool operator()(int nodeid0, int nodeid1);

    private:
        //reference to graph
        Graph &m_graph;
    };

    CompareNodeDistance compare;
};


#endif  //_PRIORITYQUEUE__H_
