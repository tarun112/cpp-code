/*
* -MinSpanTree implementation
* -Uses Prim's algorithm to find minimum spanning tree for a graph
*/

#include <map>
#include <tuple>
#include <cassert>
#include <set>
#include "minspantree.h"
#include "graph.h"
using namespace std;

//uses Prim's algorithm to finds minimum spanning tree 
//which is to start with an empty tree and keep building the tree by
// adding edges with least cost such that one vertex is already in tree 
// and another not in tree until the tree spans all nodes
//returns tree nodes and cost
int MinSpanTree::findTree(vector<int> & mstedges, double &mstcost)
{
    //get the graph edges
    vector<int> edgeidvec;
    m_graph.getEdges(edgeidvec);

    //sort the edges w.r.t edge costs
    list<int> edgeidlist(edgeidvec.begin(), edgeidvec.end());
    edgeidlist.sort(m_compare);

    vector<int> nodeidvec;
    m_graph.getNodes(nodeidvec);

    //sets required in Prim's algorithm. 
    //treeset contains nodes already in tree
    //ntreeset contains rest of the nodes in graph
    set<int> treeset;
    set<int> ntreeset(nodeidvec.begin(), nodeidvec.end());

    //pick one node to start
    auto it = ntreeset.begin();
    int nodeid0 = *it;
    ntreeset.erase(it);
    treeset.insert(nodeid0);

    //contains edges in tree
    vector<int> tedgevec;

    //processing nodes 
    while ( ! ntreeset.empty() )
    {
        auto listit = edgeidlist.begin(); 

        //finds and inserts edge with minimum distance such that 
        //one node is in tree and other outside tree
        while (true) 
        {
            int edgeid = *listit;
            int nodeid0, nodeid1;
            m_graph.getEdgeNodes(edgeid, nodeid0, nodeid1);

            //checking if nodeid0 is in tree & nodeid1 is not in tree
            {
                auto it = treeset.find(nodeid0);
                if( it != treeset.end() )
                {
                    auto it2 = ntreeset.find(nodeid1);
                    if (it2 != ntreeset.end() )
                    {
                        //insert the edge into tree edge list
                        tedgevec.push_back(edgeid);
                        //and remove it from the processing list
                        edgeidlist.erase(listit);

                        //insert the node into tree set
                        treeset.insert(nodeid1);
                        //and remove it from non-tree set
                        ntreeset.erase(it2);

                        //break if found required edge
                        break;
                    }
                }
            }
            
            //checking for reverse. i.e. if nodeid1 is in tree & nodeid0 is not in tree
            {
                auto it = treeset.find(nodeid1);
                if( it != treeset.end() )
                {
                    auto it2 = ntreeset.find(nodeid0);
                    if (it2 != ntreeset.end() )
                    {
                        tedgevec.push_back(edgeid);
                        edgeidlist.erase(listit);
                        treeset.insert(nodeid0);
                        ntreeset.erase(it2);
                        //break if found required edge
                        break;
                    }
                }
            }

            //break if no more edges in the list
            listit++;
            if(listit == edgeidlist.end())
                break;
        }
    }

    //return tree edges & total cost 
    mstcost = 0.0;
    for (auto it = tedgevec.begin(); it != tedgevec.end(); it++)
    {
        int edgeid = *it;
        mstedges.push_back(edgeid);

        //edge cost
        double cost = 0.0;
        int res = m_graph.getEdgeDistance(edgeid, cost);
        mstcost += cost;
    }

    return 0;
}

//compares edge distances for given edge ids
bool MinSpanTree::CompareEdgeDistance::operator()(int edgeid0, int edgeid1)
{
    double distance0 = 0.0;
    double distance1 = 0.0;
    int res = m_graph.getEdgeDistance(edgeid0, distance0); 
    assert( res == m_graph.e_Ok);

    res = m_graph.getEdgeDistance(edgeid1, distance1); 
    assert( res == m_graph.e_Ok);

    return (distance0 < distance1);
}

