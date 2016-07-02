#include "Moves.h"


Move* init_new_move(int j, int i) {
	/* initializes a new move */

	Move* move = (Move*)malloc(sizeof(Move));
	if (move == NULL) {
		perror("Error: standard function malloc has failed");

		exit(1);
	}

	move->col = j;
	move->row = i;
	move->becomeTo = 'd'; /*d= default  (not becoming other type)*/
	move->next = NULL;

	return move;

}


MovesList* init_new_movesList(int j, int i) {
	/* initializes a new moves list */

	MovesList* moves = (MovesList*)malloc(sizeof(MovesList));
	if (moves == NULL) {
		perror("Error: standard function malloc has failed");
		exit(1);
	}
	moves->start = init_new_move(j, i);
	moves->end = moves->start;
	return moves;
}

MovesList* init_new_movesList_forGui(int j, int i){
	/* initializes a new moves list */

	MovesList* moves = (MovesList*)malloc(sizeof(MovesList));
	if (moves == NULL) {
		perror("Error: standard function malloc has failed");
		exit(1);
	}
	moves->start = init_new_move(j, i);
	moves->end = init_new_move(j, i);
	moves->start->next = moves->end;
	return moves;
}

int is_empty_movesList(MovesList* moves){
	/* returns 1 if the moves list is empty, 0 otherwise */

	if ((moves->end->col == moves->start->col) && (moves->end->row == moves->start->row) && (moves->start->becomeTo == moves->end->becomeTo)){
		return 1; /*the list contains only the first position and no other positions to go to - no moves = empty movesList*/
	}
	return 0;
}

void free_movesList(MovesList* moves){
	/* free moves list */

	Move* currentMove;
	while (moves->start != moves->end){
		currentMove = moves->start;
		moves->start = moves->start->next;
		free(currentMove);
	}
	free(moves->start);
}

MovesList* init_new_movesList_for_bestMoves(int j, int i, int endj, int endi){
	/* initializes a new moves list for get best moves command */

	MovesList* moves = (MovesList*)malloc(sizeof(MovesList));
	if (moves == NULL) {
		perror("Error: standard function malloc has failed");

		exit(1);
	}
	moves->start = init_new_move(j, i);
	moves->end = init_new_move(endj, endi);
	return moves;
}

struct bestMoveForPosition* init_new_bestMoveForPosition(int j, int i, int endj, int endi){
	/* initializes a new bestMoveForPosition struct */

	struct bestMoveForPosition* fromToMove = (struct bestMoveForPosition*)malloc(sizeof(struct bestMoveForPosition));
	if (fromToMove == NULL) {
		perror("Error: standard function malloc has failed");

		exit(1);
	}

	fromToMove->bestForPosition = init_new_movesList_for_bestMoves(j, i, endj, endi);
	fromToMove->next = NULL;
	return fromToMove;
}

struct bestMovesForBoard* init_new_bestMovesForBoard(int j, int i){
	/* initializes a new bestMovesForBoard struct */

	struct bestMovesForBoard* bestMovesList = (struct bestMovesForBoard*)malloc(sizeof(struct bestMovesForBoard));
	if (bestMovesList == NULL) {
		perror("Error: standard function malloc has failed");
		exit(1);
	}
	bestMovesList->head = init_new_bestMoveForPosition(j, i, j, i);
	bestMovesList->tail = bestMovesList->head;
	return bestMovesList;
}

void free_bestMoveForPosition(struct bestMoveForPosition* bestMovePos){
	/* free bestMoveForPosition struct */

	free(bestMovePos->bestForPosition->start);
	free(bestMovePos->bestForPosition->end);
	
}

void free_bestMovesForBoard(struct bestMovesForBoard* bestForBoard){
	/* free bestMovesForBoard struct */

	struct bestMoveForPosition* currentBestMove;

	while (bestForBoard->head != bestForBoard->tail){
		currentBestMove = bestForBoard->head;
		bestForBoard->head = bestForBoard->head->next;
		free_bestMoveForPosition(currentBestMove);
		free(currentBestMove);
	}
	free_bestMoveForPosition(bestForBoard->head);
	free(bestForBoard->head);
}




