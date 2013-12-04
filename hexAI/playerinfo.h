/*
*  -PlayerInfo decration
*  -various classes and enums for player and game info.
*/
#ifndef  _PLAYERINFO__H_
#define  _PLAYERINFO__H_

#include <string>
using namespace std;

//enum for game type (players human and/or computer)
enum class GamePlayers
{
    HUMAN_VS_HUMAN = 1,
    HUMAN_VS_COMP = 2,
    COMP_VS_COMP = 3
};

//enum for player type (player human or computer)
enum class PlayerType
{
    NONE = 0,
    HUMAN = 1,
    COMP = 2
};

//class to store about each player(blue/red, human/comp, info for print)
class PlayerInfo
{
public:
    //constructor initialized with player type and info for print
    PlayerInfo(PlayerType type, string info):
        m_type(type), m_info(info)
    {}

    //get print info
    string getInfo()
    {
        return m_info;
    }

    //get player type (human or computer or none)
    PlayerType getType()
    {
        return m_type;
    }

private:
    PlayerType m_type;  //human or computer
    string    m_info;  //info for print
};

#endif  // _PLAYERINFO__H_
