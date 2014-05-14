/*
*  -HexAI class implementation
*  -uses Monte Carlo simulation to computes move with most winning chance.
*  -uses UnionFind to determine winner by checking connectivity between virtual nodes
*/
#include <algorithm>
#include <random>
#include <chrono>
#include <list>
#include <vector>
#include <iostream>
#include <set>
#include <unordered_set>
#include "hex.h"
#include "graph.h"
#include "hexai.h"
#include "unionfind.h"

//number of repitions for Monte Carlo simulation
const int NUM_REPS = 1000;

//One time retrival of edges from graph as edges are invariant in game.
//since they are bidirectional remove one of them.
int HexAI::initialize()
{
    vector<int> tmpvec;
    m_graph.getEdges(tmpvec);
    list<int> tmplst(tmpvec.begin(), tmpvec.end());  //list is efficent in removing elements
    
    //find and remove one of the bidirectional edge
    for (int edgeid : tmplst)
    {
        int node0,node1;
        m_graph.getEdgeNodes(edgeid, node0, node1);

        //other edge will be with nodes reversed
        int edgeid2 = m_graph.makeEdgeid(node1, node0);
        tmplst.remove(edgeid2);  //remove the other
    }

    m_graphEdges.assign(tmplst.begin(), tmplst.end());

    return 0;
}

//check winner if any for current board
int HexAI::checkWinner(PlayerColor &winner)
{
    //get nodes for both blue and red
    vector<int> tmpvec;
    m_hex.getColorNodes(PlayerColor::RED, tmpvec);
    unordered_set<int> rednodes(tmpvec.begin(), tmpvec.end());
    tmpvec.clear();

    m_hex.getColorNodes(PlayerColor::BLUE, tmpvec);
    unordered_set<int> bluenodes(tmpvec.begin(), tmpvec.end());;
    tmpvec.clear();

    //check if blue or red or none is winner for current board
    return checkWinner(bluenodes, rednodes, winner);
}

//check if blue or red is winner given sets of blue and red nodes
int HexAI::checkWinner(const unordered_set<int> &bluenodes, 
    const unordered_set<int> &rednodes, PlayerColor &winner)
{
    //unionfind is specific and efficient than MinSpanTree and Dijkstra
    UnionFind blueuf;  //unionfind object for each blue and red nodes
    UnionFind reduf;

    //loop over graph edges checking if they connect same-color nodes
    //and updating unionfind objects 
    for ( int edgeid : m_graphEdges )
    {
        int nodeid0, nodeid1;
        m_graph.getEdgeNodes(edgeid, nodeid0, nodeid1);

        //check if both nodes are in blueset
        if ( bluenodes.end() != bluenodes.find(nodeid0) && 
             bluenodes.end() != bluenodes.find(nodeid1) )
        {
             blueuf.joinNodes(nodeid0, nodeid1);
             continue;  
        }
        
        //check if both nodes are in red set
        if ( rednodes.end() != rednodes.find(nodeid0) && 
             rednodes.end() != rednodes.find(nodeid1) )
        {
             reduf.joinNodes(nodeid0, nodeid1);
             continue;
        }
    }

    //get virtual nodes to check connectivity between them
    pair<int,int> vrednodes;
    pair<int,int> vbluenodes;
    m_hex.getVirtualNodes(vbluenodes, vrednodes);

    //check if same-color virtual nodes are connected to find winner
    if (blueuf.connected(vbluenodes.first, vbluenodes.second) )
        winner = PlayerColor::BLUE;
    else if (reduf.connected(vrednodes.first, vrednodes.second) )
        winner = PlayerColor::RED;
    else
        winner = PlayerColor::BLANK;

    return StatEnum::OK;
}