void freeToFreeNode(toFreeNode * curToFreeNode){
	if (curToFreeNode != NULL){
		freeToFreeNode(curToFreeNode->next);
		if (curToFreeNode->pointer){
			free(curToFreeNode->pointer);
			curToFreeNode->pointer = NULL;
		}
		if (curToFreeNode){
			free(curToFreeNode);
			curToFreeNode = NULL;
		}
	}
}


void freeUsedList(){
	if (useSpace){
		freeToFreeNode(useSpace->head);
		free(useSpace);
		useSpace = NULL;
	}
}

void reInitUsedList(){
	freeUsedList();
	useSpace = (toFreeList *)calloc(1, sizeof(toFreeList));
	if (useSpace == NULL){
		perror("Error: standard function reInitUsedList has failed");

		exit(0);
	}
}
/*
* add data to yet to be freed list
*/
void addToUserUsedList(void * allocated){
	toFreeNode * oldHead = NULL;
	toFreeNode * newHead = (toFreeNode *)calloc(1, sizeof(toFreeNode));
	if (newHead == NULL){
		perror("Error: standard function addToUserUsedList has failed");

		freeUsedList();
		exit(0);
	}
	newHead->pointer = allocated;
	if (useSpace->head == NULL){
		useSpace->head = newHead;
	}
	else{
		oldHead = useSpace->head;
		useSpace->head = newHead;
		useSpace->head->next = oldHead;
	}
}


char* charToString_type(char c) {
	/* given a char representing a piece's type, returns its string representation */

	if (c == BLACK_Q || c == WHITE_Q) {
		return "queen";
	}

	else if (c == BLACK_B || c == WHITE_B){
		return "bishop";
	}

	else if (c == BLACK_N || c == WHITE_N) {
		return "knight";
	}

	else if (c == BLACK_R || c == WHITE_R) {
		return "rook";
	}

	else return "d";
}


char stringToChar_type(char* type, char* pieceColor){
	/* given a string representing a piece's type, returns its char representation */

	int color = strcmp(pieceColor, "white");

	if (strcmp(type, "queen\n") == 0){
		if (color == 0){
			return WHITE_Q;
		}
		else{
			return BLACK_Q;
		}
	}

	else if (strcmp(type, "bishop\n") == 0){
		if (color == 0){
			return WHITE_B;
		}
		else{
			return BLACK_B;
		}
	}

	else if (strcmp(type, "knight\n") == 0){
		if (color == 0){
			return WHITE_N;
		}
		else{
			return BLACK_N;
		}
	}

	else if (strcmp(type, "rook\n") == 0){
		if (color == 0){
			return WHITE_R;
		}
		else{
			return BLACK_R;
		}
	}

	else return 'd';
}


void print_movesList(MovesList* moves){
	/* prints all the moves in the given  moves list */

	Move* currentMove = moves->start;

	if (currentMove == moves->end){ /*empty moves list*/
		return;
	}

	currentMove = currentMove->next;

	while (currentMove != moves->end){ /*print every move*/
		if (printf("<%c,%d> ", moves->start->col + 97, moves->start->row + 1) < 0) {
			perror("Error: standard function printf has failed");

			exit(1);
		}
		if (printf("to <%c,%d>", currentMove->col + 97, currentMove->row + 1) < 0) {
			perror("Error: standard function printf has failed");
			exit(1);
		}
		if (currentMove->becomeTo != 'd') {
			if (printf(" %s\n", charToString_type(currentMove->becomeTo)) < 0) {
				perror("Error: standard function printf has failed");
				exit(1);
			}
		}
		else {
			if (printf("%s", "\n") < 0) {
				perror("Error: standard function printf has failed");
				exit(1);
			}
		}
		currentMove = currentMove->next;
	}

	if (printf("<%c,%d> ", moves->start->col + 97, moves->start->row + 1) < 0) {
		perror("Error: standard function printf has failed");
		exit(1);
	}
	if (printf("to <%c,%d>", currentMove->col + 97, currentMove->row + 1) < 0) {
		perror("Error: standard function printf has failed");
		exit(1);
	}
	if (currentMove->becomeTo != 'd') {
		if (printf(" %s\n", charToString_type(currentMove->becomeTo)) < 0) {
			perror("Error: standard function printf has failed");
			exit(1);
		}
	}
	else {
		if (printf("%s", "\n") < 0) {
			perror("Error: standard function printf has failed");
			exit(1);
		}
	}
}



