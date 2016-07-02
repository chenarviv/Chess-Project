#include "minmax.h"

struct bestMovesForBoard* bestMovesListForFile;
int whitePawnForBest[BOARD_SIZE][BOARD_SIZE] = { { 0, 0, 0, 0, 0, 0, 0, 0 }, { 5, 10, 10, -20, -20, 10, 10, 5 }, { 5, -5, -10, 0, 0, -10, -5, 5 }, { 0, 0, 0, 20, 20, 0, 0, 0 }, { 5, 5, 10, 25, 25, 10, 5, 5 }, { 10, 10, 20, 30, 30, 20, 10, 10 }, { 50, 50, 50, 50, 50, 50, 50, 50 }, { 0, 0, 0, 0, 0, 0, 0, 0 } };
int blackPawnForBest[BOARD_SIZE][BOARD_SIZE] = { { 0, 0, 0, 0, 0, 0, 0, 0 }, { 50, 50, 50, 50, 50, 50, 50, 50 }, { 10, 10, 20, 30, 30, 20, 10, 10 }, { 5, 5, 10, 25, 25, 10, 5, 5 }, { 0, 0, 0, 20, 20, 0, 0, 0 }, { 5, -5, -10, 0, 0, -10, -5, 5 }, { 5, 10, 10, -20, -20, 10, 10, 5 }, { 0, 0, 0, 0, 0, 0, 0, 0 } };
int whiteKnightForBest[BOARD_SIZE][BOARD_SIZE] = { { -50, -40, -30, -30, -30, -30, -40, -50 }, { -40, -20, 0, 5, 5, 0, -20, -40 }, { -30, 5, 10, 15, 15, 10, 5, -30 }, { -30, 0, 15, 20, 20, 15, 0, -30 }, { -30, 5, 15, 20, 20, 15, 5, -30 }, { -30, 0, 10, 15, 15, 10, 0, -30 }, { -40, -20, 0, 0, 0, 0, -20, -40 }, { -50, -40, -30, -30, -30, -30, -40, -50 } };
int blackKnightForBest[BOARD_SIZE][BOARD_SIZE] = { { -50, -40, -30, -30, -30, -30, -40, -50 }, { -40, -20, 0, 0, 0, 0, -20, -40 }, { -30, 0, 10, 15, 15, 10, 0, -30 }, { -30, 5, 15, 20, 20, 15, 5, -30 }, { -30, 0, 15, 20, 20, 15, 0, -30 }, { -30, 5, 10, 15, 15, 10, 5, -30 }, { -40, -20, 0, 5, 5, 0, -20, -40 }, { -50, -40, -30, -30, -30, -30, -40, -50 } };
int whiteQueenForBest[BOARD_SIZE][BOARD_SIZE] = { { -20, -10, -10, -5, -5, -10, -10, -20 }, { -10, 0, 5, 0, 0, 0, 0, -10 }, { -10, 5, 5, 5, 5, 5, 0, -10 }, { 0, 0, 5, 5, 5, 5, 0, -5 }, { -5, 0, 5, 5, 5, 5, 0, -5 }, { -10, 0, 5, 5, 5, 5, 0, -10 }, { -10, 0, 0, 0, 0, 0, 0, -10 }, { -20, -10, -10, -5, -5, -10, -10, -20 } };
int blackQueenForBest[BOARD_SIZE][BOARD_SIZE] = { { -20, -10, -10, -5, -5, -10, -10, -20 }, { -10, 0, 0, 0, 0, 0, 0, -10 }, { -10, 5, 5, 5, 5, 5, 0, -10 }, { -5, 0, 5, 5, 5, 5, 0, -5 }, { 0, 0, 5, 5, 5, 5, 0, -5 }, { -10, 5, 5, 5, 5, 5, 0, -10 }, { -10, 0, 5, 0, 0, 0, 0, -10 }, { -20, -10, -10, -5, -5, -10, -10, -20 } };
int whiteBishopForBest[BOARD_SIZE][BOARD_SIZE] = { { -20, -10, -10, -10, -10, -10, -10, -20 }, { -10, 5, 0, 0, 0, 0, 5, -10 }, { -10, 10, 10, 10, 10, 10, 10, -10 }, { -10, 0, 10, 10, 10, 10, 0, -10 }, { -10, 5, 5, 10, 10, 5, 5, -10 }, { -10, 0, 5, 10, 10, 5, 0, -10 }, { -10, 0, 0, 0, 0, 0, 0, -10 }, { -20, -10, -10, -10, -10, -10, -10, -20 } };
int blackBishopForBest[BOARD_SIZE][BOARD_SIZE] = { { -20, -10, -10, -10, -10, -10, -10, -20 }, { -10, 0, 0, 0, 0, 0, 0, -10 }, { -10, 0, 5, 10, 10, 5, 0, -10 }, { -10, 5, 5, 10, 10, 5, 5, -10 }, { -10, 0, 10, 10, 10, 10, 0, -10 }, { -10, 10, 10, 10, 10, 10, 10, -10 }, { -10, 5, 0, 0, 0, 0, 5, -10 }, { -20, -10, -10, -10, -10, -10, -10, -20 } };
int whiteRookForBest[BOARD_SIZE][BOARD_SIZE] = { { 0, 0, 0, 5, 5, 0, 0, 0 }, { -5, 0, 0, 0, 0, 0, 0, -5 }, { -5, 0, 0, 0, 0, 0, 0, -5 }, { -5, 0, 0, 0, 0, 0, 0, -5 }, { -5, 0, 0, 0, 0, 0, 0, -5 }, { -5, 0, 0, 0, 0, 0, 0, -5 }, { 5, 10, 10, 10, 10, 10, 10, 5 }, { 0, 0, 0, 0, 0, 0, 0, 0 } };
int blackRookForBest[BOARD_SIZE][BOARD_SIZE] = { { 0, 0, 0, 0, 0, 0, 0, 0 }, { 5, 10, 10, 10, 10, 10, 10, 5 }, { -5, 0, 0, 0, 0, 0, 0, -5 }, { -5, 0, 0, 0, 0, 0, 0, -5 }, { -5, 0, 0, 0, 0, 0, 0, -5 }, { -5, 0, 0, 0, 0, 0, 0, -5 }, { -5, 0, 0, 0, 0, 0, 0, -5 }, { 0, 0, 0, 5, 5, 0, 0, 0 } };
int whiteKingForBestStart[BOARD_SIZE][BOARD_SIZE] = { { 20, 30, 10, 0, 0, 10, 30, 20 }, { 20, 20, 0, 0, 0, 0, 20, 20 }, { -10, -20, -20, -20, -20, -20, -20, -10 }, { -20, -30, -30, -40, -40, -30, -30, -20 }, { -30, -40, -40, -50, -50, -40, -40, -30 }, { -30, -40, -40, -50, -50, -40, -40, -30 }, { -30, -40, -40, -50, -50, -40, -40, -30 }, { -30, -40, -40, -50, -50, -40, -40, -30 } };
int blackKingForBestStart[BOARD_SIZE][BOARD_SIZE] = { { -30, -40, -40, -50, -50, -40, -40, -30 }, { -30, -40, -40, -50, -50, -40, -40, -30 }, { -30, -40, -40, -50, -50, -40, -40, -30 }, { -30, -40, -40, -50, -50, -40, -40, -30 }, { -20, -30, -30, -40, -40, -30, -30, -20, }, { -10, -20, -20, -20, -20, -20, -20, -10 }, { 20, 20, 0, 0, 0, 0, 20, 20 }, { 20, 30, 10, 0, 0, 10, 30, 20 } };
int blackKingForBestEnd[BOARD_SIZE][BOARD_SIZE] = { { -50, -40, -30, -20, -20, -30, -40, -50 }, { -30, -20, -10, 0, 0, -10, -20, -30 }, { -30, -10, 20, 30, 30, 20, -10, -30 }, { -30, -10, 30, 40, 40, 30, -10, -30 }, { -30, -10, 30, 40, 40, 30, -10, -30 }, { -30, -10, 20, 30, 30, 20, -10, -30 }, { -30, -30, 0, 0, 0, 0, -30, -30 }, { -50, -30, -30, -30, -30, -30, -30, -50 } };
int whiteKingForBestEnd[BOARD_SIZE][BOARD_SIZE] = { { -50, -30, -30, -30, -30, -30, -30, -50 }, { -30, -30, 0, 0, 0, 0, -30, -30 }, { -30, -10, 20, 30, 30, 20, -10, -30 }, { -30, -10, 30, 40, 40, 30, -10, -30 }, { -30, -10, 30, 40, 40, 30, -10, -30, }, { -30, -10, 20, 30, 30, 20, -10, -30 }, { -30, -20, -10, 0, 0, -10, -20, -30 }, { -50, -40, -30, -20, -20, -30, -40, -50 } };


