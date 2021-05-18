
#include <string>
#include <iostream>
#include <stdio.h>

#include "../include/othello.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    // Set the number of human player in the game
    int number_of_player = 1;

    // Check if we have a parameter    
    if (argc > 1)
    {
        int input_number = std::stoi(argv[1]);

        switch (input_number)
        {
        case 0:
            number_of_player = 0;
            break;
        case 2:
            number_of_player = 2;
            break;
        default:
            break;
        }
    }

    std::cout << "Othello" << std::endl;
    
    IGame g = IGame(number_of_player);
    g.play_game(); 

    return 0;
}