void print_bestMovesList(struct bestMovesForBoard* moves){
	/* prints all the moves in the bestMovesForBoard struct */

	struct bestMoveForPosition* currentMove = moves->head;

	while (currentMove->bestForPosition->start->col != -1){
		if (printf("<%c,%d> ", currentMove->bestForPosition->start->col + 97, currentMove->bestForPosition->start->row + 1) < 0){
			perror("Error: standard function printf has failed");
			exit(1);
		}
		if (printf("to <%c,%d>", currentMove->bestForPosition->end->col + 97, currentMove->bestForPosition->end->row + 1) < 0){
			perror("Error: standard function printf has failed");
			exit(1);
		}
		if (currentMove->bestForPosition->end->becomeTo != 'd'){
			if (printf(" %s\n", charToString_type(currentMove->bestForPosition->end->becomeTo)) < 0) {
				perror("Error: standard function printf has failed");
				exit(1);
			}
		}
		else {
			if (printf("%s", "\n") < 0) {
				perror("Error: standard function printf has failed");
				exit(1);
			}
		}
		currentMove = currentMove->next;
	}
}


void change_board_one_move(char board[BOARD_SIZE][BOARD_SIZE], char newBoard[BOARD_SIZE][BOARD_SIZE], int fromCol, int fromRow, Move* moveToDo){
	/* changes the game board for one move */

	int i, j;
	char pieceType;
	int toCol = moveToDo->col;
	int toRow = moveToDo->row;

	for (j = 0; j < BOARD_SIZE; ++j) { /*copy the whole game board*/
		for (i = 0; i < BOARD_SIZE; ++i) {
			newBoard[j][i] = board[j][i];
		}
	}

	pieceType = moveToDo->becomeTo;
	if (pieceType != 'd'){	/*if pawn changes to another type*/
		newBoard[toCol][toRow] = pieceType;
	}
	else{
		newBoard[toCol][toRow] = board[fromCol][fromRow];
	}
	newBoard[fromCol][fromRow] = EMPTY;
}



void get_moves_bishop_rook_queen_by_numSteps(char board[BOARD_SIZE][BOARD_SIZE], MovesList* moves, int j, int i, int j_numSteps, int i_numSteps, char* pieceColor){
	/* gets all possible moves for a bishop/rook/queen in (j,i) given the number of steps to be done vertically (i_numSteps) and horizontally (j_numSteps) */

	while (is_valid_position_on_board(j + j_numSteps, i + i_numSteps) == 1){ /*check if the new position is valid*/
		if (check_piece_color(board, j + j_numSteps, i + i_numSteps, pieceColor) == 0){ /*empty or enemy square = possible move*/
			moves->end->next = init_new_move(j + j_numSteps, i + i_numSteps);
			moves->end = moves->end->next;

			if (board[j + j_numSteps][i + i_numSteps] != EMPTY){ /*if we ate opponent's piece*/
				break;
			}

			else{ /*if there was no eating, continue to check possible moves*/
				j += j_numSteps;
				i += i_numSteps;
			}
		}

		else{ /*we can't move*/
			break;
		}
	}
}


void get_moves_knight_king_by_numSteps(char board[BOARD_SIZE][BOARD_SIZE], MovesList* moves, int j, int i, int j_numSteps, int i_numSteps, char* pieceColor){
	/* gets all possible moves for a knight/king in (j,i) given the number of steps to be done vertically (i_numSteps) and horizontally (j_numSteps) */

	if (is_valid_position_on_board(j + j_numSteps, i + i_numSteps) == 1){ /*check if the new position is valid*/
		if (check_piece_color(board, j + j_numSteps, i + i_numSteps, pieceColor) == 0){ /*empty or enemy square = possible move*/
			moves->end->next = init_new_move(j + j_numSteps, i + i_numSteps);
			moves->end = moves->end->next;
		}
	}
}


