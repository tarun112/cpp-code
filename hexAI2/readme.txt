Implements game of Hex. Description can be found at:
https://en.wikipedia.org/wiki/Hex_%28board_game%29
http://mathworld.wolfram.com/GameofHex.html

The game uses ascii UI.  It can be played between two players taking turns or a player against computer.  Also computer can be played against computer. 


Implementation includes:
-ascii UI
-graph to store the board & to capture moves.
-game AI based on monte carlo simulation
-unionfind to check for winner/completion

Implemenation details:
c++/c++11/oops

The code is edited, compiled & built on Visual Studio Express 2013

The various modules & classes are as follows:

Player (player.h, .cpp):  base class for player information & move
PlayerHuman, PlayerCompuer: derived classes used for making moves polymorphically.  interfaces with HexUI
playerMgr: noncopyable, creates & manages player objects. interfaces with HexGame

HexGame: (hexgame.h, .cpp)   accept game type & information as input & initiate the game.  interfaces with hexUI class

HexUI (hexui.h, .cpp):  class implementing methods for ascii input/output
it iterfaces with Hex class

Hex (hex.h, .cpp):  creates & manages board using Graph.  interfaces with HexAI for coputers move and for checking game completion and winner

HexAI (hexai.h, .cpp): computes moves with best chance of winning using monte carlo simulation.
uses UnionFind to check for winner.

Graph (graph.h, .cpp): bidirectional graph with nodes as board position and edges as connectivity.

UnionFind (unionfind.h): checks for connectivity between two given nodes


