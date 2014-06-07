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
#include "hex.h"

using namespace std;

//uses Prim's algorithm to finds minimum spanning tree 
//which is to start with an empty tree and keep building the tree by
//  adding edges with least cost such that one vertex is already in tree 
//  and another not in tree until the tree spans all nodes
//returns tree nodes and cost
int MinSpanTree::findTree(int startnodeid, vector<int>& edgeidvec,
    vector<int>& nodeidvec, vector<int> & mstedges, double &mstcost)
{
    list<int> edgeidlist(edgeidvec.begin(), edgeidvec.end());
    edgeidlist.sort(m_compare);  //sort the edges w.r.t edge costs

    //sets required in Prim's algorithm. 
    //treeset contains nodes already in tree
    set<int> treeset;  
    //ntreeset contains rest of the nodes in graph
    set<int> ntreeset(nodeidvec.begin(), nodeidvec.end());

    //move start node from ntreeset to treeset
    ntreeset.erase(startnodeid);
    treeset.insert(startnodeid);
 
    vector<int> tedgevec;  //stores edges in tree
    bool stopflag = false;  //set in inner loop to terminate outer loop

    //processing nodes till all nodes are part of tree or process is stoped
    while ( ! ntreeset.empty() && ! stopflag)
    {
        auto listit = edgeidlist.begin(); 
        if (edgeidlist.end() == listit )
            break;  //break if no edges to process

        //finds and inserts edge with minimum distance such that 
        //one node is in tree and other outside tree
        while ( true ) 
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
                        //move edge from processing to tree edge list
                        tedgevec.push_back(edgeid);
                        edgeidlist.erase(listit);

                        //move node from nin-tree to tree set
                        treeset.insert(nodeid1);
                        ntreeset.erase(it2);

                        //break if required edge is found
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
                        break;
                    }
                }
            }

            listit++;

            //break processing if reached end of list and conditions not met. 
            if(edgeidlist.end() == listit)
            {
                stopflag = true;
                break;
            }
        }
    }

    //return tree edges & total cost 
    mstedges.clear();
    mstedges.assign(tedgevec.begin(), tedgevec.end());
    
    mstcost = 0.0;
    for (auto it = tedgevec.begin(); it != tedgevec.end(); ++it)
    {
        //edge cost
        double cost = 0.0;
        int res = m_graph.getEdgeDistance(*it, cost);
        mstcost += cost;
    }

    return StatEnum::OK;
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

//mst over all edges and nodes in graph
int MinSpanTree::findTree(vector<int> & mstedges, double &mstcost)
{
    //get the graph edges
    vector<int> edgeidvec;
    m_graph.getEdges(edgeidvec);

    vector<int> nodeidvec;
    m_graph.getNodes(nodeidvec);

    int startnodeid = nodeidvec[0];

    int res = findTree(startnodeid, edgeidvec, nodeidvec, mstedges, mstcost);
    return res;
}

//use start nodeid for tree start node. start nodeid status is 
//  used as the color filter for the graph edges and nodes
int MinSpanTree::findTree(int startnodeid, vector<int> & mstedges, double &mstcost)
{
    //get the graph edges
    PlayerEnum player;
    m_hex.getNodeStatus(startnodeid, player);

    vector<int> edgeidvec;
    m_hex.getColorEdges(player, edgeidvec);

    vector<int> nodeidvec;
    m_hex.getColorNodes(player, nodeidvec);

    int res = findTree(startnodeid, edgeidvec, nodeidvec, mstedges, mstcost);
    return res;
}

