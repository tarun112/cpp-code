/*
*  -class HexUI implementation
*  -prompts for input and print output using ascii chars on console
*  -uses hex to mange board and moves
*/
#include <utility>
#include <string>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <sstream>
#include <cctype>
#include <limits>
#include "player.h"
#include "hexui.h"

using namespace std;

//input string to interrupt game
const string GAME_INTERRUPT = "9999";

//prompt for first player and print info about game
int HexUI::initialize()
{
    //print board and game playing information
    cout << string(50, '-') << endl;
    cout << "On Board:" << endl;
    cout << endl;
    cout << "-Blank position is '.'" << endl;
    cout << endl;
    cout << "-" << m_playerBlue.getInfo() << " piece is 'X'" << endl;; 
    cout << "\tconnects West-East, moves 1st"<< endl; 
    cout << endl;
    cout << "-" << m_playerRed.getInfo() << " piece is 'O'" << endl;; 
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
int HexUI::drawMove(PlayerColor player)
{
    //print information about current state and previous move
    cout << string(50, '\n');
    cout << string(50, '-') << endl;
    cout << "Board (after move " << m_moveNumber << ")" << endl;
    if (PlayerColor::BLUE == player )
        cout << "Previous move: " << m_playerBlue.getInfo() << " - " << m_previousMove << endl;
    else if (PlayerColor::RED == player )
        cout << "Previous move: " << m_playerRed.getInfo() << " - " << m_previousMove << endl;
    else
        cout << "Previous move: None" << endl;
    cout << string(20, '-') << endl;
    cout << endl;

    m_hex.drawBoard();  //call hex to draw move
    
    cout << endl << endl;
    cout << string(50, '-') << endl;
    return StatEnum::OK;
}

inline Player* HexUI::getOtherPlayer(Player* player) 
{
    if(PlayerColor::BLUE == player->getColor()) 
        return &m_playerRed;
    else if(PlayerColor::RED == player->getColor()) 
        return &m_playerBlue;
    return nullptr;
}

//start hex game
int HexUI::play()
{
    m_hex.initialize();  //initalize hex object

    initialize();  //initialize hexboard 

    drawMove(PlayerColor::BLANK);  //draw blank board

    int gamestat = StatEnum::OK;
    Player* curplayer = &m_playerBlue;

    //game loop.  keep asking for move till game ends.
    while (gamestat == StatEnum::OK)
    {
        m_moveNumber++;
        gamestat = curplayer->play(*this);
        curplayer = getOtherPlayer(curplayer);
        //check for nullptr
    }

    return StatEnum::OK;
}

//play for given player
int HexUI::play(PlayerColor player)
{
    //get move for player as hex-position
    pair<int,int> position;
    int res = getMove(player, position);
    if (StatEnum::INTERRUPT == res)
        return res;

    //get move in printable form 
    m_previousMove =  getMoveFromPosition(position);

    //place move on board 
    res = makeMove(player, position);
    return res;
}

//compute move for given player
int HexUI::playComputer(PlayerColor player)
{
    //print computing move message
    if (player == PlayerColor::BLUE)
        cout << endl << m_playerBlue.getInfo() << " computing... ";
    else if (player == PlayerColor::RED)
        cout << endl << m_playerRed.getInfo() << " computing... ";
    else
        assert(false);

    //compute move and get it as hex-position
    pair<int,int> position;
    int res = m_hex.getComputerMove(player, position);
    if (StatEnum::INTERRUPT == res)
        return res;

    //get move in printable form 
    m_previousMove =  getMoveFromPosition(position);

    //place move on board 
    res = makeMove(player, position);
    return res;
}

//check if move is within hex-board bounds
bool HexUI::isMoveOnBoard(pair<int,int> position)
{
    int ipos = position.first;
    int jpos = position.second;
    if ((ipos < 0 || ipos >= m_size) || (jpos < 0 || jpos >= m_size) )
        return false;
    return true;
}

//prompt for player input and check if input is correct
int HexUI::getMove(PlayerColor player, pair<int,int>& position)
{

    //prompt for player move
    if (player == PlayerColor::BLUE)
        cout << endl << m_playerBlue.getInfo() << " turn : ";
    else if (player == PlayerColor::RED)
        cout << endl << m_playerRed.getInfo() << " turn : ";
    else
        assert(false);

    //get input in string
    string input;
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

    return StatEnum::OK;
}

//check and place move on board.  check for winner.
int HexUI::makeMove(PlayerColor player, pair<int,int> position)
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
    PlayerColor winner = PlayerColor::BLANK;
    res = m_hex.checkForWinner(winner);
    if (res == StatEnum::WINNER )
    {
        cout << endl << string(50,'-') << endl;
        cout << "** Game Ends **" << endl;
        cout << "Winner is: ";
        if (PlayerColor::BLUE == winner)
            cout << m_playerBlue.getInfo() << endl;
        else if (PlayerColor::RED == winner)
            cout << m_playerRed.getInfo() << endl;
        else
            assert(false);
        cout << string(50,'-') << endl;
        cout << "Press Enter to continue";
        cin.ignore(std::numeric_limits<streamsize>::max(),'\n');

        return StatEnum::WINNER;
    }

    //check if board is completly filled and no winner.
    //By rules of hex this will never happen.
    if (m_moveNumber == m_size * m_size)
    {
        cout << endl << string(50,'-') << endl;
        cout << "** Game Ends **" << endl;
        cout << "Game is a Tie. No more possible moves." << endl;
        cout << string(50,'-') << endl;
        cout << "Press Enter to continue";
        cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
        cin.ignore(std::numeric_limits<streamsize>::max(),'\n');

        return StatEnum::BOARDFULL;
    }
    
    return StatEnum::OK;
}

//for given position return printable move
string HexUI::getMoveFromPosition(pair<int,int> position)
{
    char ichar = static_cast<char>(position.first + static_cast<int>('a'));
    string movestr;
    movestr += ichar;  //concat
    movestr += to_string(position.second + 1);
    return movestr;
}
