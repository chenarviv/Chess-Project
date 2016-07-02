#ifndef MINMAX_H_
#define MINMAX_H_


#include "ChessGame.h"
#include "Moves.h"

#define MAX_MOVES 1000000

void freeStatic(void);
int game_board_score(char board[BOARD_SIZE][BOARD_SIZE], char* currentColor);
int rate_pieces(char board[BOARD_SIZE][BOARD_SIZE], char *color);
int rate_position(char board[BOARD_SIZE][BOARD_SIZE], char *color, int pieces);
int num_of_possible_moves(char board[BOARD_SIZE][BOARD_SIZE], char *color);
int rate_moveability(char board[BOARD_SIZE][BOARD_SIZE], char *color);
int best_scoring_func(char board[BOARD_SIZE][BOARD_SIZE], char *color, char *enemy_color);
struct bestMovesForBoard* getMyBest();
int getScoreCommand_alphaBeta_maximize(char board[BOARD_SIZE][BOARD_SIZE], int alpha, int beta, int maxDepth, int currentDepth, char* currentColor, MovesList* myBestMove, int best, MovesList* userMove, int isFirst);
int getScoreCommand_alphaBeta_minimize(char board[BOARD_SIZE][BOARD_SIZE], int alpha, int beta, int maxDepth, int currentDepth, char* currentColor, MovesList* myBestMove, int best, MovesList* userMove, int isFirst);
int alphaBeta_maximize(char board[BOARD_SIZE][BOARD_SIZE], int alpha, int beta, int maxDepth, int currentDepth, char* currentColor, MovesList* myBestMove, int best, int flagBestMoves, struct bestMovesForBoard* bestMovesList);
int alphaBeta_minimize(char board[BOARD_SIZE][BOARD_SIZE], int alpha, int beta, int maxDepth, int currentDepth, char* currentColor, MovesList* myBestMove, int best, int flagBestMoves, struct bestMovesForBoard* bestMovesList);


#endif 