int game_board_score(char board[BOARD_SIZE][BOARD_SIZE], char* currentColor){
	/* the scoring function of the game board - returns the score of the current game board */

	int i, j;
	int score = 0;
	int currentTurn;
	int isWhiteColor;
	char* enemyColor;

	currentTurn = getStaticCurrentTurn();

	if (currentTurn == 0){
		currentColor = "white";
		enemyColor = "black";
		isWhiteColor = 1;

	}
	else if (currentTurn == 1){
		currentColor = "black";
		enemyColor = "white";
		isWhiteColor = 0;

	}


	if (check_if_color_can_Mate(board, currentColor) == 1){ /*check if current color wins*/
		return 100000;
	}

	if (check_if_color_can_Mate(board, enemyColor) == 1){ /*check if enemy color wins*/
		return -100000;
	}

	if (areTherePossibleMoves(board, currentColor) == 0){ /*check tie*/
		if (check_if_color_can_Check(board, enemyColor) == 0){
			return 0;
		}
	}

	if (areTherePossibleMoves(board, enemyColor) == 0){ /*check tie*/
		if (check_if_color_can_Check(board, currentColor) == 0){
			return 0;
		}
	}

	for (i = 0; i < BOARD_SIZE; i++){	/*go over the whole game board and calculate the score*/
		for (j = 0; j < BOARD_SIZE; j++){

			if ((board[j][i]) == WHITE_P){
				score++;
			}
			else if (board[j][i] == WHITE_N || board[j][i] == WHITE_B){
				score += 3;
			}
			else if (board[j][i] == WHITE_R){
				score += 5;
			}
			else if (board[j][i] == WHITE_Q){
				score += 9;
			}
			else if (board[j][i] == WHITE_K){
				score += 400;
			}
			else if (board[j][i] == BLACK_P){
				score--;
			}
			else if (board[j][i] == BLACK_N || board[j][i] == BLACK_B){
				score -= 3;
			}
			else if (board[j][i] == BLACK_R){
				score -= 5;
			}
			else if (board[j][i] == BLACK_Q){
				score -= 9;
			}
			else if (board[j][i] == BLACK_K){
				score -= 400;
			}
		}
	}

	if (isWhiteColor == 0){
		score = score*(-1);
	}
	return score;
}


