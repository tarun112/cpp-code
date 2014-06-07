#include <iostream>
#include "hexgame.h"

//const int HEXSIZE = 7;
const int HEXSIZE_MIN = 3;
const int HEXSIZE_MAX = 26;

//get the hex board size. Since all subsequent objects are initialized 
//  with size, it is to be entered here.
int getHexSize(int& size)
{
    int input = 0;
    //size less than 3 is trivial case(first player wins or draw) 
    //size greater than 26 is limited by how moves are entered ('c14' etc.).
    cout << "Input Hex game size between "<< HEXSIZE_MIN << 
        " and " << HEXSIZE_MAX << " : ";
    cin >> input;
    if (input < HEXSIZE_MIN || input > HEXSIZE_MAX)
    {
        cout << "Invalid size.  Input size between "<< HEXSIZE_MIN << 
            " and " << HEXSIZE_MAX << endl;
        int res = getHexSize(size);
        return res;
    }
    size = input;
    return 0;
}

int main()
{
    int size;
    getHexSize(size);

    HexGame hexgame(size);  //hexgame object initialized with board size
    hexgame.play();  //play the game

    return 0;
}
