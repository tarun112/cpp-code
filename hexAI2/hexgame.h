/*
*  -hexgame.h
*  -get game size and type(players human or computer) and start games
*/
#ifndef _HEXGAME__H_
#define _HEXGAME__H_

#include <string>
#include "playerenums.h"
using namespace std;

class HexGame 
{
public:
    HexGame(){}

    ~HexGame(){}

    //get the hex board size. Since all subsequent objects are initialized 
    //  with size, it is to be entered here.
    int getHexSize(int& size);

    //prompt and take input for game type.(player human or computer)
    int getGamePlayers( pair<PlayerType, string>& blueplayer, 
                        pair<PlayerType, string>& redplayer);
  
    //play
    int play();

};

#endif //_HEXGAME__H_
