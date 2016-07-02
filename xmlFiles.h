#ifndef XMLFILES_H_
#define XMLFILES_H_

#include "chessprog.h"
#include <stdio.h>
#include <string.h>
#include <math.h> 
#include <stdlib.h>
#include <ctype.h>

FILE* create_xml_file(char* fileName);
void write_game_to_xml_file(char board[BOARD_SIZE][BOARD_SIZE], FILE* xmlFile, int mode, int difficulty, int best, char* userColor, char* nextTurn);
char* xml_file_to_string(char* xmlFileName);
char* get_turn_mode_difficulty_color(char* xmlFileString, char* field);
void xmlRow_to_consoleRow(char board[BOARD_SIZE][BOARD_SIZE], char* xmlFileString, char* field, int row);
void xmlBoard_to_consoleBoard(char board[BOARD_SIZE][BOARD_SIZE], char* xmlFileString);

#endif