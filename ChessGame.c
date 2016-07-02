#include "ChessGame.h"
#include "Moves.h"


int check_if_enemy_can_eat(char board[BOARD_SIZE][BOARD_SIZE], int j, int i, int j_numSteps, int i_numSteps, char direction){
	/* returns 1 if there's an enemy's piece in the given direction that can eat our piece in (j,i), and 0 otherwise */
	int isIncrement;
	char* ourPieceColor;

	if (check_piece_color(board, j, i, "white") == 1){
		ourPieceColor = "white";
	}
	else{
		ourPieceColor = "black";
	}

	isIncrement = 0;

	while (is_valid_position_on_board(j + j_numSteps, i + i_numSteps) == 1){  /*loop until finding a piece (suspect that may eat)*/
		if (board[j + j_numSteps][i + i_numSteps] == EMPTY && direction != 'L'){
			j += j_numSteps;
			i += i_numSteps;
			isIncrement = 1;
		}
		else break;
	}

	if (is_valid_position_on_board(j + j_numSteps, i + i_numSteps) == 1 && board[j + j_numSteps][i + i_numSteps] != EMPTY){ /*there is a piece in the way*/
		if (strcmp(ourPieceColor, "black") == 0){	/*our piece's color is black - check if white can eat*/
			if (direction == 's'){	/*straight*/
				if (board[j + j_numSteps][i + i_numSteps] == WHITE_Q || board[j + j_numSteps][i + i_numSteps] == WHITE_R)
					return 1;
				else if (isIncrement == 0 && board[j + j_numSteps][i + i_numSteps] == WHITE_K)
					return 1;
			}
			else if (direction == 'd'){ /*diagonal*/
				if (board[j + j_numSteps][i + i_numSteps] == WHITE_Q || board[j + j_numSteps][i + i_numSteps] == WHITE_B)
					return 1;
				else if (isIncrement == 0 && ((board[j - 1][i - 1] == WHITE_P) || (board[j + 1][i - 1] == WHITE_P) || board[j + j_numSteps][i + i_numSteps] == WHITE_K))
					return 1;
			}
			else if (direction == 'L' && isIncrement == 0){	/*knight*/
				if (board[j + j_numSteps][i + i_numSteps] == WHITE_N)
					return 1;
				else return 0;
			}
		}
		else{	/*our piece's color is white - check if black can eat*/
			if (direction == 's'){	/*straight*/
				if (board[j + j_numSteps][i + i_numSteps] == BLACK_Q || board[j + j_numSteps][i + i_numSteps] == BLACK_R)
					return 1;
				else if (isIncrement == 0 && board[j + j_numSteps][i + i_numSteps] == BLACK_K)
					return 1;
			}
			else if (direction == 'd'){	/*diagonal*/
				if (board[j + j_numSteps][i + i_numSteps] == BLACK_Q || board[j + j_numSteps][i + i_numSteps] == BLACK_B)
					return 1;
				else if (isIncrement == 0 && ((board[j - 1][i + 1] == BLACK_P) || (board[j + 1][i + 1] == BLACK_P) || board[j + j_numSteps][i + i_numSteps] == BLACK_K))
					return 1;
			}
			else if (direction == 'L' && isIncrement == 0){	/*knight*/
				if (board[j + j_numSteps][i + i_numSteps] == BLACK_N)
					return 1;
				else return 0;
			}

		}
	}
	return 0;
}


