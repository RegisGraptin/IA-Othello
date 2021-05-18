
#include "../include/othello.hpp"
#include <bitset>
#include <cstdio>

const int Board::HEURISTIQUE[N_ROWS][N_COLS] = {
    {500, -150, 30, 10, 10, 30, -150, 500},
    {-150, -250, 0, 0, 0, 0, -250, -150},
    {30, 0, 1, 2, 2, 1, 0, 30},
    {10, 0, 2, 16, 16, 2, 0, 10},
    {10, 0, 2, 16, 16, 2, 0, 10},
    {30, 0, 1, 2, 2, 1, 0, 30},
    {-150, -250, 0, 0, 0, 0, -250, -150},
    {500, -150, 30, 10, 10, 30, -150, 500}};

Board::Board()
{
    // Initialize the board with the 4 pawns
    board_white |= 1UL << ((3 * N_ROWS) + 3);
    board_white |= 1UL << ((4 * N_ROWS) + 4);

    board_black |= 1UL << ((4 * N_ROWS) + 3);
    board_black |= 1UL << ((3 * N_ROWS) + 4);
};


// DEBUG COMMAND BINARY : std::bitset<64>(*opponnent_player)

/**
 * Create a mask for the given position and try to see if we have 0.
 */
bool Board::check_empty_case(int x, int y) {
    return (board_black & (1UL << ((x * N_ROWS) + y))) == 0 
        && (board_white & (1UL << ((x * N_ROWS) + y))) == 0;
}

bool Board::is_active(unsigned long long * board, int x, int y) {
    return (*board & (1UL << (( x * N_ROWS) + y))) > 0;
}

void Board::set_piece(unsigned long long * board, int x, int y) {
    *board |= 1UL << ((x * N_ROWS) + y);
}

void Board::remove_piece(unsigned long long * board, int x, int y) {
    *board &= ~ ( 1UL << ((x * N_ROWS) + y));
}

