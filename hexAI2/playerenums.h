/*
*  -declares Enums 
*/

#ifndef _PLAYERENUMS__H_
#define _PLAYERENUMS__H_

//enum for red and blue players
enum class PlayerColor
{
    BLANK = 0,
    BLUE = 1,
    RED = 2
};


//enum for player type (player human or computer)
enum class PlayerType
{
    NONE = 0,
    HUMAN = 1,
    COMP = 2
};


//enum for status passed by methods.  
//in methods they are passed as int
enum StatEnum
{
    NOK = -1,
    OK = 0,
    INVALID_POS = 1,
    POS_OCCUPIED = 2,
    WINNER = 3,
    INTERRUPT = 4,
    BOARDFULL = 5
};

#endif  //_PLAYERENUMS__H_
