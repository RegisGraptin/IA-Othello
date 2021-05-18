#include "../include/othello.hpp"
#include <string>
#include <stdio.h>
#include <limits>
#include <ctime>
#include <chrono>
#include <algorithm>


//Constructors

IPlayer::IPlayer(Color c){
	color = c;
}

HumanPlayer::HumanPlayer(Color c):IPlayer(c){}

RobotPlayer::RobotPlayer(Color c):IPlayer(c){}

//Methods
tuple<int,int> HumanPlayer::move(Board& b, Color c){
		//Input is not validated yet
		bool input = false;
		//Coordinate of the move to make
		int move_x;
		int move_y;
		do{
			//Prompt
			printf("$> Enter the line and the column where you want to place your pawn : ");
			//Expect two integers
			if(scanf("%d %d", &move_x, &move_y) != 2){
				//Error while reading. So we empty the buffer.
				//Capture everything except a newline character
				scanf("%*[^\n]");
				//Capture newline
				getchar();
				//Start the loop again
				continue;
			}
		//printf("%d %d", move_x, move_y);

		//Check if the move is not out of bound
		if((move_x >= 0 && move_x <= b.N_ROWS) && (move_y >= 0 && move_y <= b.N_COLS)){
			if(b.possible_move(c, move_x, move_y)){
				//Input is valid
				input = true;
			}
			else{
				input = false;
			}
		}
		else{
			//Error message
			printf("Bad input for one of the two coordinates !\n");
		}
		}while(!input);

		return tuple<int,int>(move_x,move_y);

}

tuple<int,int> RobotPlayer::move(Board& b, Color c){
	printf("Robot !\n");

	//int depth = 12;

	//tuple<int, int, int> result = this->minmax(b, depth, c, true);
	
	//tuple<int, int, int> result = this->minmaxab(b, depth, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), c, true);
	
	tuple<int, int, int> result = this->iterdeep(b, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), c, true);

	cout << "Best move with heuristic :" << get<0>(result) << "; move " << get<1>(result) << " " << get<2>(result) << '\n';
	return make_tuple(get<1>(result), get<2>(result));
}

/*
tuple<int,int> IPlayer::move(Board& b){
	printf("Super class of Player, not suppose to move !");
	return tuple<int,int>(-1,-1);
}*/


tuple<int, int, int> RobotPlayer::minmax(Board board, int depth, Color player, bool maximizingPlayer) {

	// Check if the game is finish or if we are at the limit of the depth
	if (depth == 0 || board.finish()) {
		return make_tuple(board.result(player), -1, -1);
	}
	// We want to maximize the heuristique
	if (maximizingPlayer) {
		// Initialize the value for the heuristique result
		int value = std::numeric_limits<int>::min();
		int max_x = -1, max_y = -1;

		// Get all the possible movement available
		vector<tuple<int, int>> moves = board.all_possible_move(player);
		//Shuffling to obtain different games on computer vs computer
		random_shuffle(moves.begin(), moves.end());

		if(!moves.empty()){
			// For each movement, we copy the board and we make the move
			for (tuple<int, int> move : moves) {
				Board b = Board(board);
				b.move(player, std::get<0>(move), std::get<1>(move));

				// Then, we compute the heuristique
				tuple<int, int, int> intermidiate = minmax(b, depth - 1, (Color) !player, false);
				// Save the result, if promising movement
				if (get<0>(intermidiate) > value) {
					value = get<0>(intermidiate);
					max_x = std::get<0>(move);
					max_y = std::get<1>(move);
				}
			}
		}
		else{
			//If we don't have any move available, go directly to the next step and retrieve the heuristic value anyway
			Board b = Board(board);
			tuple<int, int, int> intermidiate = minmax(b, depth - 1, (Color) !player, false);
			value = get<0>(intermidiate);
		}
		return make_tuple(value, max_x, max_y);
	} 
	// Do the same, but here we want to minimize the heuristique value
	else {
		int value = std::numeric_limits<int>::max();
		int max_x = -1, max_y = -1;

		vector<tuple<int, int>> moves = board.all_possible_move(player);
		random_shuffle(moves.begin(), moves.end());

		if(!moves.empty()){
			for (tuple<int, int> move : moves) {
				Board b = Board(board);
				b.move(player, std::get<0>(move), std::get<1>(move));

				tuple<int, int, int> intermidiate = minmax(b, depth - 1, (Color) !player, true);
				if (get<0>(intermidiate) < value) {
					value = get<0>(intermidiate);
					max_x = std::get<0>(move);
					max_y = std::get<1>(move);
				}
				
			}
		}
		else{
			Board b = Board(board);
			tuple<int, int, int> intermidiate = minmax(b, depth - 1, (Color) !player, true);
			value = get<0>(intermidiate);
		}
		return make_tuple(value, max_x, max_y);
	}
}

