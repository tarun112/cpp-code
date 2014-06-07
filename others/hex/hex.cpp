#include <iostream>
#include <iomanip>
#include <cassert>
#include "graph.h"
#include "hex.h"
#include "minspantree.h"

using namespace std;

//showing a hex-position with its connections to neighbours
//    .  .
//     \/
//  . _ . _ .    hex-position with neighbours
//     /\
//    .  .
//
//         i
//      0 --> N-1              
//     0            hex-position (i,j)
//   j |
//    N-1
//

//initializes graph and adds edges to create the hex board.
int Hex::initialize()
{
    double distance = 1.0;  //default cost of hex-edges

    //for each middle node add six edges
    for (int i=1; i<m_size-1; i++)
    {
        for (int j=1; j<m_size-1; j++)
        {
            m_graph.insertEdge(getNodeid(i, j), getNodeid(i, j-1), distance);
            m_graph.insertEdge(getNodeid(i, j), getNodeid(i+1, j-1), distance);
            m_graph.insertEdge(getNodeid(i, j), getNodeid(i-1, j), distance);
            m_graph.insertEdge(getNodeid(i, j), getNodeid(i+1, j), distance);
            m_graph.insertEdge(getNodeid(i, j), getNodeid(i-1, j+1), distance);
            m_graph.insertEdge(getNodeid(i, j), getNodeid(i, j+1), distance);
        }
    }

    //for side nodes add four edges
    for (int i=1; i<m_size-1; i++)
    {
        int j = 0;
        m_graph.insertEdge(getNodeid(i,j), getNodeid(i-1, j), distance);
        m_graph.insertEdge(getNodeid(i,j), getNodeid(i+1, j), distance);
        m_graph.insertEdge(getNodeid(i,j), getNodeid(i-1, j+1), distance);
        m_graph.insertEdge(getNodeid(i,j), getNodeid(i, j+1), distance);

        j = m_size-1;
        m_graph.insertEdge(getNodeid(i,j), getNodeid(i, j-1), distance);
        m_graph.insertEdge(getNodeid(i,j), getNodeid(i+1, j-1), distance);
        m_graph.insertEdge(getNodeid(i,j), getNodeid(i-1, j), distance);
        m_graph.insertEdge(getNodeid(i,j), getNodeid(i+1, j), distance);
    }

    //continue adding edges to side nodes
    for (int j=1; j<m_size-1; j++)
    {
        int i = 0;
        m_graph.insertEdge(getNodeid(i,j), getNodeid(i, j-1), distance);
        m_graph.insertEdge(getNodeid(i,j), getNodeid(i+1, j-1), distance);
        m_graph.insertEdge(getNodeid(i,j), getNodeid(i+1, j), distance);
        m_graph.insertEdge(getNodeid(i,j), getNodeid(i, j+1), distance);

        i = m_size-1;
        m_graph.insertEdge(getNodeid(i,j), getNodeid(i, j-1), distance);
        m_graph.insertEdge(getNodeid(i,j), getNodeid(i-1, j), distance);
        m_graph.insertEdge(getNodeid(i,j), getNodeid(i-1, j+1), distance);
        m_graph.insertEdge(getNodeid(i,j), getNodeid(i, j+1), distance);
    }

    //for four corner nodes add two or three edges
    int i=0;
    int j=0;
    m_graph.insertEdge(getNodeid(i,j),getNodeid(i+1, j), distance);
    m_graph.insertEdge(getNodeid(i,j),getNodeid(i, j+1), distance);

    i=m_size-1;
    j=0;
    m_graph.insertEdge(getNodeid(i,j),getNodeid(i-1, j), distance);
    m_graph.insertEdge(getNodeid(i,j),getNodeid(i-1, j+1), distance);
    m_graph.insertEdge(getNodeid(i,j),getNodeid(i, j+1), distance);

    i=0;
    j=m_size-1;
    m_graph.insertEdge(getNodeid(i,j),getNodeid(i, j-1), distance);
    m_graph.insertEdge(getNodeid(i,j),getNodeid(i+1, j-1), distance);
    m_graph.insertEdge(getNodeid(i,j),getNodeid(i+1, j), distance);

    i=m_size-1;
    j=m_size-1;
    m_graph.insertEdge(getNodeid(i,j),getNodeid(i, j-1), distance);
    m_graph.insertEdge(getNodeid(i,j),getNodeid(i-1, j), distance);

    //virtual nodes are not on hex-board.  they are used for checking
    //  connectivity from one side to opposite side 
    //virtual nodes ids
    m_blueNodeidStart = m_size * m_size;
    m_blueNodeidEnd = m_size * m_size + 1;
    m_redNodeidStart = m_size * m_size + 2;
    m_redNodeidEnd = m_size * m_size + 3;

    //insert edges for virtual nodes.  each virtual nodes connects to all
    //  one side nodes including corners.
    for (int i=0; i<m_size; i++)
    {
        int j = 0;
        m_graph.insertEdge(m_redNodeidStart,getNodeid(i,j), distance);
        m_graph.insertEdge(getNodeid(i,j), m_redNodeidStart, distance);

        j = m_size-1;
        m_graph.insertEdge(m_redNodeidEnd, getNodeid(i,j), distance);
        m_graph.insertEdge(getNodeid(i,j), m_redNodeidEnd, distance);
    }

    for (int j=0; j<m_size; j++)
    {
        int i = 0;
        m_graph.insertEdge(m_blueNodeidStart, getNodeid(i,j), distance);
        m_graph.insertEdge(getNodeid(i,j), m_blueNodeidStart, distance);

        i = m_size-1;
        m_graph.insertEdge(m_blueNodeidEnd, getNodeid(i,j), distance);
        m_graph.insertEdge(getNodeid(i,j), m_blueNodeidEnd, distance);
    }

    //initialize player(blank) associated with node
    for (int i=0; i<m_size*m_size; i++)
        m_nodeStatusMap[i] = PlayerEnum::PLAYER_BLANK;

    //node status for four virtual node
    m_nodeStatusMap[m_redNodeidStart] = PlayerEnum::PLAYER_RED;
    m_nodeStatusMap[m_redNodeidEnd] = PlayerEnum::PLAYER_RED;
    m_nodeStatusMap[m_blueNodeidStart] = PlayerEnum::PLAYER_BLUE;
    m_nodeStatusMap[m_blueNodeidEnd] = PlayerEnum::PLAYER_BLUE;

    return StatEnum::OK;
}

