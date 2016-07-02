
#include "chessprog.h"
#include "ChessGame.h"
#include "xmlFiles.h"
#include "Moves.h"
#include "minmax.h"
#include "Gui.h"

static int best = 0;
static int staticMinMaxDepth = 1;
static int staticMode = 1;
static int staticCurrentTurn = 0;  /*0-white, 1-black*/
static int staticUserColor = 0;     /*0-white, 1-black*/

int getStaticCurrentTurn(void){
	return staticCurrentTurn;
}

int isFileExists(char *xmlFileName){

	char* fileContentString = xml_file_to_string(xmlFileName);
	if (fileContentString == NULL){
		free(fileContentString);
		return 0; /*file doesn't exist*/
	}
	free(fileContentString);
	return 1; /*file exists*/

}


void init_board(char board[BOARD_SIZE][BOARD_SIZE]){
	/* initializes the game board */
	int i;
	int j;
	for (i = 0; i < BOARD_SIZE; ++i){
		for (j = 0; j < BOARD_SIZE; ++j){
			board[j][i] = EMPTY;
		}
	}
	for (i = 0; i < BOARD_SIZE; ++i){
		board[i][1] = WHITE_P;
		board[i][6] = BLACK_P;
	}

	board[0][0] = WHITE_R;
	board[1][0] = WHITE_N;
	board[2][0] = WHITE_B;
	board[3][0] = WHITE_Q;
	board[4][0] = WHITE_K;
	board[5][0] = WHITE_B;
	board[6][0] = WHITE_N;
	board[7][0] = WHITE_R;
	board[0][7] = BLACK_R;
	board[1][7] = BLACK_N;
	board[2][7] = BLACK_B;
	board[3][7] = BLACK_Q;
	board[4][7] = BLACK_K;
	board[5][7] = BLACK_B;
	board[6][7] = BLACK_N;
	board[7][7] = BLACK_R;
}


void print_line(){
	/* helper of "print_board" function */
	int i;
	if (printf(" |") < 0) {
		perror("Error: standard function printf has failed");
		exit(1);
	}
	for (i = 1; i < BOARD_SIZE * 4; i++){
		if (printf("-") < 0) {
			perror("Error: standard function printf has failed");
			exit(1);
		}
	}
	if (printf("|\n") < 0) {
		perror("Error: standard function printf has failed");
		exit(1);
	}
}


void print_board(char board[BOARD_SIZE][BOARD_SIZE]){
	/* prints the current game board */
	int i, j;
	print_line();
	for (j = BOARD_SIZE - 1; j >= 0; j--){
		if (printf("%d", j + 1) < 0) {
			perror("Error: standard function printf has failed");
			exit(1);
		}
		for (i = 0; i < BOARD_SIZE; i++){
			if (printf("| %c ", board[i][j]) < 0) {
				perror("Error: standard function printf has failed");
				exit(1);
			}
		}
		if (printf("|\n") < 0) {
			perror("Error: standard function printf has failed");
			exit(1);
		}
		print_line();
	}
	if (printf("  ") < 0) {
		perror("Error: standard function printf has failed");
		exit(1);
	}
	for (j = 0; j < BOARD_SIZE; j++){
		if (printf(" %c  ", (char)('a' + j)) < 0) {
			perror("Error: standard function printf has failed");
			exit(1);
		}
	}
	if (printf("\n") < 0) {
		perror("Error: standard function printf has failed");
		exit(1);
	}
}


int is_white_piece(char board[BOARD_SIZE][BOARD_SIZE], int j, int i){
	/* returns 1 if the piece in (j,i) is in white color, 0 otherwise */

	if (board[j][i] == WHITE_B || board[j][i] == WHITE_K || board[j][i] == WHITE_N || board[j][i] == WHITE_P || board[j][i] == WHITE_Q || board[j][i] == WHITE_R){
		return 1;
	}
	return 0;

}


int is_black_piece(char board[BOARD_SIZE][BOARD_SIZE], int j, int i){
	/* returns 1 if the piece in (j,i) is in black color, 0 otherwise */

	if (board[j][i] == BLACK_B || board[j][i] == BLACK_K || board[j][i] == BLACK_N || board[j][i] == BLACK_P || board[j][i] == BLACK_Q || board[j][i] == BLACK_R){
		return 1;
	}

	return 0;

}


int check_piece_color(char board[BOARD_SIZE][BOARD_SIZE], int j, int i, char* color){
	/* checks if the piece in (j,i) is indeed in the given color */

	if (board[j][i] == EMPTY){
		return 0;
	}
	else{
		if (strcmp(color, "black") == 0){
			if (board[j][i] == BLACK_B || board[j][i] == BLACK_K || board[j][i] == BLACK_N || board[j][i] == BLACK_P || board[j][i] == BLACK_Q || board[j][i] == BLACK_R){
				return 1;
			}
			else{
				return 0;
			}
		}
		else{
			if (board[j][i] == WHITE_B || board[j][i] == WHITE_K || board[j][i] == WHITE_N || board[j][i] == WHITE_P || board[j][i] == WHITE_Q || board[j][i] == WHITE_R){
				return 1;
			}
			else{
				return 0;
			}
		}
	}
}

