/*
*  -class HexGame implementation
*  -prompts for input and print output using ascii chars on console
*/
#include <utility>
#include <string>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <sstream>
#include <cctype>
#include <limits>
#include "hexgame.h"

using namespace std;

const string GAME_INTERRUPT = "9999";

//prompt for first player and print info about game
int HexGame::initialize()
{
    //printing messages and prompting for input
    cout << string(50, '\n');
    cout << string(50, '-') << endl;
    cout << "Would you like to play first as 'Blue' (y/n): ";
    char input;
    cin >> input;
    cout << endl;
    cout << string(50, '-') << endl;
    
    if (input == 'y' || input == 'Y') 
    {
        m_playerBlue = "Player One 'Blue'(X)";
        m_playerRed = "Player Two 'Red'(O)";
    }
    else
    {
        m_playerBlue = "Player Two 'Blue'(X)";
        m_playerRed = "Player One 'Red'(O)";
    }

    cout << "On Board:" << endl;
    cout << endl;
    cout << "-Blank position is '.'" << endl;
    cout << endl;
    cout << "-" << m_playerBlue << " piece is 'X'" << endl;; 
    cout << "\tconnects West-East, moves 1st"<< endl; 
    cout << endl;
    cout << "-" << m_playerRed << " piece is 'O'" << endl;; 
    cout << "\tconnects North-South"<< endl; 
    cout << endl;
    cout << "-Input moves as for example 'c6'" << endl;
    cout << endl;
    cout << "-To interrupt game input '9999'" << endl;
    cout << string(50, '-') << endl;
    cout << "Press Enter to continue";
    cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
    cin.ignore(std::numeric_limits<streamsize>::max(),'\n');

    return StatEnum::OK;
}

//draw board after a move
int HexGame::drawMove(PlayerEnum player)
{
    cout << string(50, '\n');
    cout << string(50, '-') << endl;
    cout << "Board (after move " << m_moveNumber << ")" << endl;
    if (PlayerEnum::PLAYER_BLUE == player )
        cout << "Previous move: " << m_playerBlue << " - " << m_previousMove << endl;
    else if (PlayerEnum::PLAYER_RED == player )
        cout << "Previous move: " << m_playerRed << " - " << m_previousMove << endl;
    else
        cout << "Previous move: None" << endl;
    cout << string(20, '-') << endl;
    cout << endl;

    m_hex.drawBoard();
    
    cout << endl << endl;
    cout << string(50, '-') << endl;
    return StatEnum::OK;
}

//start hex game
int HexGame::play()
{
    int res = m_hex.initialize();  //iniitalize hex object

    initialize();  //initialize hexboard 

    drawMove(PlayerEnum::PLAYER_BLANK);  //draw blank board

    int gameStat = StatEnum::OK;
    PlayerEnum curplayer = PlayerEnum::PLAYER_BLUE;

    //game loop.  keep asking for move till game ends.
    while (gameStat == StatEnum::OK)
    {
        m_moveNumber++;

        //play for current player
        gameStat = play(curplayer); 

        //change player after move
        if (PlayerEnum::PLAYER_BLUE == curplayer)
            curplayer = PlayerEnum::PLAYER_RED;
        else if (PlayerEnum::PLAYER_RED == curplayer)
            curplayer = PlayerEnum::PLAYER_BLUE;
        else
            assert(false);
    }

    return StatEnum::OK;
}

//play for given player
int HexGame::play(PlayerEnum player)
{
    //get move for player as hex-position
    pair<int,int> position;
    int res = getMove(player, position);
    if (StatEnum::INTERRUPT == res)
        return res;

    //place move on board 
    res = move(player, position);
    return res;
}

//check if move is within hex-board bounds
bool HexGame::isMoveOnBoard(pair<int,int> position)
{
    int ipos = position.first;
    int jpos = position.second;
    if ((ipos < 0 || ipos >= m_size) || (jpos < 0 || jpos >= m_size) )
        return false;
    return true;
}

//prompt for player input and check if input is correct
int HexGame::getMove(PlayerEnum player, pair<int,int>& position)
{
    string input;

    //prompt for player move
    if (player == PlayerEnum::PLAYER_BLUE)
        cout << endl << m_playerBlue << " turn : ";
    else if (player == PlayerEnum::PLAYER_RED)
        cout << endl << m_playerRed << " turn : ";
    else
        assert(false);
    cin >> input ;

    //chcek for game interupt and return interrupt status
    if ( input.compare(GAME_INTERRUPT) == 0 )
    {
        cout << endl << "** Game interrupted **" << endl;
        cout << string(50,'-') << endl;
        cout << "Press Enter to continue";
        cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
        cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
        return StatEnum::INTERRUPT;
    }

    //convert the input to hex-position
    char icoord;
    int jcoord; 
    stringstream(input.substr(0,1)) >> icoord;
    stringstream(input.substr(1)) >> jcoord;
    position.first = toupper(static_cast<int>(icoord)) - static_cast<int>('A');
    position.second = jcoord - 1;
   
    //check move hex-position.  if not valid print message and prompt 
    //  for another move
    if (! isMoveOnBoard(position))
    {
        cout << "Invalid move. Position not on Board" << endl;
        int res = getMove(player, position);
        return res;
    }

    m_previousMove = input;  

    return StatEnum::OK;
}

//check and place move on board.  check for winner.
int HexGame::move(PlayerEnum player, pair<int,int> position)
{
    //check and place positon on board for player
    int res = m_hex.setPositionStatus(position.first, position.second, player);
    if (res == StatEnum::POS_OCCUPIED)
    {
        cout << "Invalid move. Position on Board occupied." << endl;
        res = play(player);
        return res;
    }

    drawMove(player);  //draw board

    //check if any winnner
    PlayerEnum winner = PlayerEnum::PLAYER_BLANK;
    res = m_hex.checkForWinner(winner);
    if (res == StatEnum::WINNER )
    {
        cout << endl << string(50,'-') << endl;
        cout << "** Game Ends **" << endl;
        cout << "Winner is: ";
        if (PlayerEnum::PLAYER_BLUE == winner)
            cout << m_playerBlue << endl;
        else if (PlayerEnum::PLAYER_RED == winner)
            cout << m_playerRed << endl;
        else
            assert(false);
        cout << string(50,'-') << endl;
        cout << "Press Enter to continue";
        cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
        cin.ignore(std::numeric_limits<streamsize>::max(),'\n');

        return StatEnum::WINNER;
    }

    return StatEnum::OK;
}