tuple<int, int, int> RobotPlayer::minmaxab(Board board, int depth, int alpha, int beta, Color player, bool maximizingPlayer) {

	// Check if the game is finish or if we are at the limit of the depth
	if (depth == 0 || board.finish()) {
		return make_tuple(board.heuristique(player), -1, -1);
	}
	// We want to maximize the heuristique
	if (maximizingPlayer) {
		// Initialize the value for the heuristique result
		int value = std::numeric_limits<int>::min();
		int max_x = -2, max_y = -2;

		// Get all the possible movement available
		vector<tuple<int, int>> moves = board.all_possible_move(player);
		
		//Shuffling to obtain different games on computer vs computer scenarios
		random_shuffle(moves.begin(), moves.end());

		int temp_alpha = alpha;
		
		if(!moves.empty()){
			// For each movement, we copy the board and we make the move
			for (tuple<int, int> move : moves) {

				Board b = Board(board);
				b.move(player, std::get<0>(move), std::get<1>(move));

				// Then, we compute the heuristique
				tuple<int, int, int> intermidiate = minmaxab(b, depth - 1, temp_alpha, beta, (Color) !player, false);
				// Save the result, if promising movement

				if (get<0>(intermidiate) > value) {
					value = get<0>(intermidiate);
					max_x = std::get<0>(move);
					max_y = std::get<1>(move);
					if (value >=  beta){
						//pruning
						break;
					}
					temp_alpha = temp_alpha >= value ? temp_alpha : value;
				}
			}
		}
		else{
			//If we don't have any move available, go directly to the next step and retrieve the heuristic value anyway
			Board b = Board(board);
			tuple<int, int, int> intermidiate = minmaxab(b, depth - 1, temp_alpha, beta, (Color) !player, false);
			value = get<0>(intermidiate);
		}
		return make_tuple(value, max_x, max_y);
	} 
	// Do the same, but here we want to minimize the heuristique value
	else {
		int value = std::numeric_limits<int>::max();
		int max_x = -3, max_y = -3;

		vector<tuple<int, int>> moves = board.all_possible_move(player);
		random_shuffle(moves.begin(), moves.end());

		int temp_beta = beta;
		
		if(!moves.empty()){
			for (tuple<int, int> move : moves) {
				Board b = Board(board);
				b.move(player, std::get<0>(move), std::get<1>(move));

				tuple<int, int, int> intermidiate = minmaxab(b, depth - 1, alpha, temp_beta, (Color) !player, true);
				if (get<0>(intermidiate) < value) {
					value = get<0>(intermidiate);
					max_x = std::get<0>(move);
					max_y = std::get<1>(move);
					if(value <= alpha){
						break;
					}
					temp_beta = value >= temp_beta ? temp_beta : value;
				}
				
			}
		}
		else{
			Board b = Board(board);
			tuple<int, int, int> intermidiate = minmaxab(b, depth - 1, alpha, temp_beta, (Color) !player, true);
			value = get<0>(intermidiate);
		}
		return make_tuple(value, max_x, max_y);
	}
}

tuple<int,int,int> RobotPlayer::iterdeep(Board board, int alpha, int beta, Color player, bool maximizingPlayer){
	
	//Initialization of the two queue of action
	deque<tuple<int,int>> to_do_moves (1, make_tuple(-1,-1));
	deque<tuple<int,int>> action_moves (1, make_tuple(-1,-1));

	//The time allowed for the search
	auto duration = std::chrono::nanoseconds(1000000000);
	
	//Index for the depth of search
	int i = 0;

	tuple<int,int,int> res = make_tuple(-1,-1,-1);

	//Limit of depth exploration
	int moves_to_go = (board.N_COLS * board.N_ROWS) - board.result(BLACK) - board.result(WHITE) - 1;

	//Timestamp before the launch of the loop
	auto begin = std::chrono::system_clock::now();
	auto last_step = begin;
	while(i <= moves_to_go){
		//Increment depth
		i++;

		//Get best movement for the current depth
		res = this->limited_search(board, i, alpha, beta, action_moves, to_do_moves, player, maximizingPlayer);

		//The next actions to favorize are copied in action_moves
		action_moves = to_do_moves;

		to_do_moves.clear();

		//Get the timestamp of the end of the search
		auto step_end = std::chrono::system_clock::now();

		//If we still have time for another exploration : calculated with :
		//(timestamp of now + duration of the previous search) - timestamp before the launch
		//The addition of the duration of the prevous search is to prevent to launch further exploration with little time before timeout.
		if (std::chrono::duration_cast<std::chrono::nanoseconds>((step_end + (last_step - begin)) - begin ) > duration){
			break;
		}

		//Update the time stamp for the previous search
		last_step = step_end;
	}
	//Display the depth reached
	std::cout << "Depth reached : " << i << '\n';

	return res;
}