int is_valid_position_on_board(int j, int i){
	/* checks if the indexes make a valid position on the game board */
	if (i<0 || i>BOARD_SIZE - 1 || j<0 || j>BOARD_SIZE - 1){
		return 0; /*not valid*/
	}
	else{
		return 1; /*valid*/
	}
}

char *getSlotFileName(int slot){
	char *fileSlotName = NULL;
	int slotNameLen = strlen("game%d.xml") - 2 + log(7) / 1 + 1;
	fileSlotName = (char *)calloc(slotNameLen, sizeof(char));
	sprintf(fileSlotName, "game%d.xml", slot);
	return fileSlotName;
}


int load_game(char board[BOARD_SIZE][BOARD_SIZE], int* inputMode, int* inputDifficulty, int* inputBest, int* inputUserColor, int* inputCurrentTurn, char* inputFileName){
	/* load a game from a file name given by input or by console */

	char* xmlFileName;
	char input[100];
	char* fileContentString;

	char* diff;

	char* nextTurn;

	char* userColor;

	if (inputFileName == NULL){ /*file name given by console*/
		fgets(input, sizeof(input), stdin);
		xmlFileName = &(input[1]);
		strtok(xmlFileName, "\n");
	}
	else{ /*file name given by input*/
		xmlFileName = inputFileName;
	}

	fileContentString = xml_file_to_string(xmlFileName);
	if (fileContentString == NULL){
		print_message(WRONG_FILE_NAME);
		return 1;
	}
	else{
		if (strcmp(get_turn_mode_difficulty_color(fileContentString, "<game_mode>"), "1") == 0){
			*inputMode = 1;
		}
		else if (strcmp(get_turn_mode_difficulty_color(fileContentString, "<game_mode>"), "2") == 0){
			*inputMode = 2;
		}

		nextTurn = get_turn_mode_difficulty_color(fileContentString, "<next_turn>");

		xmlBoard_to_consoleBoard(board, fileContentString);

		if (*inputMode == 1){ /*mode 1: player Vs. player*/
			if (strcmp(nextTurn, "white") == 0){
				*inputCurrentTurn = 0;
			}
			else{
				*inputCurrentTurn = 1;
			}
			*inputUserColor = *inputCurrentTurn; /*first players color by the current turn*/
			free(fileContentString);
			return 0;
		}
		else { /*mode 2: player Vs. AI*/
			diff = get_turn_mode_difficulty_color(fileContentString, "<difficulty>");
			if (strcmp(diff, "best") == 0){
				*inputBest = 1;
			}
			else{
				*inputBest = 0;
				*inputDifficulty = (int)(*diff - '0');
			}

			userColor = get_turn_mode_difficulty_color(fileContentString, "<user_color>");
			if (strcmp(userColor, "white") == 0){
				*inputUserColor = 0;
			}
			else{
				*inputUserColor = 1;
			}
			if (strcmp(nextTurn, "white") == 0 && (*inputUserColor) == 0){
				*inputCurrentTurn = 0;
			}
			else if (strcmp(nextTurn, "black") == 0 && (*inputUserColor) == 1){
				*inputCurrentTurn = 1;
			}

			free(fileContentString);
			return 0;
		}
		free(fileContentString);
	}
	return 0;
}


void save_game(char board[BOARD_SIZE][BOARD_SIZE], int mode, int difficulty, int best, int color, char* inputFileName){
	/* save current game */

	char* xmlFileName;
	char input[100];
	int i;

	char* nextTurn;
	char* userColor;

	FILE* xmlFile = NULL;

	if (inputFileName == NULL){ /*file name given by console*/
		fgets(input, sizeof(input), stdin);
		for (i = 1; i < 100; i++){
			if (input[i] == '\n') {
				break;
			}
		}
		xmlFileName = (&input[1]);
		strtok(xmlFileName, "\n");
	}
	else{ /*file name given by input*/
		xmlFileName = inputFileName;
	}

	xmlFile = create_xml_file(xmlFileName);
	if (xmlFile == NULL) {
		print_message(WRONG_FILE_NAME);
	}
	else{
		if (color == 0){
			userColor = "white";
		}
		else{
			userColor = "black";
		}

		nextTurn = userColor;

		write_game_to_xml_file(board, xmlFile, mode, difficulty, best, userColor, nextTurn);
		fclose(xmlFile);
	}
}

int amount_of_piece(char p, char board[BOARD_SIZE][BOARD_SIZE]){
	/* returns the amount of the given type of piece on the game board */
	int amount = 0;
	int i, j;
	for (i = 0; i < BOARD_SIZE; ++i) {
		for (j = 0; j < BOARD_SIZE; ++j){
			if (board[j][i] == p){
				amount += 1;
			}
		}
	}
	return amount;
}



