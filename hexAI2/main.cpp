/*
*  -main.cpp
*  -get game size and type(players human or computer) and start games
*/
#include <iostream>
#include <string>
#include <sstream>
#include "hexgame.h"
#include "player.h"

//constants for limiting hex board size
const int HEXSIZE_MIN = 3;
const int HEXSIZE_MAX = 26;

//get the hex board size. Since all subsequent objects are initialized 
//  with size, it is to be entered here.
int getHexSize(int& size)
{
    string strinput;

    //size less than 3 is trivial case(first player wins) 
    //size greater than 26 is limited by how moves are entered (e.g. 'c14' etc. [a-z] 26 alphabets).
    cout << endl;
    cout << string(30, '-') << endl;
    cout << "Input Hex game size between "<< HEXSIZE_MIN << 
        " and " << HEXSIZE_MAX << " : ";
    cin >> strinput;

    //take input as string and extract int from it to prevent infinite loops
    stringstream ss;
    ss << strinput;
    int intval = 0;
    ss >> intval;

    if (intval < HEXSIZE_MIN || intval > HEXSIZE_MAX)
    {
        cout << endl;
        cout << "** Invalid size.  Input size between "<< HEXSIZE_MIN << 
            " and " << HEXSIZE_MAX << " **" <<  endl;
        int res = getHexSize(size); //if invalid input prompt again
        return res;
    }
    size = intval;

    return 0;
}

//prompt and take input for game type.(player human or computer)
int getGamePlayers(PlayerMgr& playermgr)
{
    cout << endl;
    cout << string(30, '-') << endl;
    cout << "To play Human vs Human enter 1." << endl;
    cout << "To play Human vs Computer enter 2." << endl;
    cout << "To play Computer vs Computer enter 3." << endl;
    cout << "Imput players option : ";
    string strinput;
    cin >> strinput;
    
    stringstream ss;
    ss << strinput;
    int option = 0;
    ss >> option;

    if (option == 1 ) {
        playermgr.createPlayerBlue(PlayerType::HUMAN, "Player One Human 'Blue'(X)");
        playermgr.createPlayerRed(PlayerType::HUMAN, "Player One Human 'Red'(O)");
        return 0;
    }
    else if (option == 2 ) {
        char input = 'y';
        cout << string(50, '\n');
        cout << string(50, '-') << endl;
        cout << "Would you like to play first as 'Blue' (y/n): ";
        cin >> input;
        cout << endl;
        cout << string(50, '-') << endl;
        if (input == 'y' || input == 'Y') {
            playermgr.createPlayerBlue(PlayerType::HUMAN, "Player One Human 'Blue'(X)");
            playermgr.createPlayerRed(PlayerType::COMP, "Player Two Computer 'Red'(O)");
            return 0;
        } else {
            playermgr.createPlayerBlue(PlayerType::COMP, "Player One Computer 'Blue'(X)");
            playermgr.createPlayerRed(PlayerType::HUMAN, "Player Two Human 'Red'(O)");
            return 0;
        }
    }
    else if (option == 3) {
        playermgr.createPlayerBlue(PlayerType::COMP, "Player One Computer 'Blue'(X)");
        playermgr.createPlayerRed(PlayerType::COMP, "Player Two Computer 'Red'(O)");
        return 0;
    } else {
        cout << endl;
        cout << "** Incorrect players option. Try again **" << endl;
        return getGamePlayers(playermgr);  //if invalid input prompt again
    }
}

//main 
int main()
{
    int size;
    getHexSize(size);

    PlayerMgr playermgr;
    int res = getGamePlayers(playermgr);

    pair<Player&,Player&> players = 
        pair<Player&,Player&>(playermgr.getPlayerBlue(), playermgr.getPlayerRed());
    
    HexGame hexgame(size, players);  //hexgame object initialized with board size
    hexgame.play();  //play the game

    return 0;
}
