/*
*  -implementation of minimum spanning tree over graph 
*     using Kruskal's algorithm
*/

#include <vector>
#include <set>
#include "graph.h"
#include "mstkruskal.h"
#include "priorityqueue.h"

//uses Kruskal's algorithm to find minimum spanning tree over graph
int MinSpanTreeKruskal::findTree(vector<int> &mstedges, double &mstcost)
{
    vector<int> nodeidvec;
    int res = m_graph.getNodes(nodeidvec);  //get graph nodes

    //create set for each node in graph
    vector<set<int>> tmpvec(nodeidvec.size());
    for (size_t i=0; i<nodeidvec.size(); ++i)
        tmpvec[i].insert(nodeidvec[i]);  //tmp vector converted to list

    list<set<int>> nodesetslist(tmpvec.begin(), tmpvec.end());

    vector<int> edgeidvec;
    res = m_graph.getEdges(edgeidvec);  //get greaph edges

    //creating priority queue w.r.t distance for edges in graph
    PriorityQueue pqueue(m_graph, edgeidvec);

    //mst edges
    vector<int> tedgevec;
        
    //loop over sorted edges with minimum cost for processing
    while ( ! pqueue.empty() )
    {
        int edgeid = pqueue.top();

        int nodeid0, nodeid1;
        m_graph.getEdgeNodes(edgeid, nodeid0, nodeid1);

        //tmp iterators to contain edge nodes sets. initialized to list end.
        auto lstit0 = nodesetslist.end();
        auto lstit1 = nodesetslist.end();

        //search for sets containing edge nodes. 
        for (auto it = nodesetslist.begin(); it != nodesetslist.end(); ++it)
        {
            //find sets containing two nodes and set them to two tmp iterators
            set<int> & nodeset = *it;
            if ((lstit0 == nodesetslist.end()) && (nodeset.find(nodeid0) != nodeset.end()))
                lstit0 = it;

            if ((lstit1 == nodesetslist.end()) && (nodeset.find(nodeid1) != nodeset.end()))
                lstit1 = it;

            //if both sets are found
            if ( (lstit0 != nodesetslist.end()) && (lstit1 != nodesetslist.end()) )
            {
                if (lstit0 == lstit1)
                {
                    //if both nodes belong to same set than discard the edge
                    pqueue.pop();
                    break;
                }
                else
                {
                    //merge second set with first and erase second from list
                    lstit0->insert(lstit1->begin(), lstit1->end());
                    nodesetslist.erase(lstit1);
                    
                    //add edge to tree and remove it from the priority queue
                    tedgevec.push_back(edgeid);
                    pqueue.pop();
                    break;
                }
            }
        }

    }

    //check for number of connected MST components found 
    int numcomp = nodesetslist.size();
    if (numcomp > 1 )
        return Status::DISJOINT;  //if more than two return message

    //if one connected MST component, return tree edges & total cost 
    mstedges.assign(tedgevec.begin(), tedgevec.end());

    mstcost = 0.0;
    for (auto it = tedgevec.begin(); it != tedgevec.end(); it++)
    {
        //edge cost
        int edgeid = *it;
        double cost = 0.0;
        int res = m_graph.getEdgeDistance(edgeid, cost);
        mstcost += cost;
    }

    return Status::OK;
}