void get_moves_pawn_by_numSteps(char board[BOARD_SIZE][BOARD_SIZE], MovesList* moves, int j, int i, int j_numSteps, int i_numSteps){
	/* gets all possible moves for a pawn in (j,i) given the number of steps to be done vertically (i_numSteps) and horizontally (j_numSteps) */

	if (is_valid_position_on_board(j + j_numSteps, i + i_numSteps)){ /*check if the new position is valid*/

		if (check_piece_color(board, j, i, "white") == 1){
			if ((check_piece_color(board, j + j_numSteps, i + i_numSteps, "black") == 1 && j_numSteps != 0) || (board[j + j_numSteps][i + i_numSteps] == EMPTY && j_numSteps == 0)){
				/*means we eat opponent's piece or move to an empty square*/
				if (i == 6){
					moves->end->next = init_new_move(j + j_numSteps, i + i_numSteps);
					moves->end->next->becomeTo = WHITE_Q;
					moves->end = moves->end->next;
					moves->end->next = init_new_move(j + j_numSteps, i + i_numSteps);
					moves->end->next->becomeTo = WHITE_B;
					moves->end = moves->end->next;
					moves->end->next = init_new_move(j + j_numSteps, i + i_numSteps);
					moves->end->next->becomeTo = WHITE_N;
					moves->end = moves->end->next;
					moves->end->next = init_new_move(j + j_numSteps, i + i_numSteps);
					moves->end->next->becomeTo = WHITE_R;
					moves->end = moves->end->next;
				}
				else{
					moves->end->next = init_new_move(j + j_numSteps, i + i_numSteps);
					moves->end = moves->end->next;
				}
			}
		}
		else if (check_piece_color(board, j, i, "black") == 1){
			if ((check_piece_color(board, j + j_numSteps, i + i_numSteps, "white") == 1 && j_numSteps != 0) || (board[j + j_numSteps][i + i_numSteps] == EMPTY && j_numSteps == 0)){
				/*means we eat opponent's piece or move to an empty square*/
				if (i == 1){
					moves->end->next = init_new_move(j + j_numSteps, i + i_numSteps);
					moves->end->next->becomeTo = BLACK_Q;
					moves->end = moves->end->next;
					moves->end->next = init_new_move(j + j_numSteps, i + i_numSteps);
					moves->end->next->becomeTo = BLACK_B;
					moves->end = moves->end->next;
					moves->end->next = init_new_move(j + j_numSteps, i + i_numSteps);
					moves->end->next->becomeTo = BLACK_N;
					moves->end = moves->end->next;
					moves->end->next = init_new_move(j + j_numSteps, i + i_numSteps);
					moves->end->next->becomeTo = BLACK_R;
					moves->end = moves->end->next;
				}
				else{
					moves->end->next = init_new_move(j + j_numSteps, i + i_numSteps);
					moves->end = moves->end->next;
				}
			}
		}
	}
}


void get_moves_pawn(char board[BOARD_SIZE][BOARD_SIZE], MovesList* moves, int j, int i){
	/* gets all possible moves for a pawn in (j,i) */

	if (board[j][i] == WHITE_P) {
		get_moves_pawn_by_numSteps(board, moves, j, i, 0, 1);
		get_moves_pawn_by_numSteps(board, moves, j, i, -1, 1);
		get_moves_pawn_by_numSteps(board, moves, j, i, 1, 1);
	}
	else if (board[j][i] == BLACK_P) {
		get_moves_pawn_by_numSteps(board, moves, j, i, 0, -1);
		get_moves_pawn_by_numSteps(board, moves, j, i, 1, -1);
		get_moves_pawn_by_numSteps(board, moves, j, i, -1, -1);
	}

}


void get_moves_bishop(char board[BOARD_SIZE][BOARD_SIZE], MovesList* moves, int j, int i, char* pieceColor){
	/* gets all possible moves for a bishop in (j,i) */


	get_moves_bishop_rook_queen_by_numSteps(board, moves, j, i, 1, 1, pieceColor);
	get_moves_bishop_rook_queen_by_numSteps(board, moves, j, i, 1, -1, pieceColor);
	get_moves_bishop_rook_queen_by_numSteps(board, moves, j, i, -1, 1, pieceColor);
	get_moves_bishop_rook_queen_by_numSteps(board, moves, j, i, -1, -1, pieceColor);
}


void get_moves_knight(char board[BOARD_SIZE][BOARD_SIZE], MovesList* moves, int j, int i, char* pieceColor){
	/* gets all possible moves for a knight in (j,i) */

	get_moves_knight_king_by_numSteps(board, moves, j, i, -1, -2, pieceColor);
	get_moves_knight_king_by_numSteps(board, moves, j, i, -1, 2, pieceColor);
	get_moves_knight_king_by_numSteps(board, moves, j, i, 1, -2, pieceColor);
	get_moves_knight_king_by_numSteps(board, moves, j, i, 1, 2, pieceColor);
	get_moves_knight_king_by_numSteps(board, moves, j, i, -2, -1, pieceColor);
	get_moves_knight_king_by_numSteps(board, moves, j, i, -2, 1, pieceColor);
	get_moves_knight_king_by_numSteps(board, moves, j, i, 2, -1, pieceColor);
	get_moves_knight_king_by_numSteps(board, moves, j, i, 2, 1, pieceColor);
}