int rate_pieces_for_best(char board[BOARD_SIZE][BOARD_SIZE], char *color) {
	int i, j;

	int counter = 0;
	if (strcmp(color, "black") == 0){
		for (j = 0; j < BOARD_SIZE; ++j){
			for (i = 0; i < BOARD_SIZE; ++i){
				/*sum all the color's and enemy's pawns/kings*/
				if (board[j][i] == BLACK_Q)
					counter += 900;
				else if (board[j][i] == BLACK_N)
					counter += 300;
				else if (board[j][i] == BLACK_B)
					counter += 300;
				else if (board[j][i] == BLACK_R)
					counter += 500;
				else if (board[j][i] == BLACK_P)
					counter += 100;
			}
		}
	}
	else{
		for (j = 0; j < BOARD_SIZE; ++j){
			for (i = 0; i < BOARD_SIZE; ++i){
				/*sum all the color's and enemy's pawns/kings*/
				if (board[j][i] == WHITE_Q)
					counter += 900;
				else if (board[j][i] == WHITE_N)
					counter += 300;
				else if (board[j][i] == WHITE_B)
					counter += 300;
				else if (board[j][i] == WHITE_R)
					counter += 500;
				else if (board[j][i] == WHITE_P)
					counter += 100;
			}
		}
	}

	return counter;
}