void Board::move(Color p, int x, int y)
{
    unsigned long long * current_player; 
    unsigned long long * opponent_player;

    //Move with negative coordinate is a skip
    if(x <= -1 && y <= -1){
        return;
    }

    if (p == WHITE) {
        current_player = &board_white;
        opponent_player = &board_black;
    } else {
        current_player = &board_black;
        opponent_player = &board_white;
    }

    // Add a piece at the current position
    this->set_piece(current_player, x, y);


    // Check up
    if (x > 0 && !check_empty_case(x - 1, y))
    {
        int i = x - 1;
        while (i >= 0 && ! this->is_active(current_player, i, y))
        {
            i--;
            //If we encounter one empty position, this is not a valid propagation
            if(check_empty_case(i, y)){
                i = -1;
                break;
            }
        }

        // We have a solution
        if (i >= 0)
        {
            // FLIP coins BETWEEN
            this->flip_coin_vertically(current_player, opponent_player, i, x, y);
        }
    }



    // Check down
    if (x < N_ROWS - 1 && !check_empty_case(x + 1, y))
    {
        int i = x + 1;
        while (i < N_ROWS && ! this->is_active(current_player, i, y))
        {
            i++;
            //If we encounter one empty position, this is not a valid propagation
            if(check_empty_case(i, y)){
                i = N_ROWS+1;
                break;
            }
        }
        // We have a solution
        if (i < N_ROWS)
        {
            // FLIP coins BETWEEN
            this->flip_coin_vertically(current_player, opponent_player, x, i, y);
        }
    }

    // Check for the left side
    if (y > 0 && !check_empty_case(x, y - 1))
    {
        int j = y - 1;
        while (j >= 0 && ! this->is_active(current_player, x, j))
        {
            j--;
            //If we encounter one empty position, this is not a valid propagation
            if(check_empty_case(x, j)){
                j = -1;
                break;
            }
        }
        // We have a solution
        if (j >= 0)
        {
            // FLIP coins BETWEEN
            this->flip_coin_horizontally(current_player, opponent_player, x, j, y);
        }
    }

    // Check for the right side
    if (y < N_COLS - 1 && !check_empty_case(x, y + 1))
    {
        int j = y + 1;
        while (j < N_COLS && ! this->is_active(current_player, x, j))
        {
            j++;
            //If we encounter one empty position, this is not a valid propagation
            if(check_empty_case(x, j)){
                j = N_COLS+1;
                break;
            }
        }
        // We have a solution
        if (j < N_COLS)
        {
            // FLIP coins BETWEEN
            this->flip_coin_horizontally(current_player, opponent_player, x, y, j);
        }
    }

    // Check for the diagonal left upper
    if (x > 0 && y > 0 && !check_empty_case(x - 1, y - 1))
    {
        int i = x - 1;
        int j = y - 1;
        while (i >= 0 && j >= 0 && ! this->is_active(current_player, i, j))
        {
            i--;
            j--;
            //If we encounter one empty position, this is not a valid propagation
            if(check_empty_case(i, j)){
                i = -1;
                j = -1;
                break;
            }
        }
        if (i >= 0 && j >= 0)
        {
            this->flip_coin_diagonally_left(current_player, opponent_player, i, j, x, y);
        }
    }

    // Check for the digonal right bottom
    if (x < N_ROWS - 1 && y < N_COLS - 1 && !check_empty_case(x + 1, y + 1))
    {
        int i = x + 1;
        int j = y + 1;
        while (i < N_ROWS && j < N_COLS && ! this->is_active(current_player, i, j))
        {
            i++;
            j++;
            //If we encounter one empty position, this is not a valid propagation
            if(check_empty_case(i, j)){
                i = N_ROWS+1;
                j = N_COLS+1;
                break;
            }
        }
        if (i < N_ROWS && j < N_COLS)
        {
            this->flip_coin_diagonally_left(current_player, opponent_player, x, y, i, j);
        }
    }

    // Check for the diagonal right upper
    if (x > 0 && y < N_COLS && !check_empty_case(x - 1, y + 1))
    {
        int i = x - 1;
        int j = y + 1;
        while (i >= 0 && j < N_COLS && ! this->is_active(current_player, i, j))
        {
            i--;
            j++;
            //If we encounter one empty position, this is not a valid propagation
            if(check_empty_case(i, j)){
                i = -1;
                j = N_COLS+1;
                break;
            }
        }

        if (i >= 0 && j < N_COLS)
        {
            this->flip_coin_diagonally_right(current_player, opponent_player, x, y, i, j);
        }
    }

    // Check for the diagonal left bottom
    if (x < N_ROWS && y > 0 && !check_empty_case(x + 1, y - 1))
    {
        int i = x + 1;
        int j = y - 1;
        while (i < N_ROWS && j >= 0 && ! this->is_active(current_player, i, j))
        {
            i++;
            j--;
            //If we encounter one empty position, this is not a valid propagation
            if(check_empty_case(i, j)){
                i = N_ROWS+1;
                j = -1;
                break;
            }
        }

        if (i < N_ROWS && j >= 0)
        {
            this->flip_coin_diagonally_right(current_player, opponent_player, i, j, x, y);
        }
    }
};

void Board::flip_coin_vertically(unsigned long long * current_player, unsigned long long * opponnent_player, int x_a, int x_b, int j)
{

    for (int i = x_a + 1; i < x_b; i++)
    { 
        this->set_piece(current_player, i, j);
        this->remove_piece(opponnent_player, i, j);
    }
}

void Board::flip_coin_horizontally(unsigned long long * current_player, unsigned long long *opponnent_player, int x, int y_a, int y_b)
{
    for (int j = y_a + 1; j < y_b; j++)
    {
        this->set_piece(current_player, x, j);
        this->remove_piece(opponnent_player, x, j);
    }
}

void Board::flip_coin_diagonally_left(unsigned long long *current_player, unsigned long long *opponnent_player, int x_a, int y_a, int x_b, int y_b)
{
    while (x_a < x_b && y_a < y_b)
    {
        this->set_piece(current_player, x_a, y_a);
        this->remove_piece(opponnent_player, x_a, y_a);
        x_a++;
        y_a++;
    }
}

void Board::flip_coin_diagonally_right(unsigned long long *current_player, unsigned long long *opponnent_player, int x_a, int y_a, int x_b, int y_b)
{
    while (x_a > x_b && y_a < y_b)
    {
        this->set_piece(current_player, x_a, y_a);
        this->remove_piece(opponnent_player, x_a, y_a);
        x_a--;
        y_a++;
    }
}

