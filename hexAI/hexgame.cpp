/*
*  -class HexGame implementation
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
#include "hexgame.h"

using namespace std;

//input string to interrupt game
const string GAME_INTERRUPT = "9999";

//prompt for first player and print info about game
int HexGame::initialize()
{
    //printing messages and prompting for input
    if( GamePlayers::HUMAN_VS_HUMAN == m_gamePlayers )
    {
        m_playerBlue = PlayerInfo(PlayerType::HUMAN, "Player One Human 'Blue'(X)");
        m_playerRed = PlayerInfo(PlayerType::HUMAN, "Player Two Human 'Red'(O)");
    }
    else if (GamePlayers::HUMAN_VS_COMP == m_gamePlayers)
    {
        char input = 'y';
        cout << string(50, '\n');
        cout << string(50, '-') << endl;
        cout << "Would you like to play first as 'Blue' (y/n): ";
        cin >> input;
        cout << endl;
        cout << string(50, '-') << endl;
        if (input == 'y' || input == 'Y') 
        {
            m_playerBlue = PlayerInfo(PlayerType::HUMAN, "Player One Human 'Blue'(X)");
            m_playerRed = PlayerInfo(PlayerType::COMP, "Player Two Computer 'Red'(O)");
        }
        else
        {
            m_playerBlue = PlayerInfo(PlayerType::COMP, "Player One Computer 'Blue'(X)");
            m_playerRed = PlayerInfo(PlayerType::HUMAN, "Player Two Human 'Red'(O)");
        }
    }
    else if (GamePlayers::COMP_VS_COMP == m_gamePlayers)
    {
        m_playerBlue = PlayerInfo(PlayerType::COMP, "Player One Computer 'Blue'(X)");
        m_playerRed = PlayerInfo(PlayerType::COMP, "Player Two Computer 'Red'(O)");
    }
    else
        assert(false);

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
int HexGame::drawMove(PlayerEnum player)
{
    //print information about current state and previous move
    cout << string(50, '\n');
    cout << string(50, '-') << endl;
    cout << "Board (after move " << m_moveNumber << ")" << endl;
    if (PlayerEnum::PLAYER_BLUE == player )
        cout << "Previous move: " << m_playerBlue.getInfo() << " - " << m_previousMove << endl;
    else if (PlayerEnum::PLAYER_RED == player )
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

//start hex game
int HexGame::play()
{
    m_hex.initialize();  //initalize hex object

    initialize();  //initialize hexboard 

    drawMove(PlayerEnum::PLAYER_BLANK);  //draw blank board

    int gamestat = StatEnum::OK;
    PlayerEnum curplayer = PlayerEnum::PLAYER_BLUE;

    //game loop.  keep asking for move till game ends.
    while (gamestat == StatEnum::OK)
    {
        m_moveNumber++;

        if( GamePlayers::HUMAN_VS_HUMAN == m_gamePlayers )
            gamestat = play(curplayer); //play for current player
        else if ( GamePlayers::COMP_VS_COMP == m_gamePlayers )
            gamestat = playComputer(curplayer); //play computer vs comp
        else if ( GamePlayers::HUMAN_VS_COMP == m_gamePlayers )
        {
            if (PlayerEnum::PLAYER_BLUE == curplayer )
            {
                if (m_playerBlue.getType() == PlayerType::HUMAN)
                    gamestat = play(curplayer); 
                else if (m_playerBlue.getType() == PlayerType::COMP)
                    gamestat = playComputer(curplayer);
                else
                    assert(false);
            }
            else if (PlayerEnum::PLAYER_RED == curplayer )
            {
                if (m_playerRed.getType() == PlayerType::HUMAN)
                    gamestat = play(curplayer); 
                else if (m_playerRed.getType() == PlayerType::COMP)
                    gamestat = playComputer(curplayer);
                else
                    assert(false);
            }
            else
                assert(false);
        }
        else
            assert(false);

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

//compute move for given player
int HexGame::playComputer(PlayerEnum player)
{
    //print computing move message
    if (player == PlayerEnum::PLAYER_BLUE)
        cout << endl << m_playerBlue.getInfo() << " computing... ";
    else if (player == PlayerEnum::PLAYER_RED)
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

    //prompt for player move
    if (player == PlayerEnum::PLAYER_BLUE)
        cout << endl << m_playerBlue.getInfo() << " turn : ";
    else if (player == PlayerEnum::PLAYER_RED)
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

    //store for printing move info
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
            cout << m_playerBlue.getInfo() << endl;
        else if (PlayerEnum::PLAYER_RED == winner)
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
string HexGame::getMoveFromPosition(pair<int,int> position)
{
    char ichar = static_cast<char>(position.first + static_cast<int>('a'));
    string move;
    move += ichar;  //concat
    move += to_string(position.second + 1);
    return move;
}