void set(char board[BOARD_SIZE][BOARD_SIZE], int j, int i, char* color, char* type, int console){
	/* sets game tool: pawn/rook/bishop/knight/queen/king of the given color on the board, in the given position if the action is legal,
	else notifies that it is illegal */
	char p;

	int illegal = 0;

	if (strcmp(color, "black") == 0){
		if (strcmp(type, "rook") == 0){
			p = BLACK_R;
			if (amount_of_piece(p, board) > 1){
				illegal = 1;
			}
		}
		else if (strcmp(type, "knight") == 0){
			p = BLACK_N;
			if (amount_of_piece(p, board) > 1){
				illegal = 1;
			}
		}
		else if (strcmp(type, "bishop") == 0) {
			p = BLACK_B;
			if (amount_of_piece(p, board) > 1){
				illegal = 1;
			}
		}
		else if (strcmp(type, "king") == 0) {
			p = BLACK_K;
			if (amount_of_piece(p, board) > 0){
				illegal = 1;
			}
		}
		else if (strcmp(type, "queen") == 0) {
			p = BLACK_Q;
			if (amount_of_piece(p, board) > 0){
				illegal = 1;
			}
		}
		else if (strcmp(type, "pawn") == 0) {
			p = BLACK_P;
			if (amount_of_piece(p, board) > 7){
				illegal = 1;
			}

		}
	}
	else{
		if (strcmp(color, "white") == 0){
			if (strcmp(type, "rook") == 0){
				p = WHITE_R;

				if (amount_of_piece(p, board) > 1){
					illegal = 1;
				}
			}
			else if (strcmp(type, "knight") == 0){
				p = WHITE_N;

				if (amount_of_piece(p, board) > 1){
					illegal = 1;
				}
			}
			else if (strcmp(type, "bishop") == 0) {
				p = WHITE_B;

				if (amount_of_piece(p, board) > 1){
					illegal = 1;
				}
			}
			else if (strcmp(type, "king") == 0) {
				p = WHITE_K;

				if (amount_of_piece(p, board) > 0){
					illegal = 1;
				}
			}
			else if (strcmp(type, "queen") == 0) {
				p = WHITE_Q;

				if (amount_of_piece(p, board) > 0){
					illegal = 1;
				}

			}
			else if (strcmp(type, "pawn") == 0) {
				p = WHITE_P;
				if (amount_of_piece(p, board) > 7){
					illegal = 1;
				}

			}
		}
	}

	if (illegal == 0){ /*if it's legal to put the piece on the game board*/

		board[j][i] = p;
	}
	else{
		if (console){
			printf(NO_PIECE);

		}
		
	}
}


void remove_disc(char board[BOARD_SIZE][BOARD_SIZE], int j, int i){
	if (is_valid_position_on_board(j, i) == 1){
		board[j][i] = EMPTY;
	}
}