bool Board::possible_move(Color p, int x, int y)
{

    unsigned long long * current_player; 
    unsigned long long * opponent_player; 

    if (p == WHITE) {
        current_player = &board_white;
        opponent_player = &board_black;
    } else {
        current_player = &board_black;
        opponent_player = &board_white;
    }


    // First, check if the current position has no piece
    if (! this->check_empty_case(x, y))
    {
        return false;
    }

    // Then, check the neighbors

    // Check up
    // Check if we have a neighbor above
    if (x > 0 && this->is_active(opponent_player, x-1, y))
    {
        int i = x - 1;
        // While we don't have the pawn for the current player
        while (i >= 0 && ! this->is_active(current_player, i, y))
        {
            //If we encounter one empty position, this is not a valid propagation
            if (this->check_empty_case(i, y)) {
                i = -1;
                break;
            }
            i--;

        }

        // We have a solution
        if (i >= 0)
        {   
            return true;
        }
    }

    // Check down
    if (x < N_ROWS - 1 && this->is_active(opponent_player, x+1, y))
    {
        int i = x + 1;
        while (i < N_ROWS && ! this->is_active(current_player, i, y))
        {
            //If we encounter one empty position, this is not a valid propagation
            if (this->check_empty_case(i, y)) {
                i = N_ROWS+1;
                break;
            }
            i++;
        }
        // We have a solution
        if (i < N_ROWS)
        {
            return true;
        }
    }

    // Check for the left side
    if (y > 0 && this->is_active(opponent_player, x, y - 1))
    {
        int j = y - 1;
        while (j >= 0 && ! this->is_active(current_player, x, j))
        {
            //If we encounter one empty position, this is not a valid propagation
            if (this->check_empty_case(x, j)) {
                j = -1;
                break;
            }
            j--;
        }
        // We have a solution
        if (j >= 0)
        {
            return true;
        }
    }

    // Check for the right side
    if (y < N_COLS - 1 && this->is_active(opponent_player, x, y + 1))
    {
        int j = y + 1;
        while (j < N_COLS && ! this->is_active(current_player, x, j))
        {
            //If we encounter one empty position, this is not a valid propagation
            if (this->check_empty_case(x, j)) {
                j = N_COLS+1;
                break;
            }
            j++;
        }
        // We have a solution
        if (j < N_COLS)
        {
            return true;
        }
    }

    // Check for the diagonal left upper
    if (x > 0 && y > 0 && this->is_active(opponent_player, x - 1, y - 1))
    {
        int i = x - 1;
        int j = y - 1;
        while (i >= 0 && j >= 0 && ! this->is_active(current_player, i, j)) 
        {
            //If we encounter one empty position, this is not a valid propagation
            if (this->check_empty_case(i, j)) {
                i = -1;
                j = -1;
                break;
            }
            i--;
            j--;
        }        
        if (i >= 0 && j >= 0)
        {
            return true;
        }
    }

    // Check for the digonal right bottom
    if (x < N_ROWS - 1 && y < N_COLS - 1 && this->is_active(opponent_player, x + 1, y + 1))
    {
        int i = x + 1;
        int j = y + 1;
        while (i < N_ROWS && j < N_COLS && ! this->is_active(current_player, i, j))
        {
            //If we encounter one empty position, this is not a valid propagation
            if (this->check_empty_case(i, j)) {
                i = N_ROWS+1;
                j = N_COLS+1;
                break;
            }
            i++;
            j++;
        }
        if (i < N_ROWS && j < N_COLS)
        {
            return true;
        }
    }

    // Check for the diagonal left bottom
    if (x > 0 && y < N_COLS && this->is_active(opponent_player, x - 1, y + 1))
    {
        int i = x - 1;
        int j = y + 1;
        while (i >= 0 && j < N_COLS && ! this->is_active(current_player, i, j)) 
        {
            //If we encounter one empty position, this is not a valid propagation
            if (this->check_empty_case(i, j)) {
                i = -1;
                j = N_COLS+1;
                break;
            }
            i--;
            j++;
        }

        if (i >= 0 && j < N_COLS)
        {
            return true;
        }
    }

    // Check for the diagonal left bottom
    if (x < N_ROWS && y > 0 && this->is_active(opponent_player, x + 1, y - 1))
    {
        int i = x + 1;
        int j = y - 1;
        while (i < N_ROWS && j >= 0 && ! this->is_active(current_player, i, j)) 
        {
            //If we encounter one empty position, this is not a valid propagation
            if (this->check_empty_case(i, j)) {
                i = N_ROWS+1;
                j = -1;
                break;
            }
            i++;
            j--;
        }

        if (i < N_ROWS && j >= 0)
        {
            return true;
        }
    }

    return false;
}