void get_moves_rook(char board[BOARD_SIZE][BOARD_SIZE], MovesList* moves, int j, int i, char* pieceColor){
	/* gets all possible moves for a rook in in (j,i) */

	get_moves_bishop_rook_queen_by_numSteps(board, moves, j, i, -1, 0, pieceColor);
	get_moves_bishop_rook_queen_by_numSteps(board, moves, j, i, 1, 0, pieceColor);
	get_moves_bishop_rook_queen_by_numSteps(board, moves, j, i, 0, -1, pieceColor);
	get_moves_bishop_rook_queen_by_numSteps(board, moves, j, i, 0, 1, pieceColor);
}


void get_moves_queen(char board[BOARD_SIZE][BOARD_SIZE], MovesList* moves, int j, int i, char* pieceColor) {
	/* gets all possible moves for a queen in (j,i) */

	get_moves_bishop(board, moves, j, i, pieceColor);
	get_moves_rook(board, moves, j, i, pieceColor);
}


void get_moves_king(char board[BOARD_SIZE][BOARD_SIZE], MovesList* moves, int j, int i, char* pieceColor){
	/* gets all possible moves for a king in (j,i)  */

	get_moves_knight_king_by_numSteps(board, moves, j, i, 1, 1, pieceColor);
	get_moves_knight_king_by_numSteps(board, moves, j, i, 1, -1, pieceColor);
	get_moves_knight_king_by_numSteps(board, moves, j, i, -1, 1, pieceColor);
	get_moves_knight_king_by_numSteps(board, moves, j, i, -1, -1, pieceColor);
	get_moves_knight_king_by_numSteps(board, moves, j, i, -1, 0, pieceColor);
	get_moves_knight_king_by_numSteps(board, moves, j, i, 1, 0, pieceColor);
	get_moves_knight_king_by_numSteps(board, moves, j, i, 0, -1, pieceColor);
	get_moves_knight_king_by_numSteps(board, moves, j, i, 0, 1, pieceColor);

}


void get_moves_for_piece(char board[BOARD_SIZE][BOARD_SIZE], MovesList* moves, int j, int i){
	/* gets all moves for the piece in (j,i) */

	char* pieceColor;

	moves->start->col = j;
	moves->start->row = i;


	if (check_piece_color(board, j, i, "white") == 1){
		pieceColor = "white";
	}
	else{
		pieceColor = "black";
	}

	if (board[j][i] == BLACK_K || board[j][i] == WHITE_K){
		get_moves_king(board, moves, j, i, pieceColor);
	}
	else if (board[j][i] == BLACK_Q || board[j][i] == WHITE_Q){
		get_moves_queen(board, moves, j, i, pieceColor);
	}
	else if (board[j][i] == BLACK_B || board[j][i] == WHITE_B){
		get_moves_bishop(board, moves, j, i, pieceColor);
	}
	else if (board[j][i] == BLACK_N || board[j][i] == WHITE_N){
		get_moves_knight(board, moves, j, i, pieceColor);
	}
	else if (board[j][i] == BLACK_R || board[j][i] == WHITE_R){
		get_moves_rook(board, moves, j, i, pieceColor);
	}
	else{
		get_moves_pawn(board, moves, j, i);
	}
}


