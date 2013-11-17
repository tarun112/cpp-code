/*
* -PriorityQueue methods definitions & implementations.
* -returns elements according to given priorities.
*/
#include "priorityqueue.h"	
#include "graph.h"

//initialized with reference to graph & the nodeid vector
PriorityQueue::PriorityQueue(Graph& graph, vector<int>& invec):
    m_graph(graph),	m_vec(invec.begin(), invec.end()),
    compare(CompareDistance(graph))
{	
    //using stl heap functions to arrange elements using priorities
    make_heap(m_vec.begin(), m_vec.end(), compare);	
}
	
//returns the top element
int PriorityQueue::top() 
{
    return m_vec.front();
}

//pops the top element
void PriorityQueue::pop()
{
    pop_heap(m_vec.begin(), m_vec.end(), compare); 
    m_vec.pop_back();
}

//returns true if the queue is empty
bool PriorityQueue::empty()
{
    return m_vec.empty();
}

//set changed priorities 
void PriorityQueue::reprioritize()
{
    make_heap(m_vec.begin(), m_vec.end(), compare);
}

//comparison class constructor. initialized with reference to graph
PriorityQueue::CompareDistance::CompareDistance(Graph &graph):
    m_graph(graph)
{}

//comparision operator to compare nodeid w.r.t to their distance values
bool PriorityQueue::CompareDistance::operator()(int id0, int id1)
{
//    return m_graph.getNodeDistance(nodeid1) < m_graph.getNodeDistance(nodeid0);
    double dist0;
    m_graph.getEdgeDistance(id0, dist0);

    double dist1;
    m_graph.getEdgeDistance(id1, dist1);

    return (dist1 < dist0);
}