tuple<int,int,int> RobotPlayer::limited_search(Board board, int depth, int alpha, int beta, deque<tuple<int,int>>& action_moves, deque<tuple<int,int>>& to_do_moves, Color player, bool maximizingPlayer){
	// Check if the game is finish or if we are at the limit of the depth
	if (depth == 0 || board.finish()) {
		return make_tuple(board.heuristique(player), -1, -1);
	}
	// We want to maximize the heuristique
	if (maximizingPlayer) {
		// Initialize the value for the heuristique result
		int value = std::numeric_limits<int>::min();
		int max_x = -1, max_y = -1;

		// Get all the possible movement available
		vector<tuple<int, int>> moves = board.all_possible_move(player);

		int temp_alpha = alpha;
		
		//If we have actions to favorize
		//Check necessary ? May have impact on performance
		if(!action_moves.empty()){
			//We take the first
			tuple<int,int> action = action_moves.front();
			//Check if it is a valid move by finding it in the list of possible move
			auto it = moves.begin();
			while(it != moves.end()){
				if(get<0>(*it) == get<0>(action) && get<1>(*it) == get<1>(action)){
					break;
				}
				it++;
			}
			//If so, we favorize it in the search
			if(it != moves.end()){
				moves.erase(it);
				moves.insert(moves.begin(),action);
			}
			//The action is then popped.
			action_moves.pop_front();
		}

		if(!moves.empty()){
			// For each movement, we copy the board and we make the move
			for (tuple<int, int> move : moves) {

				//Creating a new action_list
				deque<tuple<int,int>> tmp;

				Board b = Board(board);
				b.move(player, std::get<0>(move), std::get<1>(move));

				// Then, we compute the heuristique
				tuple<int, int, int> intermidiate = limited_search(b, depth - 1, temp_alpha, beta, action_moves, tmp ,(Color) !player, false);
				// Save the result, if promising movement

				if (get<0>(intermidiate) > value) {
					value = get<0>(intermidiate);
					max_x = std::get<0>(move);
					max_y = std::get<1>(move);

					//Update the list of movement
					to_do_moves = tmp;
					to_do_moves.push_front(make_tuple(max_x,max_y));

					if (value >=  beta){
						//pruning
						break;
					}
					temp_alpha = temp_alpha >= value ? temp_alpha : value;
				}
			}
		}
		else {
			//If we don't have any move available, go directly to next step, retrieve the heuristic value, and put a negative move in the list
			Board b = Board(board);
			deque<tuple<int,int>> tmp;
			tuple<int, int, int> intermidiate = limited_search(b, depth - 1, temp_alpha, beta, action_moves, tmp ,(Color) !player, false);
			to_do_moves = tmp;
			to_do_moves.push_front(make_tuple(max_x,max_y));
			value = get<0>(intermidiate);
		}
		
		return make_tuple(value, max_x, max_y);
	} 
	// Do the same, but here we want to minimize the heuristique value
	else {
		int value = std::numeric_limits<int>::max();
		int max_x = -1, max_y = -1;

		vector<tuple<int, int>> moves = board.all_possible_move(player);

		int temp_beta = beta;


		if(!action_moves.empty()){

			tuple<int,int> action = action_moves.front();
			auto it = moves.begin();
			while(it != moves.end()){
				if(get<0>(*it) == get<0>(action) && get<1>(*it) == get<1>(action)){
					break;
				}
				it++;
			}
			if(it != moves.end()){
				moves.erase(it);
				moves.insert(moves.begin(),action);
			}
			action_moves.pop_front();
		}
		if(!moves.empty()){
			for (tuple<int, int> move : moves) {

				deque<tuple<int,int>> tmp;

				Board b = Board(board);
				b.move(player, std::get<0>(move), std::get<1>(move));

				tuple<int, int, int> intermidiate = limited_search(b, depth - 1, alpha, temp_beta, action_moves, tmp,(Color) !player, true);
				if (get<0>(intermidiate) < value) {
					value = get<0>(intermidiate);
					max_x = std::get<0>(move);
					max_y = std::get<1>(move);

					to_do_moves = tmp;
					to_do_moves.push_front(make_tuple(max_x,max_y));

					if(value <= alpha){
						break;
					}
					temp_beta = value >= temp_beta ? temp_beta : value;
				}
				
			}
		}
		else{
			Board b = Board(board);
			deque<tuple<int,int>> tmp;
			tuple<int, int, int> intermidiate = limited_search(b, depth - 1, alpha, temp_beta, action_moves, tmp,(Color) !player, true);
			to_do_moves = tmp;
			to_do_moves.push_front(make_tuple(max_x,max_y));
			value = get<0>(intermidiate);
		}
		return make_tuple(value, max_x, max_y);
	}
}


