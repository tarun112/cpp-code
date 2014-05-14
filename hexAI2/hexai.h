/*
*  -HexAI declaration
*  -methods to compute moves and to check winner
*  -uses UnionFind to determine winner by checking connectivity between virtual nodes
*  -uses Monte Carlo simulation to compute moves with best winning chances
*/
#ifndef  _HEXAI__H_
#define  _HEXAI__H_

#include <cassert>
//#include <set>
#include <unordered_set>
#include <map>

class Graph;
class Hex;

//class HexAI declaration
class HexAI 
{
public:
    //constructor initiaized with references of graph and hex
    HexAI(Graph &graph, Hex &hex):
        m_graph(graph), m_hex(hex)
    {
        initialize();
    }

    //initialize the one-time constructed data 
    int initialize();

    //for player compute move with best winning chance and
    //  return it
    int computeMove(PlayerColor player, int& nodeid);

    //get other player for current player
    PlayerColor getOtherPlayer(PlayerColor player)
    {
        if (PlayerColor::BLUE == player)
            return PlayerColor::RED;
        else if (PlayerColor::RED == player)
            return PlayerColor::BLUE;
        else
        {
            assert(false);
            return PlayerColor::BLANK;
        }
    }

    //check if either of blue or red is winner
    int checkWinner(const unordered_set<int> &bluenodes, 
        const unordered_set<int> &rednodes, PlayerColor &winner);

    //check for and return winner
    int checkWinner(PlayerColor &winner);

    //compute move with most winning chance
    int computeMove(const unordered_set<int>& bluenodes, 
        const unordered_set<int> rednodes, 
        const vector<int> blanknodes, int curmoveid);

private:
    Graph  &m_graph;
    Hex   &m_hex;

    vector<int> m_graphEdges;   //graph edges remain unchanged

    //map of move(graph node) and its winning chance
    map<int, double> m_moveChanceMap;
};


#endif  //_HEXAI__H_
