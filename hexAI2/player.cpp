/*
*  -implements 'PlayerHuman', 'PlayerComputer' derived  classes .
*  -This implements the Player inheritance and management.  It iterfaces with HexGame  
*     to accept & make player specific moves.
*  -implements 'PlayerMgr' managing class.  Manages creation deletion of Player objects
*/
#include "hexgame.h"
#include "player.h"

//play for player human
int PlayerHuman::play(HexGame& hexgame) 
{
    return hexgame.play(getColor());
}

//play for player computer
int PlayerComputer::play(HexGame& hexgame) 
{
    return hexgame.playComputer(getColor());
}

//create blue player, it can be either human or computer
int PlayerMgr::createPlayerBlue( PlayerType type, const string& info) {
    if(PlayerType::HUMAN == type) 
        m_playerBlue = new PlayerHuman(PlayerColor::BLUE, info);
    else if(PlayerType::COMP == type) 
        m_playerBlue = new PlayerComputer(PlayerColor::BLUE, info);
    else
        assert(0);
    return 0;
}

//create red player, it can be either human or computer
int PlayerMgr::createPlayerRed( PlayerType type, const string& info) {
    if(PlayerType::HUMAN == type) 
        m_playerRed = new PlayerHuman(PlayerColor::RED, info);
    else if(PlayerType::COMP == type) 
        m_playerRed = new PlayerComputer(PlayerColor::RED, info);
    else
        assert(0);
    return 0;
}

