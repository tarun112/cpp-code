/*
*  -declares 'Player' abstract base class
*  -declares 'PlayerHuman', 'PlayerComputer' derived  classes.
*  -declares 'PlayerMgr' managing class.
*  -This implements the Player inheritance and management.  It iterfaces with HexGame  
*     to accept & make player specific moves.
*/
#ifndef _PLAYER___HPP_
#define _PLAYER___HPP_

#include <string>
#include <cassert>
#include "playerenums.h"
using namespace std;

class HexGame;

//abstract base class Player
//it implements the mandatory methods for getting & setting info & color
class Player {
public:
    //constructor with player color and info arguments
    Player(PlayerColor color, const string& info) :
        m_color(color), m_info(info)
    {}

    virtual ~Player() { }; //virtual destructor

    virtual int play(HexGame&) = 0; //abstract method
        
    const string& getInfo() { return m_info; } //get info

    PlayerColor getColor() { return m_color; } //get color

private:
    string m_info;
    PlayerColor m_color;
};


//class PlayerHuman is derived from class Player.
//implements methods for human play
class PlayerHuman : public Player {
public:
    PlayerHuman(PlayerColor color, const string& info) : Player(color, info)
    {}

    virtual ~PlayerHuman() override
    {}

    virtual int play(HexGame&) override;
};


//class PlayerComputer is derived from class Player.
//implements methods for computer play
class PlayerComputer : public Player {
public:
    PlayerComputer(PlayerColor color, const string& info) : Player(color, info)
    {}

    virtual ~PlayerComputer() override
    {}

    virtual int play(HexGame&) override;
};


//class PlayerMgr manages PLayerHuman & PlayerComputer objects
class PlayerMgr {
public:
    PlayerMgr() : m_playerBlue(nullptr),m_playerRed(nullptr)
    {}

    ~PlayerMgr() {
        delete m_playerBlue;
        m_playerBlue = nullptr;
        delete m_playerRed;
        m_playerRed = nullptr;
    }

    int createPlayerBlue( PlayerType type, const string& info);
    Player& getPlayerBlue() { return *m_playerBlue; }

    int createPlayerRed( PlayerType type, const string& info);
    Player& getPlayerRed() { return *m_playerRed; }

private:
    Player* m_playerBlue;
    Player* m_playerRed;
};

#endif // _PLAYER___HPP_