void game_state_twoPlayers(char board[BOARD_SIZE][BOARD_SIZE]){
	/* game state - Player vs. Player */

	char action[60];
	int input;
	char* currentColor;
	char* enemyColor;
	char* colorForPrint = "White"; /*default */
	char* enemyColorForPrint = "Black";
	struct MovesList* userMove;
	int j, i;
	int userDepthMinmax;
	int userBest = 0;
	int depthToCheck;

	MovesList* userBestMove;
	bestMovesForBoard* bestMoves;

	if (staticCurrentTurn == 1){
		colorForPrint = "Black";
		enemyColorForPrint = "White";
	}

	if (staticCurrentTurn == 0){
		currentColor = "white";
		enemyColor = "black";
	}
	else{
		currentColor = "black";
		enemyColor = "white";
	}


	if (areTherePossibleMoves(board, currentColor) == 0){

		if (check_if_color_can_Mate(board, enemyColor) == 1){ /*Mate: player enemyColor wins*/
			if (printf("Mate! %s player wins the game\n", enemyColorForPrint) < 0) {
				perror("Error: standard function printf has failed");
				exit(1);
			}
			return;
		}
		else{
			if (printf("The game ends in a tie\n") < 0) {
				perror("Error: standard function printf has failed");
				exit(1);
			}
			return;
		}
	}



	if (printf("%s player - enter your move:\n", colorForPrint) < 0) {
		perror("Error: standard function printf has failed");
		exit(1);
	}
	/*player's first command*/
	if (scanf("%s", action) != 1) {
		perror("Error: standard function scanf has failed");
		exit(1);
	}

	while (strcmp(action, "quit") != 0){
		if (staticCurrentTurn == 0){
			currentColor = "white";
			enemyColor = "black";
		}
		else{
			currentColor = "black";
			enemyColor = "white";
		}

		if (strcmp(action, "move") == 0){ /*make a move*/
			userMove = init_new_movesList(-1, -1);
			if (create_userMove(board, userMove, currentColor) == 0){ /*move wasn't created*/
				staticCurrentTurn = abs(staticCurrentTurn - 1);
				staticUserColor = abs(staticUserColor - 1);
			}
			else if (is_legal_move(board, userMove, currentColor, 1) == 0){ /*move created is not legal*/

				staticCurrentTurn = abs(staticCurrentTurn - 1);
				staticUserColor = abs(staticUserColor - 1);
			}
			else{ /*the given move is legal - make the move*/
				change_board_one_move(board, board, userMove->start->col, userMove->start->row, userMove->end); /*errors with the MovesList struct, dont know why*/
				print_board(board);

				if (check_if_color_can_Mate(board, currentColor) == 1){ /*Mate: player of currentColor wins*/
					if (printf("Mate! %s player wins the game\n", colorForPrint) < 0) {
						perror("Error: standard function printf has failed");
						exit(1);
					}
					free_movesList(userMove);
					free(userMove);
					break;
				}

				else if (areTherePossibleMoves(board, enemyColor) == 0){ /*if there aren't any possible moves*/
					if (check_if_color_can_Check(board, currentColor) == 1){
						if (printf("Mate! %s player wins the game\n", colorForPrint) < 0) {
							perror("Error: standard function printf has failed");
							exit(1);
						}
						free_movesList(userMove);
						free(userMove);
						break;
					}
					else{
						if (printf("The game ends in a tie\n") < 0) {
							perror("Error: standard function printf has failed");
							exit(1);
						}
						free_movesList(userMove);
						free(userMove);
						break;
					}
				}
				else if (check_if_color_can_Check(board, currentColor) == 1){ /*Check: player of currentColor threatens the other player's king*/
					if (printf("Check!\n") < 0) {
						perror("Error: standard function printf has failed");
						exit(1);

					}
				}
			}
			free_movesList(userMove);
			free(userMove);
		}
		else if (strcmp(action, "get_moves") == 0){ /*print the possible moves for the piece in the requested position */
			if (scanf("%s", action) != 1) {
				perror("Error: standard function scanf has failed");
				exit(1);
			}
			j = (int)(action[1] - 'a');
			i = (int)(action[3] - '0') - 1;
			print_possible_moves_for_piece(board, j, i, currentColor);
			staticCurrentTurn = abs(staticCurrentTurn - 1);
			staticUserColor = abs(staticUserColor - 1);
		}
		else if (strcmp(action, "get_best_moves") == 0){ /*print the best moves for the current player*/
			if (scanf("%s", action) != 1) {
				perror("Error: standard function scanf has failed");
				exit(1);
			}
			if (strcmp(action, "best") == 0){
				userBest = 1;
				userDepthMinmax = 4;
			}
			else{
				depthToCheck = atoi(action);

				if (depthToCheck < 1 || depthToCheck>4){
					print_message(WRONG_MINIMAX_DEPTH);

				}
				else{
					userDepthMinmax = depthToCheck;
				}
			}
			/*we may assume that depth is legal */

			bestMoves = init_new_bestMovesForBoard(-1, -1);

			userBestMove = init_new_movesList(-1, -1);
			userBestMove->start->next = init_new_move(-1, -1);
			userBestMove->end = userBestMove->start->next;


			alphaBeta_maximize(board, -10000000, 10000000, userDepthMinmax, 0, currentColor, userBestMove, userBest, 1, bestMoves);
			print_bestMovesList(getMyBest());
			staticUserColor = abs(staticUserColor - 1);
			staticCurrentTurn = abs(staticCurrentTurn - 1);
			free_movesList(userBestMove);
			free(userBestMove);

			free(bestMoves);
		}

		else if (strcmp(action, "get_score") == 0){ /*print the score of a specific move*/
			if (scanf("%s", action) != 1) {
				perror("Error: standard function scanf has failed");
				exit(1);
			}
			if (strcmp(action, "best") == 0){
				userBest = 1;
				userDepthMinmax = 4;
			}
			else{
				depthToCheck = atoi(action);

				if (depthToCheck < 1 || depthToCheck>4){
					print_message(WRONG_MINIMAX_DEPTH);

				}
				else{
					userDepthMinmax = depthToCheck;
				}
			}
			/*we may assume that depth is legal*/
			if (scanf("%s", action) != 1) {
				perror("Error: standard function scanf has failed");
				exit(1);
			}

			if (strcmp(action, "move") == 0){

				userMove = init_new_movesList(-1, -1);
				if (create_userMove(board, userMove, currentColor) == 0){ /*move wasn't created*/
					staticUserColor = abs(staticUserColor - 1);
					staticCurrentTurn = abs(staticCurrentTurn - 1);
				}
				else if (is_legal_move(board, userMove, currentColor, 1) == 0){ /*move created is not legal*/
					staticUserColor = abs(staticUserColor - 1);
					staticCurrentTurn = abs(staticCurrentTurn - 1);
				}
				else{ /*the given move is legal*/

					userBestMove = init_new_movesList(-1, -1);
					userBestMove->start->next = init_new_move(-1, -1);
					userBestMove->end = userBestMove->start->next;

					printf("%d\n", getScoreCommand_alphaBeta_maximize(board, -10000000, 10000000, userDepthMinmax, 0, currentColor, userBestMove, userBest, userMove, 1));

					staticUserColor = abs(staticUserColor - 1);
					staticCurrentTurn = abs(staticCurrentTurn - 1);
					free_movesList(userBestMove);
					free(userBestMove);
					free_movesList(userMove);
					free(userMove);

				}
			}
			else{
				print_message(ILLEGAL_COMMAND);
				staticCurrentTurn = abs(staticCurrentTurn - 1);
				staticUserColor = abs(staticUserColor - 1);

				while ((input = getchar()) != EOF && input != '\n'){
					if (scanf("%s", action) != 1) {
						perror("Error: standard function scanf has failed");
						exit(1);
					}
				}



			}
		}
		else if (strcmp(action, "quit") == 0){ /*quit the game*/
			exit(0);
		}
		else if (strcmp(action, "save") == 0){ /*save the game*/
			save_game(board, 1, staticMinMaxDepth, best, staticUserColor, NULL);
			staticCurrentTurn = abs(staticCurrentTurn - 1);
			staticUserColor = abs(staticUserColor - 1);
		}
		else{
			while ((input = getchar()) != EOF && input != '\n'){
				if (scanf("%s", action) != 1) {
					perror("Error: standard function scanf has failed");
					exit(1);
				}
			}
			print_message(ILLEGAL_COMMAND);
			staticCurrentTurn = abs(staticCurrentTurn - 1);
			staticUserColor = abs(staticUserColor - 1);
		}
		staticCurrentTurn = abs(staticCurrentTurn - 1);  /*switch to other player's turn*/
		staticUserColor = abs(staticUserColor - 1);  /*switch to other player's color*/

		if (staticUserColor == 0){
			colorForPrint = "White";
		}
		else{
			colorForPrint = "Black";
		}
		if (printf("%s player - enter your move:\n", colorForPrint) < 0) {
			perror("Error: standard function printf has failed");
			exit(1);
		}
		if (scanf("%s", action) != 1) {
			perror("Error: standard function scanf has failed");
			exit(1);
		}
	}
}


