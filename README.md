# Othello Game - Reversi

Implementation of the Othello game (Reversi) on command line interface. 

Possibility to play with 2 human players, or with a machine or just see 2 machines fight each other. 

## Installation

```bash
make
./bin/main <number_of_robot_player>
```

When starting up, you have the possibility to select how many robots you want to have. You can select the parameter with an integer value as 0, 1 or 2. If no parameter, the default behavior is 1.
- 0 : 2 human players
- 1 : 1 robot and 1 human
- 2 : 2 robot players 

## Implemented algorithms

For the machine behavior, we implemented a min-max algorithm with alpha-beta pruning. 

We also have a bitboard representation for the board game. Here, we have 2 bits of 64 bits (board of 8 x 8). One for the white player and another for the black player.


## Team
- Régis GRAPTIN
- Guillaume SONNET