//draw hex-board on console using ascii characters.
//uses '.' for blank position, 'X' for first player 
//  and 'O' for second player.
int Hex::drawBoard()
{
    int ichr = 0;
    int jchr = 0;
    cout << setw(6) << endl;
    for (int i=0; i<m_size; i++)
        cout << static_cast<char>(65+i) << "   ";

    for (int j=0; j<m_size; j++)
    {
        cout << setw(j*2+2) << endl;
        cout << j+1 << "   ";
        for (int i=0; i<m_size; i++)
        {
            PlayerEnum player;
            getPositionStatus(i, j, player);
            if (PlayerEnum::PLAYER_BLUE == player) 
                cout << "X" << "   ";
            else if (PlayerEnum::PLAYER_RED == player)
                cout << "O" << "   ";
            else
                cout << "." << "   ";
        }
        cout << j+1 ;
    }
    cout << setw(2*m_size+4) << endl;
    for (int i=0; i<m_size; i++)
        cout << static_cast<char>(65+i) << "   ";

    return StatEnum::OK;
}

//get graph-nodeid for hes-position
int Hex::getNodeid(int ipos, int jpos)
{
    if (ipos >= m_size || jpos >= m_size)
        return StatEnum::INVALID_POS;

    return (jpos * m_size + ipos);
}

//get hes-position for graph-nodeid 
int Hex::getPosition(int nodeid, int& ipos, int& jpos)
{
    if (nodeid >= m_size * m_size)
        return StatEnum::INVALID_POS;

    ipos = nodeid % m_size;
    jpos = (nodeid - ipos) / m_size;
    return StatEnum::OK;
}