int num_of_possible_moves(char board[BOARD_SIZE][BOARD_SIZE], char *color){
	int num_of_moves = 0;
	MovesList* moves;
	Move *move;
	int j, i;
	for (j = 0; j < BOARD_SIZE; ++j){
		for (i = 0; i < BOARD_SIZE; ++i){
			if (check_piece_color(board, j, i, color) == 1){/*for each of the color's pawn/king check if it can move*/
				moves = init_new_movesList(j, i);
				get_moves_for_piece(board, moves, j, i);
				delete_king_threatenning_moves(board, moves);
				move = moves->start;
				while (move != moves->end){
					move = move->next;
					num_of_moves++;
				}
				free_movesList(moves);
				free(moves);
			}
		}
	}
	return num_of_moves;
}

int rate_moveability(char board[BOARD_SIZE][BOARD_SIZE], char *color){
	int counter = 0;
	counter += num_of_possible_moves(board, color);
	return counter;
}



int rate_position(char board[BOARD_SIZE][BOARD_SIZE], char *color, int pieces){
	int i, j;
	int counter = 0;
	if (strcmp(color, "black") == 0){
		for (j = 0; j < BOARD_SIZE; ++j){
			for (i = 0; i < BOARD_SIZE; ++i){
				/*sum all the color's and enemy's pawns/kings*/
				if (board[j][i] == BLACK_Q)
					counter += blackQueenForBest[j][i];
				else if (board[j][i] == BLACK_N)
					counter += blackKnightForBest[j][i];
				else if (board[j][i] == BLACK_B)
					counter += blackBishopForBest[j][i];
				else if (board[j][i] == BLACK_R)
					counter += blackRookForBest[j][i];
				else if (board[j][i] == BLACK_P)
					counter += blackPawnForBest[j][i];
				else if (board[j][i] == BLACK_K){
					if (pieces >= 1750){
						counter += blackKingForBestStart[j][i];
					}
					else {
						counter += blackKingForBestEnd[j][i];
					}
				}
			}
		}
	}
	else{
		for (j = 0; j < BOARD_SIZE; ++j){
			for (i = 0; i < BOARD_SIZE; ++i){
				/*sum all the color's and enemy's pawns/kings*/
				if (board[j][i] == WHITE_Q)
					counter += whiteQueenForBest[j][i];
				else if (board[j][i] == WHITE_N)
					counter += whiteKnightForBest[j][i];
				else if (board[j][i] == WHITE_B)
					counter += whiteBishopForBest[j][i];
				else if (board[j][i] == WHITE_R)
					counter += whiteRookForBest[j][i];
				else if (board[j][i] == WHITE_P)
					counter += whitePawnForBest[j][i];
				else if (board[j][i] == WHITE_K){
					if (pieces >= 1750){
						counter += whiteKingForBestStart[j][i];
					}
					else {
						counter += whiteKingForBestEnd[j][i];
					}
				}
			}
		}
	}

	return counter;
}



int best_scoring_func(char board[BOARD_SIZE][BOARD_SIZE], char *color, char *enemy_color){
	/*best - scoring func*/
	int counter = 0;
	int count_moveability_color;
	int count_moveability_enemycolor;
	int piecesRating = rate_pieces_for_best(board, color);
	count_moveability_color = rate_moveability(board, color);
	count_moveability_enemycolor = rate_moveability(board, enemy_color);
	if (count_moveability_color == 0){
		return -200000;
	}
	if (count_moveability_enemycolor == 0){
		return 200000;
	}
	counter += piecesRating;
	counter += count_moveability_color;
	counter += rate_position(board, color, piecesRating);
	piecesRating = rate_pieces_for_best(board, enemy_color);
	counter -= piecesRating;
	counter -= count_moveability_enemycolor;
	counter -= rate_position(board, enemy_color, piecesRating);
	return counter;
}