void game_state_playerVsAI(char board[BOARD_SIZE][BOARD_SIZE]){
	/* game state - Player vs. AI */

	char action[60] = " ";
	int currentTurn = 0;
	int i, j;
	int input;
	char* playerColor = "white";
	char* computerColor = "black";
	char* currentColor;
	char* enemyColorForPrint;
	char* enemyColor;

	char* playerColorForPrint = "White";
	char* computerColorForPrint = "Black";
	int userDepthMinmax;
	int userBest = 0;
	int depthToCheck;

	MovesList* userMove;
	MovesList* computerBestMove;
	MovesList* userBestMove;
	bestMovesForBoard* bestMoves;

	if (staticUserColor == 1){
		playerColor = "black";
		playerColorForPrint = "Black";
		computerColor = "white";
		computerColorForPrint = "White";
	}

	if (staticCurrentTurn == 1){
		currentTurn = 1;
	}

	if (staticCurrentTurn == 0){
		currentColor = "white";
		enemyColor = "black";
		enemyColorForPrint = "Black";

	}
	else{
		currentColor = "black";
		enemyColor = "white";
		enemyColorForPrint = "White";


	}

	if (areTherePossibleMoves(board, currentColor) == 0){

		if (check_if_color_can_Mate(board, enemyColor) == 1){ /*Mate: player enemyColor wins*/
			if (printf("Mate! %s player wins the game\n", enemyColorForPrint) < 0) {
				perror("Error: standard function printf has failed");
				exit(1);
			}
			return;
		}
		else{
			if (printf("The game ends in a tie\n") < 0) {
				perror("Error: standard function printf has failed");
				exit(1);
			}
			return;
		}
	}



	while (strcmp(action, "quit") != 0){
		if (staticCurrentTurn == currentTurn&& staticUserColor == staticCurrentTurn) { /*if the player begin*/
			if (printf("%s player - enter your move:\n", playerColorForPrint) < 0) {
				perror("Error: standard function printf has failed");
				exit(1);
			}
			if (scanf("%s", action) != 1) {
				perror("Error: standard function scanf has failed");
				exit(1);
			}

			if (strcmp(action, "move") == 0){ /*make a move*/
				userMove = init_new_movesList(-1, -1);
				if (create_userMove(board, userMove, playerColor) == 0){ /*move wasn't created*/
					currentTurn = abs(currentTurn - 1);
					staticCurrentTurn = abs(staticCurrentTurn - 1);
				}
				else if (is_legal_move(board, userMove, playerColor, 1) == 0){ /*move created is not legal*/
					currentTurn = abs(currentTurn - 1);
					staticCurrentTurn = abs(staticCurrentTurn - 1);
				}
				else{ /*the given move is legal - make the move*/
					change_board_one_move(board, board, userMove->start->col, userMove->start->row, userMove->end);
					print_board(board);

					if (check_if_color_can_Mate(board, playerColor) == 1){ /*Mate: player wins*/
						if (printf("Mate! %s player wins the game\n", playerColorForPrint) < 0) {
							perror("Error: standard function printf has failed");
							exit(1);
						}
						free_movesList(userMove);
						free(userMove);
						break;
					}

					else if (areTherePossibleMoves(board, computerColor) == 0){ /*if there aren't any possible moves*/
						if (check_if_color_can_Check(board, playerColor) == 1){
							if (printf("Mate! %s player wins the game\n", playerColorForPrint) < 0) {
								perror("Error: standard function printf has failed");
								exit(1);
							}
							free_movesList(userMove);
							free(userMove);
							break;
						}
						else{
							if (printf("The game ends in a tie\n") < 0){
								perror("Error: standard function printf has failed");
								exit(1);
							}
							free_movesList(userMove);
							free(userMove);
							break;
						}
					}
					else if (check_if_color_can_Check(board, playerColor) == 1){ /*Check: player of currentColor threatens the other player's king*/
						if (printf("Check!\n") < 0) {
							perror("Error: standard function printf has failed");
							exit(1);
						}
					}

				}
				free_movesList(userMove);
				free(userMove);
			}


			else if (strcmp(action, "get_moves") == 0){ /*print the possible moves for the piece in the requested position */
				if (scanf("%s", action) != 1) {
					perror("Error: standard function scanf has failed");
					exit(1);
				}
				j = (int)(action[1] - 'a');
				i = (int)(action[3] - '0') - 1;
				print_possible_moves_for_piece(board, j, i, playerColor);
				currentTurn = abs(currentTurn - 1);
				staticCurrentTurn = abs(staticCurrentTurn - 1);
			}
			else if (strcmp(action, "quit") == 0){  /*quit the game*/
				exit(0);
			}
			else if (strcmp(action, "save") == 0){	/*save the game*/
				save_game(board, 2, staticMinMaxDepth, best, staticUserColor, NULL);
				currentTurn = abs(currentTurn - 1);
				staticCurrentTurn = abs(staticCurrentTurn - 1);
			}
			else if (strcmp(action, "get_best_moves") == 0){ /*print the best moves for the current player*/
				if (scanf("%s", action) != 1) {
					perror("Error: standard function scanf has failed");
					exit(1);
				}
				if (strcmp(action, "best") == 0){
					userBest = 1;
					userDepthMinmax = 4;
				}
				else{
					depthToCheck = atoi(action);

					if (depthToCheck < 1 || depthToCheck>4){
						print_message(WRONG_MINIMAX_DEPTH);

					}
					else{
						userDepthMinmax = depthToCheck;
					}
				}

				bestMoves = init_new_bestMovesForBoard(-1, -1);

				userBestMove = init_new_movesList(-1, -1);
				userBestMove->start->next = init_new_move(-1, -1);
				userBestMove->end = userBestMove->start->next;


				alphaBeta_maximize(board, -10000000, 10000000, userDepthMinmax, 0, playerColor, userBestMove, userBest, 1, bestMoves);
				print_bestMovesList(getMyBest());
				currentTurn = abs(currentTurn - 1);
				staticCurrentTurn = abs(staticCurrentTurn - 1);
				free_movesList(userBestMove);
				free(userBestMove);
				
				
				
				freeStatic();
				/**free(bestMoves);**/
			}

			else if (strcmp(action, "get_score") == 0){ /*print the score of a specific move*/
				if (scanf("%s", action) != 1) {
					perror("Error: standard function scanf has failed");
					exit(1);
				}
				if (strcmp(action, "best") == 0){
					userBest = 1;
					userDepthMinmax = 4;
				}
				else{
					depthToCheck = atoi(action);

					if (depthToCheck < 1 || depthToCheck>4){
						print_message(WRONG_MINIMAX_DEPTH);

					}
					else{
						userDepthMinmax = depthToCheck;
					}
				}
				if (scanf("%s", action) != 1) {
					perror("Error: standard function scanf has failed");
					exit(1);
				}

				if (strcmp(action, "move") == 0){ /*make a move*/

					userMove = init_new_movesList(-1, -1);
					if (create_userMove(board, userMove, playerColor) == 0){ /*move wasn't created*/
						currentTurn = abs(currentTurn - 1);
						staticCurrentTurn = abs(staticCurrentTurn - 1);
					}
					else if (is_legal_move(board, userMove, playerColor, 1) == 0){ /*move created is not legal*/
						currentTurn = abs(currentTurn - 1);
						staticCurrentTurn = abs(staticCurrentTurn - 1);
					}
					else{ /*the given move is legal*/

						userBestMove = init_new_movesList(-1, -1);
						userBestMove->start->next = init_new_move(-1, -1);
						userBestMove->end = userBestMove->start->next;

						printf("%d\n", getScoreCommand_alphaBeta_maximize(board, -10000000, 10000000, userDepthMinmax, 0, playerColor, userBestMove, userBest, userMove, 1));
						currentTurn = abs(currentTurn - 1);
						staticCurrentTurn = abs(staticCurrentTurn - 1);
						free_movesList(userBestMove);
						free(userBestMove);
						free_movesList(userMove);
						free(userMove);

					}
				}
				else{
					print_message(ILLEGAL_COMMAND);
					currentTurn = abs(currentTurn - 1);
					staticCurrentTurn = abs(staticCurrentTurn - 1);
					while ((input = getchar()) != EOF && input != '\n'){
						if (scanf("%s", action) != 1) {
							perror("Error: standard function scanf has failed");
							exit(1);
						}
					}

				}
			}
			else{
				while ((input = getchar()) != EOF && input != '\n'){
					if (scanf("%s", action) != 1) {
						perror("Error: standard function scanf has failed");
						exit(1);
					}
				}
				print_message(ILLEGAL_COMMAND);
				currentTurn = abs(currentTurn - 1);
				staticCurrentTurn = abs(staticCurrentTurn - 1);
			}
			currentTurn = abs(currentTurn - 1);
			staticCurrentTurn = abs(staticCurrentTurn - 1);
		}

		else{	/*computer's turn*/
			computerBestMove = init_new_movesList(-1, -1);
			computerBestMove->start->next = init_new_move(-1, -1);
			computerBestMove->end = computerBestMove->start->next;

			if (best == 1) {
				staticMinMaxDepth = 4;
			}
			alphaBeta_maximize(board, -10000000, 10000000, staticMinMaxDepth, 0, computerColor, computerBestMove, best, 0, NULL);
			if (printf("Computer: move ") < 0) {
				perror("Error: standard function printf has failed");
				exit(1);
			}
			print_movesList(computerBestMove);
			change_board_one_move(board, board, (computerBestMove)->start->col, (computerBestMove)->start->row, (computerBestMove)->start->next);
			print_board(board);
			free_movesList(computerBestMove);
			free(computerBestMove);

			if (check_if_color_can_Mate(board, computerColor) == 1){ /*Mate: computer wins*/
				if (printf("Mate! %s player wins the game\n", computerColorForPrint) < 0) {
					perror("Error: standard function printf has failed");
					exit(1);
				}
				break;
			}
			else if (areTherePossibleMoves(board, playerColor) == 0){ /*if there aren't any possible moves*/
				if (check_if_color_can_Check(board, computerColor) == 1){
					if (printf("Mate! %s player wins the game\n", computerColorForPrint) < 0) {
						perror("Error: standard function printf has failed");
						exit(1);
					}
					break;
				}
				else{
					if (printf("The game ends in a tie\n") < 0) { /*tie*/
						perror("Error: standard function printf has failed");
						exit(1);
					}
					break;
				}
			}
			else if (check_if_color_can_Check(board, computerColor) == 1){ /*Check: computer threatens the player's king*/
				if (printf("Check!\n") < 0) {
					perror("Error: standard function printf has failed");
					exit(1);
				}
			}

			currentTurn = abs(currentTurn - 1);	/*switch to player's turn*/
			staticCurrentTurn = abs(staticCurrentTurn - 1);
		}
	}
}