vector<tuple<int, int>> Board::all_possible_move(Color c)
{
    vector<tuple<int, int>> possible_moves;
    for (int i = 0; i < N_ROWS; i++)
    {
        for (int j = 0; j < N_COLS; j++)
        {
            if (this->possible_move(c, i, j))
            {
                possible_moves.push_back(std::make_tuple(i, j));
            }
        }
    }
    return possible_moves;
}

int Board::result(Color c)
{

    unsigned long long * current_player; 
    
    if (c == WHITE) {
        current_player = &board_white;
    } else {
        current_player = &board_black;
    }

    int count = 0;
    for (int i = 0; i < N_ROWS; i++)
    {
        for (int j = 0; j < N_COLS; j++)
        {
            if (this->is_active(current_player, i, j))
            {
                count++;
            }
        }
    }
    return count;
}

string Board::to_string()
{
    string output = "  - 0 - 1 - 2 - 3 - 4 - 5 - 6 - 7 -\n";
    for (int i = 0; i < N_ROWS; i++)
    {
        output += std::to_string(i);
        for (int j = 0; j < N_COLS; j++)
        {
            output += " | ";

            if(check_empty_case(i, j))
            {
                output += " ";
            }
            else
            {
                if (this->is_active(&board_white, i, j)) {
                    output += std::to_string(1);
                } else {
                    output += std::to_string(0);
                }
            }
        }
        output += " | ";
        output += "\n";
    }
    return output;
}

string Board::raw_string() 
{
    string output = "";
    for (int i = 0; i < N_ROWS; i++)
    {
        for (int j = 0; j < N_COLS; j++)
        {
            if (this->check_empty_case(i, j))
            {
                output += " ";
            }
            else
            {
                if (this->is_active(&board_white, i, j)) {
                    output += std::to_string(1);
                } else {
                    output += std::to_string(0);
                }
            }
        }
    }
    return output;
}

/**
* The game is finished if a player has no points.
*/
bool Board::finish()
{
    return this->noMove();
}

bool Board::isFull(){
    for (int i = 0; i < N_ROWS; i++)
    {
        for (int j = 0; j < N_COLS; j++)
        {
            if (this->check_empty_case(i, j))
            {
                return false;
            }
        }
    }
    //cout << "no more pawned can be placed" << '\n';
    return true;
}

bool Board::noPoints(){
    bool res = this->result(BLACK) == 0 || this->result(WHITE) == 0;
    /*
    if(res)
        cout << "one player has no more points" << '\n';*/
    return res;
}

bool Board::noMove(){
    bool res = this->all_possible_move(BLACK).empty() && this->all_possible_move(WHITE).empty();
    /*
    if(res)
        cout << "no move left for anyone" << '\n';*/
    return res;
}

int Board::heuristique(Color c)
{
    unsigned long long * current_player; 
    unsigned long long * opponent_player;

    int current_point = 0;
    int opponent_point = 0;

    if (c == WHITE) {
        current_player = &board_white;
        opponent_player = &board_black;
    } else {
        current_player = &board_black;
        opponent_player = &board_white;
    }

    for (int i = 0; i < N_ROWS; i++)
    {
        for (int j = 0; j < N_COLS; j++)
        {
            if (this->is_active(current_player, i, j))
            {
                current_point += HEURISTIQUE[i][j];
            }
            else if (this->is_active(opponent_player, i, j))
            {
                opponent_point += HEURISTIQUE[i][j];
            }
        }
    }
    return current_point - opponent_point;
}