struct bestMovesForBoard* getMyBest(void){
	return bestMovesListForFile;
}

int getScoreCommand_alphaBeta_maximize(char board[BOARD_SIZE][BOARD_SIZE], int alpha, int beta, int maxDepth, int currentDepth, char* currentColor, MovesList* myBestMove, int best, MovesList* userMove, int isFirst){
	int i, j, score = 0;
	Move* move;
	MovesList* moves;
	char* enemyColor;
	char newBoard[BOARD_SIZE][BOARD_SIZE];

	if (strcmp(currentColor, "black") == 0) {
		enemyColor = "white";
	}
	else {
		enemyColor = "black";
	}

	if (currentDepth == maxDepth) {		/*last level*/
		if (best == 0){
			return game_board_score(board, currentColor);
		}
		else{
			return best_scoring_func(board, currentColor, enemyColor);
		}
	}

	if (isFirst == 0){ /*if not the first level*/
		for (j = 0; j < BOARD_SIZE; ++j){	/*go over the whole game board*/
			for (i = 0; i < BOARD_SIZE; ++i){
				if (check_piece_color(board, j, i, currentColor) == 1){ /*if (j,i) contains a piece of the current color*/
					moves = init_new_movesList(j, i);
					get_moves_for_piece(board, moves, j, i); /*get moves for the current piece*/
					delete_king_threatenning_moves(board, moves); /*delete illegal moves*/
					move = moves->start;

					while (move != moves->end){ /*go over each legal move of the current piece*/
						move = move->next;
						change_board_one_move(board, newBoard, j, i, move);
						score = getScoreCommand_alphaBeta_minimize(newBoard, alpha, beta, maxDepth, currentDepth + 1, enemyColor, myBestMove, best, userMove, 0); /*call to minimize*/


						if (alpha < score){
							alpha = score;
							if (currentDepth == 0){  /*update best move*/
								(myBestMove)->start->col = j;
								(myBestMove)->start->row = i;
								(myBestMove)->end->col = move->col;
								(myBestMove)->end->row = move->row;
								(myBestMove)->end->becomeTo = move->becomeTo;
							}
						}
						if (beta <= alpha){ /*pruning */
							free_movesList(moves);
							free(moves);
							return alpha;
						}
					}
					free_movesList(moves);
					free(moves);
				}
			}
		}
		return alpha;
	}
	else{ /*the first level*/
		change_board_one_move(board, newBoard, userMove->start->col, userMove->start->row, userMove->end);
		score = getScoreCommand_alphaBeta_minimize(newBoard, alpha, beta, maxDepth, currentDepth + 1, enemyColor, myBestMove, best, userMove, 0);
		if (alpha < score){
			alpha = score;
		}
		/**if (beta <= alpha){
		free_movesList(moves);
		free(moves);
		return alpha;
		}

		**/
	}
	return alpha;
}




