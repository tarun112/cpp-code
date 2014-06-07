/*
*  -declaration and implementation of class UnionFind
*  -it identifies connected set with a root. two nodes 
*     are conneced if they have the same root. 
*/
#ifndef _UNIONFIND__H_
#define _UNIONFIND__H_

//#include <map>
#include <map>
using namespace std;

//to check for winner using unionfind (disjoint sets) instead of 
//  costly MinSpanTree or Djikstra
class UnionFind
{
public:
    //join sets containing two nodes
    int joinNodes(int id0, int id1)
    {
        int root0 = getRoot(id0);
        int root1 = getRoot(id1);
        if (root0 != root1 )
            m_rootmap[root0] = root1;

        return 0;
    }

    //get root of set containing given node
    int getRoot(int id)
    {
        auto it = m_rootmap.find(id); 
        while (it != m_rootmap.end() )
        {
            id = it->second;
            it = m_rootmap.find(id);
        }
        return id;
    }


    //check if two nodes are connected by checking they have
    //  same root.
    bool connected(int id0, int id1)
    {
        if (getRoot(id0) == getRoot(id1) )
            return true;
        return false;
    }

private:
    map<int,int> m_rootmap;  //id, root(id) map

};

#endif  //_UNIONFIND__H_
