/*
*  -implements hex game player interface using ascii characters on console.
*  -prompts for player input.  checks and places moves.
*  -uses Hex to manage board and moves
*/
#ifndef _HEXGAME__H_
#define _HEXGAME__H_

#include "hex.h"
class Player;

//class HexGame declaration
class HexGame
{
public:
    //constructor initialized with hex-size(side-length) and Hex object
    HexGame(int size, pair<Player&,Player&> gameplayers):
        m_size(size), m_hex(size), // m_gamePlayers(gameplayers),
        m_moveNumber(0), m_previousMove(""),
        m_playerBlue(gameplayers.first),
        m_playerRed(gameplayers.second)
    {}

    inline Player* getOtherPlayer(Player* player);

    //start hex game
    int play();

    //play for specified player
    int play(PlayerColor player);

    //prompt for player input, check input
    int getMove(PlayerColor player, pair<int,int>& coords);

    //check and place player input on hex-board
    int makeMove(PlayerColor player, pair<int,int> coords);

    //check if input is valid
    bool isMoveOnBoard(pair<int,int> position);

    int playComputer(PlayerColor player);

    //draw board
    int drawMove(PlayerColor player);

    //initialize hex game
    int initialize();

    //for given position return printable move
    string getMoveFromPosition(pair<int,int> position);

private:
    int   m_size;  //hex-size which is number of positions on one side
    Hex   m_hex;  //Hex object used for processing moves
//    GamePlayers   m_gamePlayers;  //players human and/or computer

    //stores player info 
    Player&   m_playerBlue;
    Player&   m_playerRed;

    int   m_moveNumber;  //counter for moves used to print current move info
    string  m_previousMove;  //previous move info used for printing
};

#endif  //_HEXGAME__H_