int getScoreCommand_alphaBeta_minimize(char board[BOARD_SIZE][BOARD_SIZE], int alpha, int beta, int maxDepth, int currentDepth, char* currentColor, MovesList* myBestMove, int best, MovesList* userMove, int isFirst){
	/* minimizing the minimax function */

	int i, j, score;
	Move* move;
	MovesList* moves;
	char* enemyColor;
	char newBoard[BOARD_SIZE][BOARD_SIZE];

	if (strcmp(currentColor, "black") == 0) {
		enemyColor = "white";
	}
	else {
		enemyColor = "black";
	}

	if (currentDepth == maxDepth) {		/*last level*/
		if (best == 0){
			return game_board_score(board, currentColor);
		}
		else{
			return best_scoring_func(board, currentColor, enemyColor);
		}
	}

	for (j = 0; j < BOARD_SIZE; ++j){	/*go over the whole game board*/
		for (i = 0; i < BOARD_SIZE; ++i){
			if (check_piece_color(board, j, i, currentColor) == 1){	/*if (j,i) contains a piece of the current color*/
				moves = init_new_movesList(j, i);
				get_moves_for_piece(board, moves, j, i);	/*get moves for the current piece*/
				delete_king_threatenning_moves(board, moves); /*delete illegal moves*/
				move = moves->start;

				while (move != moves->end){	/*go over each legal move of the current piece*/
					move = move->next;
					change_board_one_move(board, newBoard, j, i, move);
					score = getScoreCommand_alphaBeta_maximize(newBoard, alpha, beta, maxDepth, currentDepth + 1, enemyColor, myBestMove, best, userMove, 0); /*call to maximize*/
					if (beta > score){
						beta = score;
					}
					if (beta <= alpha){	/*pruning*/
						free_movesList(moves);
						free(moves);
						return beta;
					}
				}
				free_movesList(moves);
				free(moves);
			}
		}
	}
	return beta;
}

void freeStatic(void){
	free_bestMovesForBoard(bestMovesListForFile);
	free(bestMovesListForFile);
	
}
int alphaBeta_maximize(char board[BOARD_SIZE][BOARD_SIZE], int alpha, int beta, int maxDepth, int currentDepth, char* currentColor, MovesList* myBestMove, int best,
	int flagBestMoves, struct bestMovesForBoard* bestMovesList){
	/* maximizing the minimax function */

	int i, j, score = 0;
	int scoreBeforeChange;

	Move* move;
	MovesList* moves;
	char* enemyColor;
	char newBoard[BOARD_SIZE][BOARD_SIZE];


	if (strcmp(currentColor, "black") == 0) {
		enemyColor = "white";
	}
	else {
		enemyColor = "black";
	}


	if (currentDepth == maxDepth) {		/*last level*/
		bestMovesListForFile = bestMovesList;
		if (best == 0){
			return game_board_score(board, currentColor);
		}
		else{
			return best_scoring_func(board, currentColor, enemyColor);
		}
	}

	for (j = 0; j < BOARD_SIZE; ++j){	/*go over the whole game board*/
		for (i = 0; i < BOARD_SIZE; ++i){
			if (check_piece_color(board, j, i, currentColor) == 1){ /*if (j,i) contains a piece of the current color*/
				moves = init_new_movesList(j, i);
				get_moves_for_piece(board, moves, j, i); /*get moves for the current piece*/
				delete_king_threatenning_moves(board, moves); /*delete illegal moves*/
				move = moves->start;

				while (move != moves->end){ /*go over each legal move of the current piece*/
					move = move->next;

					scoreBeforeChange = score;
					change_board_one_move(board, newBoard, j, i, move);

					score = alphaBeta_minimize(newBoard, alpha, beta, maxDepth, currentDepth + 1, enemyColor, myBestMove, best, flagBestMoves, bestMovesList); /*call to minimize*/

					if (score == 10000000){
						if (getStaticCurrentTurn() == 0){
							if (areTherePossibleMoves(newBoard, "black") == 0){
								if (check_if_color_can_Mate(newBoard, "white") == 0){
									score = scoreBeforeChange;
								}

							}
						}
						else if (getStaticCurrentTurn() == 1){

							if (areTherePossibleMoves(newBoard, "white") == 0){
								if (check_if_color_can_Mate(newBoard, "black") == 0){
									score = scoreBeforeChange;
								}
							}
						}
					}


					if (flagBestMoves == 1){ /*we are here by the "get_best_moves" command*/

						if (alpha == score){
							if (currentDepth == 0){

								(bestMovesList->tail->bestForPosition)->start->col = j;
								(bestMovesList->tail->bestForPosition)->start->row = i;
								(bestMovesList->tail->bestForPosition)->end->col = move->col;
								(bestMovesList->tail->bestForPosition)->end->row = move->row;
								(bestMovesList->tail->bestForPosition)->end->becomeTo = move->becomeTo;

								bestMovesList->tail->next = init_new_bestMoveForPosition(-1, -1, -1, -1);
								bestMovesList->tail = bestMovesList->tail->next;

								bestMovesListForFile = bestMovesList;

							}
						}
						else if (alpha < score){




							alpha = score;
							if (currentDepth == 0){  /*update best move*/
								(myBestMove)->start->col = j;
								(myBestMove)->start->row = i;
								(myBestMove)->end->col = move->col;
								(myBestMove)->end->row = move->row;
								(myBestMove)->end->becomeTo = move->becomeTo;

								free_bestMovesForBoard(bestMovesList);
								free(bestMovesList);
								bestMovesList = init_new_bestMovesForBoard(-1, -1);

								(bestMovesList->tail->bestForPosition)->start->col = j;
								(bestMovesList->tail->bestForPosition)->start->row = i;
								(bestMovesList->tail->bestForPosition)->end->col = move->col;
								(bestMovesList->tail->bestForPosition)->end->row = move->row;
								(bestMovesList->tail->bestForPosition)->end->becomeTo = move->becomeTo;

								bestMovesList->tail->next = init_new_bestMoveForPosition(-1, -1, -1, -1);
								bestMovesList->tail = bestMovesList->tail->next;
								bestMovesListForFile = bestMovesList;
							}
						}
					}
					else if (alpha < score){
						alpha = score;
						if (currentDepth == 0){  /*update best move*/
							(myBestMove)->start->col = j;
							(myBestMove)->start->row = i;
							(myBestMove)->end->col = move->col;
							(myBestMove)->end->row = move->row;
							(myBestMove)->end->becomeTo = move->becomeTo;
						}
					}

					if (beta <= alpha){ /*pruning*/
						free_movesList(moves);
						free(moves);
						return alpha;
					}
				}

				free_movesList(moves);
				free(moves);
			}
		}
	}
	return alpha;
}