void delete_king_threatenning_moves(char board[BOARD_SIZE][BOARD_SIZE], MovesList* userMoves){
	/* deletes moves that threaten the user's king - these are illegal moves for the user.
	move that threatens the user's king is a move that after it, the enemy can eat the user's king ("Check") */

	char newBoard[BOARD_SIZE][BOARD_SIZE];
	char* enemyColor;

	Move* currentMove;
	Move* prevMove;
	Move* nextMove;

	int pieceColumn = userMoves->start->col;
	int pieceRow = userMoves->start->row;

	if (check_piece_color(board, pieceColumn, pieceRow, "white") == 1){
		enemyColor = "black";
	}
	else{
		enemyColor = "white";
	}

	prevMove = userMoves->start; /*go over each move in the list and check if the enemy threatens the user's king ("Check") after the move*/
	for (currentMove = userMoves->start->next; currentMove != userMoves->end->next; currentMove = nextMove){
		change_board_one_move(board, newBoard, pieceColumn, pieceRow, currentMove);

		if (check_if_color_can_Check(newBoard, enemyColor) == 1){ /*enemy threatens the king = illegal move - delete this move from the list*/
			if (currentMove == userMoves->end){
				nextMove = currentMove->next;
				userMoves->end = prevMove;
				free(currentMove);
				break;
			}
			prevMove->next = currentMove->next;
			nextMove = currentMove->next;
			free(currentMove);
		}
		else{
			nextMove = currentMove->next;
			prevMove = currentMove;
		}

	}
}


void get_possible_moves_for_piece(char board[BOARD_SIZE][BOARD_SIZE], MovesList* moves, int j, int i){
	/* gets all possible moves for the piece in (j,i) */

	get_moves_for_piece(board, moves, j, i);
	delete_king_threatenning_moves(board, moves);
}


void print_possible_moves_for_piece(char board[BOARD_SIZE][BOARD_SIZE], int j, int i, char* pieceColor){
	/* prints possible moves for the piece in (j,i) */

	if (is_valid_position_on_board(j, i) == 1){
		if (check_piece_color(board, j, i, pieceColor) == 1){

			MovesList* moves = init_new_movesList(j, i);
			get_possible_moves_for_piece(board, moves, j, i);
			print_movesList(moves);
			free_movesList(moves);
			free(moves);
		}
		else{
			print_message("This position does not contain your piece\n");
		}
	}
	else{
		print_message(WRONG_POSITION);
	}
}


int create_userMove(char board[BOARD_SIZE][BOARD_SIZE], MovesList* moves, char* pieceColor){
	/* creates the user's move according to the input given by console
	returns 1 if the move was successfully created, and 0 otherwise */

	char userInput[60];
	char* pieceType;
	int row = -1;
	int col = -1;

	fgets(userInput, sizeof(userInput), stdin); /*user type his move in console*/

	/*first we get the starting position of the move from the user input*/
	col = (int)(userInput[2] - 'a');
	row = (int)(userInput[4] - '0') - 1;
	if (is_valid_position_on_board(col, row) == 0){ /*check if the position is valid*/
		print_message(WRONG_POSITION);
		return 0;
	}
	else{ /*set the starting position of the move*/
		moves->start->col = col;
		moves->start->row = row;
	}

	/*second we get the ending position of the move from the user input*/
	col = (int)(userInput[11] - 'a');
	row = (int)(userInput[13] - '0') - 1;

	if (is_valid_position_on_board(col, row) == 0){ /*check if the position is valid*/
		print_message(WRONG_POSITION);
		return 0;
	}
	else{
		/*set the ending position of the move*/
		moves->start->next = init_new_move(col, row);
		moves->end = moves->start->next;
		if ((board[moves->start->col][moves->start->row] == BLACK_P && moves->end->row == 0) || (board[moves->start->col][moves->start->row] == WHITE_P && moves->end->row == 7)){
			pieceType = &(userInput[15]);
			if (strcmp(pieceType, "\n") != 0){ /*if the user defined the new type*/
				pieceType = &(userInput[16]);
				moves->end->becomeTo = stringToChar_type(pieceType, pieceColor);
			}
			else{ /*if the user didn't defined the new type - the default is queen*/
				if (check_piece_color(board, moves->start->col, moves->start->row, "white") == 1){
					moves->end->becomeTo = WHITE_Q;
				}
				else{
					moves->end->becomeTo = BLACK_Q;
				}
			}
		}
	}

	return 1; /*move was successfully created*/
}



int create_userMove_for_gui(char board[BOARD_SIZE][BOARD_SIZE], MovesList* moves){
	/* creates the user's move according to the input given by console
	returns 1 if the move was successfully created, and 0 otherwise */

	int fromRow = moves->start->row;
	int fromCol = moves->start->col;
	int toRow = moves->end->row;
	int toCol = moves->end->col;

	/*first we get the starting position of the move from the user input*/
	if (is_valid_position_on_board(fromCol, fromRow) == 0){ /*check if the position is valid*/
		print_message(WRONG_POSITION);
		return 0;
	}

	/*second we get the ending position of the move from the user input*/

	if (is_valid_position_on_board(toCol, toRow) == 0){ /*check if the position is valid*/
		print_message(WRONG_POSITION);
		return 0;
	}

	return 1; /*move was successfully created*/
}


