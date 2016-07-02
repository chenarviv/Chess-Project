#ifndef CHESSPROG_H_
#define CHESSPROG_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> 
#include <ctype.h>


#define WHITE_P 'm'
#define WHITE_B 'b'
#define WHITE_N 'n'
#define WHITE_R 'r'
#define WHITE_Q 'q'
#define WHITE_K 'k'

#define BLACK_P 'M'
#define BLACK_B 'B'
#define BLACK_N 'N'
#define BLACK_R 'R'
#define BLACK_Q 'Q'
#define BLACK_K 'K'

#define EMPTY ' '

#define BOARD_SIZE 8

/* board initialization*/
#define ENTER_SETTINGS "Enter game settings:\n" 
#define WRONG_GAME_MODE "Wrong game mode\n"
#define TWO_PLAYERS_GAME_MODE "Running game in 2 players mode\n"
#define PLAYER_VS_AI_GAME_MODE "Running game in player vs. AI mode\n"
#define WRONG_MINIMAX_DEPTH "Wrong value for minimax depth. The value should be between 1 to 4\n"
#define WRONG_FILE_NAME "Wrong file name\n"
#define WRONG_POSITION "Invalid position on the board\n"
#define NO_PIECE "Setting this piece creates an invalid board\n"   
#define WROND_BOARD_INITIALIZATION "Wrong board initialization\n"

#define ILLEGAL_COMMAND "Illegal command, please try again\n"
#define ILLEGAL_MOVE "Illegal move\n"

#define WRONG_ROOK_POSITION "Wrong position for a rook\n" 
#define ILLEGAL_CALTLING_MOVE "Illegal castling move\n"  

#define TIE "The game ends in a tie\n"

#define perror_message(func_name) (perror("Error: standard function %s has failed", func_name))
#define print_message(message) (printf("%s", message))


struct Move{
	int col;
	int row;
	char becomeTo; /*if the piece's type become other type it is the char of the new type, otherwise it is d=default*/
	struct Move* next;
} typedef Move;


struct MovesList{
	struct Move* start;
	struct Move* end;
} typedef MovesList;





struct bestMoveForPosition{
	struct MovesList* bestForPosition;
	struct bestMoveForPosition* next;
};

struct bestMovesForBoard{
	struct bestMoveForPosition* head;
	struct bestMoveForPosition* tail;
} typedef bestMovesForBoard;











/*prototypes:*/
struct bestMovesForBoard* getMyBest(void);
int getStaticCurrentTurn(void);
int isFileExists(char *xmlFileName);
void init_board(char board[BOARD_SIZE][BOARD_SIZE]);
void print_line();
void print_board(char board[BOARD_SIZE][BOARD_SIZE]);
int is_white_piece(char board[BOARD_SIZE][BOARD_SIZE], int j, int i);
int is_black_piece(char board[BOARD_SIZE][BOARD_SIZE], int j, int i);
int check_piece_color(char board[BOARD_SIZE][BOARD_SIZE], int j, int i, char* color);
int is_valid_position_on_board(int j, int i);
char *getSlotFileName(int slot);
int load_game(char board[BOARD_SIZE][BOARD_SIZE], int* inputMode, int* inputDifficulty, int* inputBest, int* inputUserColor, int* inputCurrentTurn, char* inputFileName);
void save_game(char board[BOARD_SIZE][BOARD_SIZE], int mode, int difficulty, int best, int color, char* inputFileName);
int amount_of_piece(char p, char board[BOARD_SIZE][BOARD_SIZE]);
void set(char board[BOARD_SIZE][BOARD_SIZE], int j, int i, char* color, char* type,int console);
void remove_disc(char board[BOARD_SIZE][BOARD_SIZE], int j, int i);
void game_state_twoPlayers(char board[BOARD_SIZE][BOARD_SIZE]);
void game_state_playerVsAI(char board[BOARD_SIZE][BOARD_SIZE]);
void settings_state(char board[BOARD_SIZE][BOARD_SIZE]);
void game_on_console();

#endif