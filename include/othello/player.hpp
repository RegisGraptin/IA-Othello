#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "color.hpp"
#include "board.hpp"
#include <tuple>
#include <deque>

class IPlayer 
{
    public:
        //Color of the player, and by extension of his pawns
        Color color;

        //Constructor
        IPlayer(Color c);

        virtual tuple<int,int> move(Board& b, Color c) = 0;

        //TODO : Add an equality operator which compares the Color attribute.
        inline bool operator==(const IPlayer& P) {
        	return color == P.color;
        }
};

class HumanPlayer: public IPlayer{
	public:
		HumanPlayer(Color c);
		tuple<int,int> move(Board& b, Color c);
};

class RobotPlayer: public IPlayer{
	public:
		RobotPlayer(Color c);
		tuple<int,int> move(Board& b, Color c);

    private:

        /**
         * Min Max Implementation. 
         * @return tuple<int, int, int> score, move_x, move_y : return the heuristique value from the board movement we did. 
         *
         * @param board : Current board.
         * @param depth : Indicate the depth of the tree search. 
         * @param player : color of the current player.
         * @param maximizingPlayer : Indicate if we want to maximize or minimize the score of the player.
         */
        tuple<int, int, int> minmax(Board board, int depth, Color player, bool maximizingPlayer);

        /**
         * Min Max Implementation with the alpha beta optimisation. 
         * @return tuple<int, int, int> score, move_x, move_y : return the heuristique value from the board movement we did. 
         *
         * @param board : Current board.
         * @param depth : Indicate the depth of the tree search.
         * @param alpha : the best value for the Max player found so far.
         * @param beta : the best value for the Min player found so far. 
         * @param player : color of the current player.
         * @param maximizingPlayer : Indicate if we want to maximize or minimize the score of the player.
         */
        tuple<int, int, int> minmaxab(Board board, int depth, int alpha, int beta, Color player, bool maximizingPlayer);

        /**
         * Iterative deepening implementation.
         * @return tuple<int, int, int> score, move_x, move_y : return the heuristique value from the board movement we did. 
         *
         * @param board : Current board.
         * @param alpha : the best value for the Max player found so far.
         * @param beta : the best value for the Min player found so far. 
         * @param player : color of the current player.
         * @param maximizingPlayer : Indicate if we want to maximize or minimize the score of the player.
        */
        tuple<int,int,int> iterdeep(Board board, int alpha, int beta, Color player, bool maximizingPlayer);
        
        /**
        * Sub-function for the iterative deepening : modification of the minmaxab function to retrieve the best sequence of action for the current depth.
        * @return tuple<int, int, int> score, move_x, move_y : return the heuristique value from the board movement we did. 
         *
         * @param board : Current board.
         * @param depth : Indicate the depth of the tree search.
         * @param alpha : the best value for the Max player found so far.
         * @param beta : the best value for the Min player found so far.
         * @param action_moves : the best sequence of actions to favorize in the tree search.
         * @param to_do_moves : the best sequence of actions for the current depth 
         * @param player : color of the current player.
         * @param maximizingPlayer : Indicate if we want to maximize or minimize the score of the player.
        */
        tuple<int,int,int> limited_search(Board board, int depth, int alpha, int beta, deque<tuple<int,int>>& action_moves, deque<tuple<int,int>>& to_do_moves, Color player, bool maximizingPlayer);
};


#endif
