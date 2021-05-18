#ifndef GAME_HPP
#define GAME_HPP

#include "player.hpp"
#include "board.hpp"

class IGame 
{
    public:
        //The first player
        IPlayer* player1;
        //The second player
        IPlayer* player2;
        //The Game Board
        Board board;

        /*
        Constructor
        */
        IGame();

        /**
         * Game constructor with robot.
         * The numberOfRobot determine the number of robot player.
         * This number have to be 1 or 2.
         */
        IGame(int numberOfRobot);

        /*
        Launch a loop which stops when the game is finished.
        During the loop, each player makes a move one turn at a time until the board is full of pawns.
        */
        void play_game();
};

#endif
