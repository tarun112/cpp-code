/*
*  -Hex implementation
*  -manages board and moves and keep track of current configuration.
*  -uses Graph(edge-list adjacency) to create board configuration
*  -uses HexAI to compute moves and check for winner if any
*/
#include <iostream>
#include <iomanip>
#include <cassert>
#include "graph.h"
#include "hex.h"
#include "hexai.h"

using namespace std;

//showing a hex-position with its connections to neighbours
//    .  .
//     \/
//  . _ . _ .    hex-position with neighbours  
//     /\
//    .  .
//
//     . .
//      \|
//     ._._.     with skewed board  
//       |\
//       . .
//
//         i
//      0 --> N-1              
//     0            hex-position direction(i,j)
//   j |
//    N-1
//
//             n(i-1,j)  n(i-1,j+1)
//  n(i,j-1)  cur(i,j)   n(i,j+1)    neighbors of position (i,j) 
// n(i+1,j-1)  n(i+1,j)
//

//initializes graph and adds edges to create the hex board.
//it is easier and less confusing to add individual neighbors than to do in loops
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
        m_nodeStatusMap[i] = PlayerColor::BLANK;

    //node status for four virtual node9xc6
    m_nodeStatusMap[m_redNodeidStart] = PlayerColor::RED;
    m_nodeStatusMap[m_redNodeidEnd] = PlayerColor::RED;
    m_nodeStatusMap[m_blueNodeidStart] = PlayerColor::BLUE;
    m_nodeStatusMap[m_blueNodeidEnd] = PlayerColor::BLUE;

    return StatEnum::OK;
}

//draw hex-board on console using ascii characters.
//uses '.' for blank position, 'X' for first player 
//  and 'O' for second player.
int Hex::drawBoard()
{
    cout << setw(6) << endl;
    for (int i=0; i<m_size; i++)
        cout << static_cast<char>(65+i) << "   ";

    for (int j=0; j<m_size; j++)
    {
        cout << setw(j*2+2) << endl;
        cout << j+1 << "   ";
        for (int i=0; i<m_size; i++)
        {
            PlayerColor player;
            getPositionStatus(i, j, player);
            if (PlayerColor::BLUE == player) 
                cout << "X" << "   ";
            else if (PlayerColor::RED == player)
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
int Hex::getPositionStatus(int ipos, int jpos, PlayerColor& player)
{
    player = m_nodeStatusMap[getNodeid(ipos, jpos)];
    return StatEnum::OK;
}

//get player associated with graph-node
int Hex::getNodeStatus(int nodeid, PlayerColor& player)
{
    player = m_nodeStatusMap[nodeid];
    return StatEnum::OK;
}

//set player with hex-position
int Hex::setPositionStatus(int ipos, int jpos, PlayerColor player)
{
    int nodeid = getNodeid(ipos, jpos);

    int res = setNodeStatus(nodeid, player);
    return res;
}

//set player with graph-node
int Hex::setNodeStatus(int nodeid, PlayerColor player)
{
    //check if pos already occupied
    if (m_nodeStatusMap[nodeid] != PlayerColor::BLANK )
        return StatEnum::POS_OCCUPIED;

    m_nodeStatusMap[nodeid] = player;
    return StatEnum::OK;
}

//check if player wins and return status as winner
int Hex::checkForWinner(PlayerColor& winner)
{
    //create hexai object to check for winner if any
    HexAI hexai(m_graph, *this);

    //check and get winner from hexai if any
    winner = PlayerColor::BLANK;
    hexai.checkWinner(winner);

    if (winner != PlayerColor::BLANK )
        return StatEnum::WINNER;

    return StatEnum::OK;
}

//get nodes in graph associated with player(color) 
int Hex::getColorNodes(PlayerColor player, vector<int>& nodeidvec)
{
    vector<int> lnodeidvec;
    m_graph.getNodes(lnodeidvec);

    for (int nodeid : lnodeidvec)
    {
        PlayerColor stat;
        getNodeStatus(nodeid, stat);

        if (player == stat)
            nodeidvec.push_back(nodeid);
    }
    return StatEnum::OK;
}

//compute and return move with best winning chance for current board
int Hex::getComputerMove(PlayerColor player, pair<int,int>& position)
{
    //create hexai object for move computation
    HexAI hexai(m_graph, *this);

    //compute and get move from hexai
    int movenodeid;
    int res = hexai.computeMove(player, movenodeid);
    if (res != StatEnum::OK )
        return res;

    int ipos, jpos;
    getPosition(movenodeid, ipos, jpos);
    position.first = ipos;
    position.second = jpos;

    return StatEnum::OK;
}

//return ids for virtual nodes
int Hex::getVirtualNodes(pair<int,int>& bluenodes, pair<int,int>& rednodes)
{
    bluenodes.first = m_blueNodeidStart;
    bluenodes.second = m_blueNodeidEnd;

    rednodes.first = m_redNodeidStart;
    rednodes.second = m_redNodeidEnd;

    return StatEnum::OK;
}
