#include "color.hpp"

#include <string>
#include <iostream>
#include <tuple>
#include <vector>

using namespace std;

#ifndef BOARD_HPP
#define BOARD_HPP

class Board 
{


    public:

        /**
         * Board dimension.
         */
        static const int N_COLS = 8;  
        static const int N_ROWS = 8;

        Board();

        /**
         * Realize a move for a given position.
         */
        void move(Color p, int x, int y);

        /**
         * Return true if the movement is possible or not.
         */
        bool possible_move(Color p, int x, int y);

        /**
         * Give all possible move given a color.
         */
        vector<tuple<int, int>> all_possible_move(Color c);

        /**
         * Get the number of piece on the board for the given color.
         */
        int result(Color c);

        /**
         * Print the board
         */
        string to_string();

        /**
         * Print the board representation with no indication on it.
         */
        string raw_string();

        /**
         * Return true if the game is finish.
         */
        bool finish();

        /**
         * Compute the heuristique of the current board for the given color.
         */
        int heuristique(Color c);

    private:
        /**
        * Board representation.
        * One binary for the white pawns and another one for the black pawns.
        * In the binary representation, there is a pawn when we have a 1.
        * The position is determined by : (N_ROWS * x) + y.
        */
        unsigned long long board_white = 0;
        unsigned long long board_black = 0;


        /**
         * Array of heuristique. 
         * Each position give us the point earn.
         * SOURCE : http://emmanuel.adam.free.fr/site/IMG/pdf/jeu.pdf
         */
        static const int HEURISTIQUE[N_ROWS][N_COLS];

        /**
        * Return true if no player can make a move
        */
        bool noMove();

        /**
        * Return true if one of the player has no pawn left
        */
        bool noPoints();

        /**
        * Return true if the board is filled with pawns.
        */
        bool isFull();


        /**
         * Flip the coin on the vertical axe in the interval [x_a ; x_b].
         * @requiere : x_a < x_b.
         */
        void flip_coin_vertically(unsigned long long *current_player, unsigned long long *opponnent_player, int x_a, int x_b, int y);

        /**
         * Flip the coin in the horizontal axe.
         * @requiere : y_a < y_b
         */
        void flip_coin_horizontally(unsigned long long *current_player, unsigned long long *opponnent_player, int x, int y_a, int y_b);

        /**
         * Flip the coin in the left diagonal (top left --> bottom right). 
         * @requiere : x_a < x_b
         * @requiere : y_a < y_b
         */
        void flip_coin_diagonally_left(unsigned long long *current_player, unsigned long long *opponnent_player, int x_a, int y_a, int x_b, int y_b);


        /**
         * Flip the coin in the right diagonal (top right --> bottom left)
         * @requiere : x_a > x_b
         * @requiere : y_a < y_b
         */
        void flip_coin_diagonally_right(unsigned long long *current_player, unsigned long long *opponnent_player, int x_a, int y_a, int x_b, int y_b);


        /**
         * From a given position, check if it's empty.
         */
        bool check_empty_case(int x, int y);

        /**
         * Check for the given board, if at the given position, we have a pawn.
         * Return true if we have it at the given position.
         */
        bool is_active(unsigned long long * board, int x, int j);

        /**
         * Set a coin at the given position
         */
        void set_piece(unsigned long long * board, int x, int y);

        /**
         * Remove a coin at the given position.
         */
        void remove_piece(unsigned long long * board, int x, int y);

};


#endif