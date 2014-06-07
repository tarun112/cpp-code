/*
*  -implements hex game player interface using ascii characters on console.
*  -prompts for player input, checks and places moves.
*/
#ifndef _HEXGAME__H_
#define _HEXGAME__H_

#include "hex.h"

//class HexGame declaration
class HexGame
{
public:
    //constructor initialized with hex-size(side-length) and Hex object
    HexGame(int size):
        m_size(size), m_hex(size),
        m_moveNumber(0), m_previousMove("")
    {}

    //start hex game
    int play();

    //play for specified player
    int play(PlayerEnum player);

    //prompt for player input, check input
    int getMove(PlayerEnum player, pair<int,int>& coords);

    //check and place player input on hex-board
    int move(PlayerEnum player, pair<int,int> coords);

    //check if input is valid
    bool isMoveOnBoard(pair<int,int> position);

    //draw board
    int drawMove(PlayerEnum player);

    //initialize hex game
    int initialize();

private:
    int     m_size;  //hex-size which is number of positions on one side
    Hex     m_hex;  //Hex object used for processing moves
    bool    m_twoPlayers;  //if both players human

    int     m_moveNumber;  //counter for moves

    //stores player info 
    string m_playerBlue;  
    string m_playerRed;

    string m_previousMove; 

};

#endif  //_HEXGAME__H_