void settings_state(char board[BOARD_SIZE][BOARD_SIZE]){
	/* sets the game settings */

	char user_command[60] = { ' ' };
	int minMaxDepth = 1; /*default minmax depth*/
	int mode = 1; /*default mode*/
	int i, j;
	char position[20];
	int legal_start = 0;

	int col, row;
	char playerColor[20] = "white";
	char computerColor[20] = "black";
	int isLoad = 0;
	char firstPlayerColor[20] = "white"; /*default */
	char pieceColor[20];
	char pieceType[20];
	int input;
	int errorFlag;
	int numWhiteKing = 0;
	int numBlackKing = 0;


	if (printf("Enter game settings:\n") < 0){
		perror("Error: standard function printf has failed");
		exit(1);
	}



	if (scanf("%59s", user_command) != 1) { /*read next command*/
		perror("Error: standard function scanf has failed");
		exit(1);
	}


	do{
		errorFlag = 0;
		if (strcmp("game_mode", user_command) == 0){ /*set game mode - 1: two players , 2: player vs. AI*/
			if (scanf("%s", user_command) != 1){
				perror("Error: standard function scanf has failed");
				exit(1);
			}

			if (isLoad == 0){
				if (strcmp(user_command, "1") == 0){ /*two players*/
					mode = 1;
					staticMode = 1;
					print_message(TWO_PLAYERS_GAME_MODE);
				}
				else if (strcmp(user_command, "2") == 0){ /*player vs. AI*/
					mode = 2;
					staticMode = 2;
					print_message(PLAYER_VS_AI_GAME_MODE);
				}
				else{
					print_message(WRONG_GAME_MODE);
				}
			}
			else{
				print_message(ILLEGAL_COMMAND);
			}

		}

		else if (strcmp(user_command, "difficulty") == 0){ /*set minmax depth - only avaliable in mode 2 (player vs. AI)*/
			if (scanf("%s", user_command) != 1) {
				perror("Error: standard function scanf has failed");
				exit(1);
			}
			if (mode == 2){

				if (strcmp(user_command, "depth") == 0){
					if (scanf("%d", &minMaxDepth) != 1) {
						perror("Error: standard function scanf has failed");
						exit(1);
					}
					if (minMaxDepth < 1 || minMaxDepth>4){
						print_message(WRONG_MINIMAX_DEPTH);

					}
					else{
						staticMinMaxDepth = minMaxDepth;
					}

				}
				else if (strcmp(user_command, "best") == 0){
					best = 1;
				}
				else{
					print_message(ILLEGAL_COMMAND);
				}
			}
			else{
				print_message(ILLEGAL_COMMAND);
				while ((input = getchar()) != EOF && input != '\n'){
					if (scanf("%s", user_command) != 1) {
						perror("Error: standard function scanf has failed");
						exit(1);
					}
				}
			}
		}

		else if (strcmp("user_color", user_command) == 0){ /*set user color - only avaliable in mode 2 (player vs. AI)*/
			if (scanf("%19s", playerColor) != 1) {
				perror("Error: standard function scanf has failed");
				exit(1);
			}
			if (mode == 2){

				if (strcmp(playerColor, "white") == 0){
					staticUserColor = 0;
					strcpy(computerColor, "black");

				}
				else if (strcmp(playerColor, "black") == 0){
					staticUserColor = 1;
					strcpy(computerColor, "white");

				}  /*we assume that the color input is valid - black/white only*/
			}
			else{
				if (errorFlag == 0){
					print_message(ILLEGAL_COMMAND);
					errorFlag = 1;
				}

			}
		}

		else if (strcmp(user_command, "load") == 0){	/*load game*/
			if (load_game(board, &staticMode, &staticMinMaxDepth, &best, &staticUserColor, &staticCurrentTurn, NULL) == 0){
				isLoad = 1;
				print_board(board);
			}
		}
		else if (strcmp("clear", user_command) == 0){ /*clear the game board*/
			for (i = 0; i < BOARD_SIZE; i++) {
				for (j = 0; j < BOARD_SIZE; j++) {
					remove_disc(board, j, i);
				}
			}
		}

		else if (strcmp(user_command, "next_player") == 0){
			if (scanf("%19s", firstPlayerColor) != 1) {
				perror("Error: standard function scanf has failed");
				exit(1);
			}
			if (strcmp(firstPlayerColor, "white") == 0){
				staticCurrentTurn = 0;
			}
			else if (strcmp(firstPlayerColor, "black") == 0){
				staticCurrentTurn = 1;
			} /*we assume that the color input is valid - black/white only*/
		}

		else if (strcmp("rm", user_command) == 0) { /*remove a piece from the given position*/

			if (scanf("%19s", position) != 1) {
				perror("Error: standard function scanf has failed");
				exit(1);
			}
			if (strcmp("", position) == 0){ /*if no position is given, do nothing and continue*/
				continue;
			}

			col = (int)(position[1] - 'a');
			row = (int)(position[3] - '0') - 1;

			if (position[4] != '>'){
				print_message(WRONG_POSITION);
			}
			else{

				if (is_valid_position_on_board(col, row) == 1){ /*if the position is valid, remove the piece*/
					board[col][row] = EMPTY;
				}
				else { /*invalid*/
					print_message(WRONG_POSITION);
				}
			}
		}

		else if (strcmp(user_command, "set") == 0){ /*set a piece on the game board*/

			if (scanf("%19s", position) != 1) {
				perror("Error: standard function scanf has failed");
				exit(1);
			}
			if (scanf("%19s", pieceColor) != 1) {
				perror("Error: standard function scanf has failed");
				exit(1);
			}
			if (scanf("%19s", pieceType) != 1) {
				perror("Error: standard function scanf has failed");
				exit(1);
			}
			col = (int)(position[1] - 'a');
			row = (int)(position[3] - '0') - 1;

			if (position[4] != '>'){
				print_message(WRONG_POSITION);
			}
			else{

				if (is_valid_position_on_board(col, row) == 1){ /*if the position is valid, set the piece*/
					set(board, col, row, pieceColor, pieceType,1);
				}
				else { /*invalid*/
					print_message(WRONG_POSITION);
				}
			}
		}
		else if (strcmp(user_command, "print") == 0){ /*print the current game board*/
			print_board(board);
		}


		else if (strcmp("quit", user_command) == 0){
			return;
		}

		else if (strcmp(user_command, "start") == 0){

			/*start the game - play in game state*/

			for (i = 0; i < BOARD_SIZE; ++i) {
				for (j = 0; j < BOARD_SIZE; ++j) {
					if (board[j][i] == WHITE_K){
						numWhiteKing += 1;
					}
					else if (board[j][i] == BLACK_K){
						numBlackKing += 1;
					}
				}
			}
			if (numBlackKing < 1 || numWhiteKing < 1) { /*illegal to play on this game board*/
				print_message(WROND_BOARD_INITIALIZATION);
				numWhiteKing = 0;
				numBlackKing = 0;
			}
			else {
				legal_start = 1;	/*game can start*/
				if (mode == 1){
					if (staticCurrentTurn == 0){
						staticUserColor = 0;
					}
					else if (staticCurrentTurn == 1){
						staticUserColor = 1;
					}
					game_state_twoPlayers(board);
				}
				else if (mode == 2){
					game_state_playerVsAI(board);
				}
				break;	/*game over*/
			}
		}
		else {	/*if illegal command*/
			while ((input = getchar()) != EOF && input != '\n'){
				if (scanf("%s", user_command) != 1) {
					perror("Error: standard function scanf has failed");
					exit(1);
				}
			}
			if (errorFlag == 0) {
				print_message(ILLEGAL_COMMAND);
				errorFlag = 1;
			}
		}



		if (printf("Enter game settings:\n") < 0){
			perror("Error: standard function printf has failed");
			exit(1);
		}

		if (scanf("%s", user_command) != 1) {
			perror("Error: standard function scanf has failed");
			exit(1);
		}



	} while (legal_start == 0);
}


void game_on_console(){

	char board[BOARD_SIZE][BOARD_SIZE];
	init_board(board);
	print_board(board);
	settings_state(board);

}

int main(int argc, char* argv[]){

	if (argv[1] == NULL){
		game_on_console();
	}
	else if (strcmp(argv[1], "") == 0){
		game_on_console();
	}
	else if (strcmp(argv[1], "console") == 0){
		game_on_console();
	}

	else if (strcmp(argv[1], "gui") == 0){
		gui_mode();
	}

	return 0;

}
