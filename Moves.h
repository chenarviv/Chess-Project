#ifndef MOVES_H_
#define MOVES_H_

#include "ChessGame.h"


struct toFreeNode{
	void * pointer;
	struct toFreeNode *next;
};

typedef struct toFreeNode toFreeNode;

/*
* a list of all data yet to be freed
*/
struct toFreeList{
	toFreeNode *head;
};

typedef struct toFreeList toFreeList;
toFreeList *useSpace;
/*prototypes:*/
Move* init_new_move(int j, int i);
MovesList* init_new_movesList(int j, int i);
MovesList* init_new_movesList_forGui(int j, int i);
int is_empty_movesList(MovesList* moves);
void free_movesList(MovesList* moves);
MovesList* init_new_movesList_for_bestMoves(int j, int i, int endj, int endi);
struct bestMoveForPosition* init_new_bestMoveForPosition(int j, int i, int endj, int endi);
struct bestMovesForBoard* init_new_bestMovesForBoard(int j, int i);
void free_bestMoveForPosition(struct bestMoveForPosition* bestMovePos);
void free_bestMovesForBoard(struct bestMovesForBoard* bestForBoard);
void freeToFreeNode(toFreeNode * curToFreeNode);
void freeUsedList();
void reInitUsedList();
void addToUserUsedList(void * allocated);
char* charToString_type(char c);
char stringToChar_type(char* type, char* pieceColor);
void print_movesList(MovesList* moves);
void print_bestMovesList(struct bestMovesForBoard* moves);
void change_board_one_move(char board[BOARD_SIZE][BOARD_SIZE], char newBoard[BOARD_SIZE][BOARD_SIZE], int fromCol, int fromRow, Move* moveToDo);
void get_moves_bishop_rook_queen_by_numSteps(char board[BOARD_SIZE][BOARD_SIZE], MovesList* moves, int j, int i, int j_numSteps, int i_numSteps, char* pieceColor);
void get_moves_knight_king_by_numSteps(char board[BOARD_SIZE][BOARD_SIZE], MovesList* moves, int j, int i, int j_numSteps, int i_numSteps, char* pieceColor);
void get_moves_pawn_by_numSteps(char board[BOARD_SIZE][BOARD_SIZE], MovesList* moves, int j, int i, int j_numSteps, int i_numSteps);
void get_moves_pawn(char board[BOARD_SIZE][BOARD_SIZE], MovesList* moves, int j, int i);
void get_moves_bishop(char board[BOARD_SIZE][BOARD_SIZE], MovesList* moves, int j, int i, char* pieceColor);
void get_moves_knight(char board[BOARD_SIZE][BOARD_SIZE], MovesList* moves, int j, int i, char* pieceColor);
void get_moves_rook(char board[BOARD_SIZE][BOARD_SIZE], MovesList* moves, int j, int i, char* pieceColor);
void get_moves_queen(char board[BOARD_SIZE][BOARD_SIZE], MovesList* moves, int j, int i, char* pieceColor);
void get_moves_king(char board[BOARD_SIZE][BOARD_SIZE], MovesList* moves, int j, int i, char* pieceColor);
void get_moves_for_piece(char board[BOARD_SIZE][BOARD_SIZE], MovesList* moves, int j, int i);
void delete_king_threatenning_moves(char board[BOARD_SIZE][BOARD_SIZE], MovesList* userMoves);
void get_possible_moves_for_piece(char board[BOARD_SIZE][BOARD_SIZE], MovesList* moves, int j, int i);
void print_possible_moves_for_piece(char board[BOARD_SIZE][BOARD_SIZE], int j, int i, char* pieceColor);
int create_userMove(char board[BOARD_SIZE][BOARD_SIZE], MovesList* moves, char* pieceColor);
int create_userMove_for_gui(char board[BOARD_SIZE][BOARD_SIZE], MovesList* moves);
int canPromotePawn(char * color, int y);
char * getOpositeColor(char * color);
int is_legal_move(char board[BOARD_SIZE][BOARD_SIZE], MovesList* userMove, char* currentTurnColor, int console);
int areTherePossibleMoves(char board[BOARD_SIZE][BOARD_SIZE], char* currentColor);
#endif