//for player computes move with most winning chance usign Monte Carlo simulation
int HexAI::computeMove(PlayerColor player, int& movenodeid)
{
    chrono::steady_clock::time_point timestart = chrono::steady_clock::now();

    //get current color nodes as starting point 
    //container 'unordered_set' is efficient for finding and erasing elements
    vector<int> tmpvec;
    m_hex.getColorNodes(PlayerColor::RED, tmpvec);
    unordered_set<int> orednodes(tmpvec.begin(),tmpvec.end());
    tmpvec.clear();

    m_hex.getColorNodes(PlayerColor::BLUE, tmpvec);
    unordered_set<int> obluenodes(tmpvec.begin(),tmpvec.end());;
    tmpvec.clear();

    //get blank nodes for processing
    vector<int> bnodeidvec;
    m_hex.getColorNodes(PlayerColor::BLANK, bnodeidvec);

    //random shuffle blank nodes 
    unsigned  seed = chrono::system_clock::now().time_since_epoch().count();	
    default_random_engine generator(seed);
    uniform_int_distribution<int> distribution(0, bnodeidvec.size()-1);
    shuffle(bnodeidvec.begin(), bnodeidvec.end(), generator);

    //processing loop over available moves(blank nodes)
    for( int movenodeid : bnodeidvec )
    {
        //copy the current node state and try every move from all available moves 
        unordered_set<int> rednodes(orednodes);
        unordered_set<int> bluenodes(obluenodes);
        if (PlayerColor::RED == player)
            rednodes.insert(movenodeid);  //insert the move 
        else if (PlayerColor::BLUE == player)
            bluenodes.insert(movenodeid);
        else
            assert(false);

        //monte carlo simulation
        //fill the board with random moves and test for winner. 
        //repeat ~1000 times 
        int wincount = 0;
        for (int i=0; i<NUM_REPS; ++i)
        {
            //for each repetition copy nodes status 
            unordered_set<int> rnodes(rednodes);
            unordered_set<int> bnodes(bluenodes);

            //random shuffle blank nodes for next move
            vector<int> blanknodes(bnodeidvec.begin(), bnodeidvec.end());
            unsigned seed = chrono::system_clock::now().time_since_epoch().count();	
            default_random_engine generator(seed);
            uniform_int_distribution<int> distribution(0, blanknodes.size()-1);
            shuffle(blanknodes.begin(), blanknodes.end(), generator);

            //remove the move already made from blank nodes
            blanknodes.erase(find(blanknodes.begin(), blanknodes.end(), movenodeid));  

            //loop over available blank nodes to fill the board
            //  for both players
            PlayerColor cplayer = player;
            for (int randnodeid : blanknodes )
            {
                cplayer = getOtherPlayer(cplayer);
                if (PlayerColor::RED == cplayer)
                    rnodes.insert(randnodeid);
                else if (PlayerColor::BLUE == cplayer)
                    bnodes.insert(randnodeid);
                else
                    assert(false);
            }
        
            //check for winner for each color nodes
            PlayerColor winner = PlayerColor::BLANK;
            checkWinner(bnodes, rnodes, winner);
            assert(winner != PlayerColor::BLANK); //in full board there has to be a winner

            //if computer is winner increment wincount which is used to compute
            //  winning chance for current move
            if (winner == player)
                wincount++;
        }

        //compute and keep winning chance for each possible move
        double winchance = static_cast<double>(wincount)/NUM_REPS;
        m_moveChanceMap[movenodeid] = winchance;
    }

    //get the move with maximum winning chance
    auto it = max_element(m_moveChanceMap.begin(), m_moveChanceMap.end(),
                [](const pair<int, double>& p1, const pair<int, double>& p2) {
                    return p1.second < p2.second; });
    
    //return move with best winning chance
    movenodeid = it->first;

    chrono::steady_clock::time_point timeend = chrono::steady_clock::now();
    cout << endl << "Time taken : " << 
        chrono::duration_cast<chrono::milliseconds>(timeend-timestart).count() << " milliseconds" << endl;

    return StatEnum::OK;
}

