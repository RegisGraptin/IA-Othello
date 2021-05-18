#include "../include/othello.hpp"
#include <tuple>
#include <vector>
#include <iostream>

using namespace std;

IGame::IGame()
{

	//Call the other constructors
	player1 = new HumanPlayer(BLACK);
	player2 = new HumanPlayer(WHITE);
	board = Board();
}

IGame::IGame(int numberOfRobot)
{

	switch (numberOfRobot)
	{
	case 0:
		player1 = new HumanPlayer(BLACK);
		player2 = new HumanPlayer(WHITE);
		break;
	case 1:
		player1 = new HumanPlayer(BLACK);
		player2 = new RobotPlayer(WHITE);
		break;
	case 2:
		player1 = new RobotPlayer(BLACK);
		player2 = new RobotPlayer(WHITE);
		break;
	}
	board = Board();
}

void IGame::play_game()
{

	//Black always begin
	IPlayer *current_player = player1;
	//Set the turn count to 1
	int nb_turn = 1;

	//First view
	cout << board.to_string() << endl;

	//Until no player can play
	while (!board.finish())
	{

		cout << "Turn n°" << nb_turn << endl;

		cout << "Turn of " << convert_color(current_player->color) << endl;
		cout << "Current points : " << board.result(current_player->color) << endl;

		//If we have a possibility to play
		if (!board.all_possible_move(current_player->color).empty())
		{

			tuple<int, int> res = current_player->move(board, current_player->color);
			board.move(current_player->color, get<0>(res), get<1>(res));

			current_player = current_player == player1 ? player2 : player1;

			//Increment the number of turn
			nb_turn++;

			//Print the board
			cout << board.to_string() << endl;
		}
		else
		{
			printf("No possibility to move. Skipping turn \n");

			current_player = current_player == player1 ? player2 : player1;

			nb_turn++;

			cout << board.to_string() << endl;
		}
	}

	int b_results = board.result(BLACK);
	int w_results = board.result(WHITE);

	//Afficher victoire, puis exit
	cout << "Results for the black player : " << b_results << '\n' <<  "Results for the white player : " << w_results << '\n' << "Game Over";
}
