/*
*  -main.cpp
*  -get game size and type(players human or computer) and start games
*/
#include <iostream>
#include <string>
#include <sstream>
#include "hexgame.h"

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
GamePlayers getGamePlayers()
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

    if (option == 1 )
        return GamePlayers::HUMAN_VS_HUMAN;
    else if (option == 2 )
        return GamePlayers::HUMAN_VS_COMP;
    else if (option == 3)
        return GamePlayers::COMP_VS_COMP;
    else
    {
        cout << endl;
        cout << "** Incorrect players option. Try again **" << endl;
        return getGamePlayers();  //if invalid input prompt again
    }
}

//main 
int main()
{
    int size;
    getHexSize(size);

    GamePlayers gameplayers = getGamePlayers();

    HexGame hexgame(size, gameplayers);  //hexgame object initialized with board size
    hexgame.play();  //play the game

    return 0;
}
