/*
*  -declares 'Player' abstract base class
*  -declares 'PlayerHuman', 'PlayerComputer' derived  classes.
*  -declares 'PlayerMgr' managing class.
*  -This implements the Player inheritance and management.  It iterfaces with HexUI  
*     to accept & make player specific moves.
*/
#ifndef _PLAYER___HPP_
#define _PLAYER___HPP_

#include <string>
#include <cassert>
#include "playerenums.h"
using namespace std;

class HexUI;

//abstract base class Player
//it implements the mandatory methods for getting & setting info & color
class Player {
public:
    //constructor with player color and info arguments
    Player(PlayerColor color, const string& info) :
        m_color(color), m_info(info)
    {}

    virtual ~Player() { }; //virtual destructor

    virtual int play(HexUI&) = 0; //abstract method
        
    const string& getInfo() { return m_info; } //get info

    PlayerColor getColor() { return m_color; } //get color

private:
    string m_info;
    PlayerColor m_color;
};

//----------------------------------------------------------

//class PlayerHuman is derived from class Player.
//implements methods for human play
class PlayerHuman : public Player {
public:
    PlayerHuman(PlayerColor color, const string& info) : Player(color, info)
    {}

    virtual ~PlayerHuman() override
    {}

    virtual int play(HexUI&) override;
};

//----------------------------------------------------------

//class PlayerComputer is derived from class Player.
//implements methods for computer play
class PlayerComputer : public Player {
public:
    PlayerComputer(PlayerColor color, const string& info) : Player(color, info)
    {}

    virtual ~PlayerComputer() override
    {}

    virtual int play(HexUI&) override;
};

//----------------------------------------------------------

//class PlayerMgr manages PLayerHuman & PlayerComputer objects
class PlayerMgr {
public:
    PlayerMgr(const pair<PlayerType,string>& blueplayer, 
              const pair<PlayerType,string>& redplayer);

    ~PlayerMgr() {
        delete m_playerBlue;
        m_playerBlue = nullptr;
        delete m_playerRed;
        m_playerRed = nullptr;
    }

    Player& getPlayerBlue() { return *m_playerBlue; }
    Player& getPlayerRed() { return *m_playerRed; }

private:
    int createPlayerBlue( const pair<PlayerType,string>& blueplayer );
    int createPlayerRed( const pair<PlayerType,string>& redplayer );

    PlayerMgr() = delete;
    PlayerMgr(const PlayerMgr&) = delete;
    PlayerMgr& operator=(const PlayerMgr&) = delete;

private:
    Player* m_playerBlue;
    Player* m_playerRed;
};

#endif // _PLAYER___HPP_