int alphaBeta_minimize(char board[BOARD_SIZE][BOARD_SIZE], int alpha, int beta, int maxDepth, int currentDepth, char* currentColor, MovesList* myBestMove, int best,
	int flagBestMoves, struct bestMovesForBoard* bestMovesList){
	/* minimizing the minimax function */

	int i, j, score;
	Move* move;
	MovesList* moves;
	char* enemyColor;
	char newBoard[BOARD_SIZE][BOARD_SIZE];

	if (strcmp(currentColor, "black") == 0) {
		enemyColor = "white";
	}
	else {
		enemyColor = "black";
	}

	if (currentDepth == maxDepth) {		/*last level*/
		if (best == 0){
			return game_board_score(board, currentColor);
		}
		else{
			return best_scoring_func(board, currentColor, enemyColor);
		}
	}

	for (j = 0; j < BOARD_SIZE; ++j){	/*go over the whole game board*/
		for (i = 0; i < BOARD_SIZE; ++i){
			if (check_piece_color(board, j, i, currentColor) == 1){	/*if (j,i) contains a piece of the current color*/
				moves = init_new_movesList(j, i);
				get_moves_for_piece(board, moves, j, i);	/*get moves for the current piece*/
				delete_king_threatenning_moves(board, moves); /*delete illegal moves*/
				move = moves->start;

				while (move != moves->end){	/*go over each legal move of the current piece*/
					move = move->next;
					change_board_one_move(board, newBoard, j, i, move);
					score = alphaBeta_maximize(newBoard, alpha, beta, maxDepth, currentDepth + 1, enemyColor, myBestMove, best, flagBestMoves, bestMovesList); /*call to maximize*/
					if (beta > score){
						beta = score;
					}
					if (beta <= alpha){	/*pruning*/
						free_movesList(moves);
						free(moves);
						return beta;
					}
				}
				free_movesList(moves);
				free(moves);
			}
		}
	}
	return beta;
}