int check_if_color_can_Check(char board[BOARD_SIZE][BOARD_SIZE], char* color){
	/* returns 1 if the given color can make Check on his enemy, 0 otherwise */

	int i, j;
	int i_enemyKing = -1; /*i index of enemy's king*/
	int j_enemyKing = -1; /*j index of enemy's king*/
	char enemyKingChar = '@'; /*some initialization*/
	int isCheck = 0;

	if (strcmp(color, "white") == 0){
		enemyKingChar = BLACK_K;
	}
	else if (strcmp(color, "black") == 0){
		enemyKingChar = WHITE_K;
	}

	for (j = 0; j < BOARD_SIZE; ++j){	/*loop over the whole game board to find the position of the enemy's king*/
		for (i = 0; i < BOARD_SIZE; ++i){

			if (board[j][i] == enemyKingChar){
				j_enemyKing = j;
				i_enemyKing = i;
				break;
			}
			if (j_enemyKing != -1)
				break;
		}
	}

	if (j_enemyKing == -1)	/*no enemy's king was found*/
		return 1;

	else{	/*check all possible options for Check*/
		isCheck = check_if_enemy_can_eat(board, j_enemyKing, i_enemyKing, 0, 1, 's');
		if (isCheck == 1) return 1;
		isCheck = check_if_enemy_can_eat(board, j_enemyKing, i_enemyKing, 0, -1, 's');
		if (isCheck == 1) return 1;
		isCheck = check_if_enemy_can_eat(board, j_enemyKing, i_enemyKing, -1, 0, 's');
		if (isCheck == 1) return 1;
		isCheck = check_if_enemy_can_eat(board, j_enemyKing, i_enemyKing, 1, 0, 's');
		if (isCheck == 1) return 1;
		isCheck = check_if_enemy_can_eat(board, j_enemyKing, i_enemyKing, -1, -1, 'd');
		if (isCheck == 1) return 1;
		isCheck = check_if_enemy_can_eat(board, j_enemyKing, i_enemyKing, -1, 1, 'd');
		if (isCheck == 1) return 1;
		isCheck = check_if_enemy_can_eat(board, j_enemyKing, i_enemyKing, 1, -1, 'd');
		if (isCheck == 1) return 1;
		isCheck = check_if_enemy_can_eat(board, j_enemyKing, i_enemyKing, 1, 1, 'd');
		if (isCheck == 1) return 1;
		isCheck = check_if_enemy_can_eat(board, j_enemyKing, i_enemyKing, -1, -2, 'L');
		if (isCheck == 1) return 1;
		isCheck = check_if_enemy_can_eat(board, j_enemyKing, i_enemyKing, -1, 2, 'L');
		if (isCheck == 1) return 1;
		isCheck = check_if_enemy_can_eat(board, j_enemyKing, i_enemyKing, 1, -2, 'L');
		if (isCheck == 1) return 1;
		isCheck = check_if_enemy_can_eat(board, j_enemyKing, i_enemyKing, 1, 2, 'L');
		if (isCheck == 1) return 1;
		isCheck = check_if_enemy_can_eat(board, j_enemyKing, i_enemyKing, -2, -1, 'L');
		if (isCheck == 1) return 1;
		isCheck = check_if_enemy_can_eat(board, j_enemyKing, i_enemyKing, -2, 1, 'L');
		if (isCheck == 1) return 1;
		isCheck = check_if_enemy_can_eat(board, j_enemyKing, i_enemyKing, 2, -1, 'L');
		if (isCheck == 1) return 1;
		isCheck = check_if_enemy_can_eat(board, j_enemyKing, i_enemyKing, 2, 1, 'L');
		if (isCheck == 1) return 1;

		return 0;
	}
}


int check_if_color_can_Mate(char board[BOARD_SIZE][BOARD_SIZE], char* color){
	/* returns 1 if the given color can make Mate on his enemy (=WIN), and 0 otherwise.
	the pre-condition is: the given color has now made "Check", and now we check if the enemy can save his king.
	(Mate means: color made Check on the enemy's king, and the enemy can't save him - no possible moves for him) */

	int i, j;
	struct Move* currentMove;
	struct MovesList* moves;
	char* enemyColor;
	char newBoard[BOARD_SIZE][BOARD_SIZE];


	if (strcmp(color, "white") == 0){
		enemyColor = "black";
	}
	else{
		enemyColor = "white";
	}

	if (check_if_color_can_Check(board, color) == 0){ /*first we check if the color threats the enemy's king */
		return 0;
	}

	for (j = 0; j < BOARD_SIZE; ++j){
		for (i = 0; i < BOARD_SIZE; ++i){ /*go over the whole game board*/
			if (check_piece_color(board, j, i, enemyColor) == 1){ /*go over every piece of the enemy and get his moves*/
				moves = init_new_movesList(-1, -1);
				get_moves_for_piece(board, moves, j, i);
				if (moves->start->next != NULL){
					for (currentMove = moves->start->next; currentMove != (moves->end)->next; currentMove = currentMove->next) { /*loop to go over each move*/
						change_board_one_move(board, newBoard, moves->start->col, moves->start->row, currentMove);
						if (check_if_color_can_Check(newBoard, color) == 0) {/*if the enemy can save his king*/
							free_movesList(moves);
							free(moves);
							return 0;
						}
					}
				}
				free_movesList(moves);
				free(moves);
			}
		}
	}
	return 1; /*Mate: color wins - the enemy's king can't be saved*/
}

