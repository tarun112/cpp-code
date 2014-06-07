/*
*  -declares Hex class.
*  -This implements the core of hex game.  It iterfaces with Graph to 
*     store the hex board and to process game.
*/
#ifndef _HEX__H_
#define _HEX__H_

#include "graph.h"

//enum for two players in game
enum class PlayerEnum
{
    PLAYER_BLANK = 0,
    PLAYER_BLUE = 1,
    PLAYER_RED = 2
};

//enum for status passed by methods.  
//in methods they are passed as int
enum StatEnum
{
    NOK = -1,
    OK = 0,
    INVALID_POS = 1,
    POS_OCCUPIED = 2,
    WINNER = 3,
    INTERRUPT = 4
};

//class Hex declaration
//hex-board uses positions (i,j) to define a hexagon on board.
//these hex-positions are stored in graph as nodes along with
//  four virtual nodes used to check connectivity from one side
//  to opposite side.
class Hex
{
public:
    //constructor initialized with board-size and graph object.
    Hex(int size):
        m_size(size),
        m_graph(size*size+4)  //four extra virtual nodes 
    {};

    //initializes the graph and adds edges to graph-nodes creating hex-board
    int initialize();

    //draws board using ascii characters on console
    int drawBoard();

    //gets graph-nodeid from hex-position
    int getNodeid(int ipos, int jpos);

    //gets hex-position from graph-nodeid
    int getPosition(int nodeid, int& ipos, int& jpos);

    //sets player on hex-position
    int setPositionStatus(int i, int j, PlayerEnum player);

    //gets player associated with hex-position
    int getPositionStatus(int i, int j, PlayerEnum& player);

    //sets player on graph-nodeid
    int setNodeStatus(int nodeid, PlayerEnum player);

    //gets player associated with graph-nodeid
    int getNodeStatus(int nodeid, PlayerEnum& player);

    //checks west-east or north-south connectivity for respective player
    //uses minimum spanning tree
    int checkForWinner(PlayerEnum& winner);

    //here color is player associated with node.
    //used while processing for mininimum spanning tree to get
    //  connectivity for specific player(color)
    int getColorNodes(PlayerEnum player, vector<int>& nodeidvec);

    //color edge is which has both nodes of same color
    int getColorEdges(PlayerEnum player, vector<int>& edgeidvec);

private:
    //checks if player wins by checking connectivity
    bool doPlayerWin(PlayerEnum player);

private:
    int m_size; //side length. so total board size = m_size * m_size  
    Graph m_graph; //graph will have m_size*m_size + 4(virtual) nodes.

    map<int,PlayerEnum> m_nodeStatusMap; //board nodes and their status map

    //following are four virtual nodeids used for checking winner
    int m_blueNodeidStart;
    int m_blueNodeidEnd;
    int m_redNodeidStart;
    int m_redNodeidEnd;
    
};

#endif  //_HEX__H_