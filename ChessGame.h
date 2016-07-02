#ifndef CHESSGAME_H_
#define CHESSGAME_H_

#include "chessprog.h"

int check_if_enemy_can_eat(char board[BOARD_SIZE][BOARD_SIZE], int j, int i, int j_numSteps, int i_numSteps, char direction);
int check_if_color_can_Check(char board[BOARD_SIZE][BOARD_SIZE], char* color);
int check_if_color_can_Mate(char board[BOARD_SIZE][BOARD_SIZE], char* color);
int check_if_color_make_tie(char board[BOARD_SIZE][BOARD_SIZE], char* color);

#endif