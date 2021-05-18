#ifndef COLOR_HPP
#define COLOR_HPP
#include <string>
#include <iostream>

/*
Enum to code the color of a pawn of the game.
*/
enum Color {
    BLACK,
    WHITE
};

inline std::string convert_color(Color c){
	return c == BLACK ? "Black" : "White";
}

#endif