//get player associated with hex-position
int Hex::getPositionStatus(int ipos, int jpos, PlayerEnum& player)
{
    player = m_nodeStatusMap[getNodeid(ipos, jpos)];
    return StatEnum::OK;
}

//get player associated with graph-node
int Hex::getNodeStatus(int nodeid, PlayerEnum& player)
{
    player = m_nodeStatusMap[nodeid];
    return StatEnum::OK;
}

//set player with hex-position
int Hex::setPositionStatus(int ipos, int jpos, PlayerEnum player)
{
    int nodeid = getNodeid(ipos, jpos);

    int res = setNodeStatus(nodeid, player);
    return res;
}

//set player with graph-node
int Hex::setNodeStatus(int nodeid, PlayerEnum player)
{
    //check if pos already occupied
    if (m_nodeStatusMap[nodeid] != PlayerEnum::PLAYER_BLANK )
        return StatEnum::POS_OCCUPIED;

    m_nodeStatusMap[nodeid] = player;
    return StatEnum::OK;
}

//checks if player wins by checking connectivity between
//  opposite virtual nodes.
//uses minimum spanning tree over color nodes and edges.
bool Hex::doPlayerWin(PlayerEnum player)
{
    int startnodeid;
    int endnodeid;
    if (PlayerEnum::PLAYER_BLUE == player)
    {
        startnodeid = m_blueNodeidStart;
        endnodeid = m_blueNodeidEnd;
    }
    else if (PlayerEnum::PLAYER_RED == player)
    {
        startnodeid = m_redNodeidStart;
        endnodeid = m_redNodeidEnd;
    }
    else
        assert(false);

    MinSpanTree mst(m_graph, *this);

    //finds mst over color edges using one of the virtual nodes as start-node
    //mst color is color of start-node.
    vector<int> mstedges;
    double mstcost;
    int res = mst.findTree(startnodeid, mstedges, mstcost);

    //check if the endnode is in the mst.  if it is in tree then
    //  return current player(color) as winner
    for (int edgeid : mstedges )    //using for-each idiom
    {
        int nodeid0, nodeid1;
        m_graph.getEdgeNodes(edgeid, nodeid0, nodeid1);

        if (nodeid0 == endnodeid || nodeid1 == endnodeid )
            return true;
    }

    return false;
}

//check if player wins and return status as winner
int Hex::checkForWinner(PlayerEnum& winner)
{
    if ( doPlayerWin(PlayerEnum::PLAYER_BLUE) )
    {
        winner = PlayerEnum::PLAYER_BLUE;
        return StatEnum::WINNER;
    }
    
    if ( doPlayerWin(PlayerEnum::PLAYER_RED) )
    {
        winner = PlayerEnum::PLAYER_RED;
        return StatEnum::WINNER;
    }

    return StatEnum::OK;
}

//get edges in graph with both nodes associated with player(color)
int Hex::getColorEdges(PlayerEnum player, vector<int>& edgeidvec)
{
    vector<int> ledgeidvec;
    m_graph.getEdges(ledgeidvec);

    for (int edgeid : ledgeidvec)  //using for-each idiom
    {
        int nodeid0, nodeid1;
        m_graph.getEdgeNodes(edgeid, nodeid0, nodeid1);

        PlayerEnum stat0, stat1;
        getNodeStatus(nodeid0, stat0);
        getNodeStatus(nodeid1, stat1);
        if (player == stat0 && player == stat1)
            edgeidvec.push_back(edgeid);
    }
    return StatEnum::OK;
}

//gt nodes in graph associated with player(color) 
int Hex::getColorNodes(PlayerEnum player, vector<int>& nodeidvec)
{
    vector<int> lnodeidvec;
    m_graph.getNodes(lnodeidvec);

    for (int nodeid : lnodeidvec)
    {
        PlayerEnum stat;
        getNodeStatus(nodeid, stat);

        if (player == stat)
            nodeidvec.push_back(nodeid);
    }
    return StatEnum::OK;
}
