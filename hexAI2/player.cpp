/*
*  -implements 'PlayerHuman', 'PlayerComputer' derived  classes .
*  -This implements the Player inheritance and management.  It iterfaces with HexUI  
*     to accept & make player specific moves.
*  -implements 'PlayerMgr' managing class.  Manages creation deletion of Player objects
*/
#include "hexui.h"
#include "player.h"

//play for player human
int PlayerHuman::play(HexUI& hexui) 
{
    return hexui.play(getColor());
}

//----------------------------------------------------------

//play for player computer
int PlayerComputer::play(HexUI& hexui) 
{
    return hexui.playComputer(getColor());
}

//----------------------------------------------------------

PlayerMgr::PlayerMgr(const pair<PlayerType,string>& blueplayer, 
              const pair<PlayerType,string>& redplayer)
                    : m_playerBlue(nullptr),m_playerRed(nullptr)  {
    int res = createPlayerBlue(blueplayer);
    res = createPlayerRed(redplayer);
}

//create blue player, it can be either human or computer
int PlayerMgr::createPlayerBlue(const pair<PlayerType,string>& blueplayer ) {
    if(nullptr != m_playerBlue) {
        delete m_playerBlue;
        m_playerBlue = nullptr;
    }

    PlayerType type = blueplayer.first;
    string info = blueplayer.second;

    if(PlayerType::HUMAN == type) 
        m_playerBlue = new PlayerHuman(PlayerColor::BLUE, info);
    else if(PlayerType::COMP == type) 
        m_playerBlue = new PlayerComputer(PlayerColor::BLUE, info);
    else
        assert(0);
    return 0;
}

//create red player, it can be either human or computer
int PlayerMgr::createPlayerRed( const pair<PlayerType,string>& redplayer ) {
    if(nullptr != m_playerRed) {
        delete m_playerRed;
        m_playerRed = nullptr;
    }

    PlayerType type = redplayer.first;
    string info = redplayer.second;

    if(PlayerType::HUMAN == type) 
        m_playerRed = new PlayerHuman(PlayerColor::RED, info);
    else if(PlayerType::COMP == type) 
        m_playerRed = new PlayerComputer(PlayerColor::RED, info);
    else
        assert(0);
    return 0;
}