int canPromotePawn(char * color, int y){
	if (strcmp(color, "white") == 0){
		return y == 7;
	}
	return y == 0;
}

char * getOpositeColor(char * color){
	if (strcmp(color, "white") == 0){
		return "black";
	}
	else{
		return "white";

	}
}

int is_legal_move(char board[BOARD_SIZE][BOARD_SIZE], MovesList* userMove, char* currentTurnColor, int console){
	/* returns 1 if the given user move is a legal move, 0 otherwise */

	MovesList* possibleMoves = init_new_movesList(-1, -1);
	Move* currentMove;

	int userMove_startColumn = userMove->start->col; /*initial col by user input*/
	int userMove_startRow = userMove->start->row; /*initial row by user input*/
	int userMove_afterMoveColumn = userMove->end->col; /*col after the move by user input*/
	int userMove_afterMoveRow = userMove->end->row; /*row after the move by user input*/
	int userMove_becomeTo = userMove->end->becomeTo; /*type after the move by user input*/

	int moveInList_startColumn; /*initial col by move in possible moves list*/
	int moveInList_startRow; /*initial row by move in possible moves list*/
	int moveInList_afterMoveColumn; /*col after move in the possible moves list*/
	int moveInList_afterMoveRow; /*row after move in the possible moves list*/
	int moveInList_becomeTo; /*type after move in the possible moves list*/

	/*check that the piece's color is indeed from the color that has the current turn*/
	if (check_piece_color(board, userMove_startColumn, userMove_startRow, currentTurnColor) == 0){ /*not the same color - illegal move*/
		if (console == 1){
			print_message(ILLEGAL_MOVE);
		}
		free_movesList(possibleMoves);
		free(possibleMoves);
		return 0;
	}

	/*check that after the first position in the list there is another position to jump to*/
	if (userMove->start->next == NULL){ /*no position to jump to - illegal move*/
		if (console == 1){
			print_message(ILLEGAL_MOVE);
		}
		free_movesList(possibleMoves);
		free(possibleMoves);
		return 0;
	}

	get_possible_moves_for_piece(board, possibleMoves, userMove_startColumn, userMove_startRow);

	for (currentMove = possibleMoves->start->next; currentMove != (possibleMoves->end)->next; currentMove = currentMove->next) { /*loop over over every possible move*/
		moveInList_startColumn = possibleMoves->start->col;
		moveInList_startRow = possibleMoves->start->row;

		if (userMove_startColumn == moveInList_startColumn && userMove_startRow == moveInList_startRow){ /*start square*/

			moveInList_afterMoveColumn = currentMove->col;
			moveInList_afterMoveRow = currentMove->row;
			moveInList_becomeTo = currentMove->becomeTo;

			if (userMove_afterMoveColumn == moveInList_afterMoveColumn && userMove_afterMoveRow == moveInList_afterMoveRow){
				if (userMove_becomeTo == moveInList_becomeTo){ /*means that the given user move appears in possible moves list, therefore legal move*/
					free_movesList(possibleMoves);
					free(possibleMoves);
					return 1;
				}
			}
		}

	}

	if (console == 1){
		print_message(ILLEGAL_MOVE);
	}
	free_movesList(possibleMoves);
	free(possibleMoves);
	return 0;
}



int areTherePossibleMoves(char board[BOARD_SIZE][BOARD_SIZE], char* currentColor){
	/* returns 1 if the current color has possible moves to do , 0 otherwise */

	int i, j;

	for (j = 0; j < BOARD_SIZE; ++j){
		for (i = 0; i < BOARD_SIZE; ++i){ /*go over the whole game board*/
			if (check_piece_color(board, j, i, currentColor) == 1){ /*go over every piece of the color and get his moves*/
				struct MovesList* moves = init_new_movesList(j, i);
				get_possible_moves_for_piece(board, moves, j, i);

				if ((moves->start->next) != (moves->end->next)){
					free_movesList(moves);
					free(moves);
					return 1; /* there is at least one possible move */
				}

				free_movesList(moves);
				free(moves);
			}
		}
	}

	return 0; /* there are no possible moves */
}

