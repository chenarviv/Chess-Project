#include "Gui.h"
#include "Moves.h"
#include "minmax.h"

SDL_Event e;
SDL_Surface *window;
SDL_Rect offset;
int buttonSettingsint[NUM_OF_BUTTONS][NUM_OF_DATA];

int colorBoard[BOARD_SIZE][BOARD_SIZE] = { { 0, 1, 0, 1, 0, 1, 0, 1 }, { 1, 0, 1, 0, 1, 0, 1, 0 }, { 0, 1, 0, 1, 0, 1, 0, 1 }, { 1, 0, 1, 0, 1, 0, 1, 0 },
{ 0, 1, 0, 1, 0, 1, 0, 1 }, { 1, 0, 1, 0, 1, 0, 1, 0 }, { 0, 1, 0, 1, 0, 1, 0, 1 }, { 1, 0, 1, 0, 1, 0, 1, 0 } };


static int best = 0;
static int staticMinMaxDepth = 1;
static int staticMode = 1;
static int staticCurrentTurn = 0;  /*0-white, 1-black*/
static int staticUserColor = 0;     /*0-white, 1-black*/

char* slotImages[NUM_OF_COLORS];

int gui_mode(){
	char board[BOARD_SIZE][BOARD_SIZE];
	useSpace = (toFreeList*)calloc(1, sizeof(toFreeList));
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return 1;
	}
	useSurfaceSpace = (free_surface_list*)calloc(1, sizeof(free_surface_list));
	
	init_board(board);
	SDL_putenv("SDL_VIDEO_CENTERED=center");
	atexit(SDL_Quit);
	buttonSetting(buttonSettingsint);
	gui_settings(board);
	freeUsedList();
	free_used_surface_list();
	return 0;
}
SDL_Surface * loadImg(char* path){
	SDL_Surface *temp = NULL;
	SDL_Surface * ans = NULL;
	temp = SDL_LoadBMP(path);
	if (temp == NULL){
		return NULL;
	}
	ans = SDL_DisplayFormat(temp);
	add_to_user_surface_list(ans);
	SDL_FreeSurface(temp);

	if (ans == NULL) {
		printf("ERROR: failed to load image: %s\n", SDL_GetError());
		return NULL;
	}
	return ans;
}
/*SDL_Surface * loadPngImg(char* path){
SDL_Surface * ans = NULL;
ans = IMG_Load(path);
if (ans == NULL) {
printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
}
addToUserUsedSurfaceList(ans);
return ans;
}*/
int addImage(SDL_Surface * img, SDL_Rect offset, int imgW, int imgH){
	SDL_Rect imgrect;
	imgrect.x = 0;
	imgrect.y = 0;
	imgrect.w = imgW;
	imgrect.h = imgH;
	if (SDL_BlitSurface(img, &(imgrect), window, &offset) != 0) {
		SDL_FreeSurface(img);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		return 1;
	}
	return 0;
}
int SettingWindowAddRemove(char board[BOARD_SIZE][BOARD_SIZE], int legalStart,int isSqure){
	int lastWhite = 0;
	SDL_Surface *whiteSquareImg;
	SDL_Surface *blackSquareImg;
	SDL_Surface *chessImg;
	SDL_Surface *clearImg;
	SDL_Surface *removeImg;
	SDL_Surface *setImg;
	SDL_Surface *illegalImg;
	SDL_Surface *cancelImg;
	SDL_Surface *chooseImg;
	SDL_Surface *quitGameImg;
	SDL_Rect offset;
	window = SDL_SetVideoMode(GAME_WINDOW_W, GAME_WINDOW_H, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if (window == NULL) {
		printf("ERROR: failed to set video mode: %s\n", SDL_GetError());
		return 1;
	}

	SDL_WM_SetCaption("Game window", NULL);
	if (!legalStart){
		illegalImg = loadImg(IMG_ILLEGAL_START);
	}
		
	whiteSquareImg = loadImg(IMG_WHITE_SQUARE);
	blackSquareImg = loadImg(IMG_BLACK_SQUARE);
	chessImg = loadImg(IMG_CHESS_LOGO_2);
	clearImg = loadImg(IMG_CLEAR_BOARD);
	removeImg = loadImg(IMG_REMOVE_GAME);
	cancelImg = loadImg(IMG_CANCEL_SET);
	setImg = loadImg(IMG_SET_ON_BOARD);
	chooseImg = loadImg(CHOOSE_SET);

	quitGameImg = loadImg(SET_BACK);
	SDL_FillRect(window, NULL, 0xFFFFFF);
	for (offset.x = SQUARE_HIGHT_AND_WIDTH; offset.x <= SQUARE_HIGHT_AND_WIDTH * 8; offset.x += SQUARE_HIGHT_AND_WIDTH){
		for (offset.y = SQUARE_HIGHT_AND_WIDTH; offset.y <= SQUARE_HIGHT_AND_WIDTH * 8; offset.y += SQUARE_HIGHT_AND_WIDTH){
			if (lastWhite){
				addImage(blackSquareImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
			}
			else{
				addImage(whiteSquareImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
			}
			lastWhite = !lastWhite;
		}
		lastWhite = !lastWhite;
	}
	offset.y = CHESS_LOGO_2_Y;
	offset.x = CHESS_LOGO_2_X;

	addImage(chessImg, offset, CHESS_LOGO_2_WIDTH, CHESS_LOGO_2_HIGHT);

	if (!legalStart){
		offset.x = 600;
		addImage(illegalImg, offset, ILLEGAL_W, ILLEGAL_H);
	
	}
	if (!isSqure){
		offset.x = 600;
		addImage(chooseImg, offset, CHOOSE_SET_W, CHOOSE_SET_H);

	}
	offset.x = GAME_BUTTON_WINDOW_X;
	offset.y = SET_BUTTON_Y;
	addImage(setImg, offset, CHESS_LOGO_2_WIDTH, CHESS_LOGO_2_HIGHT);
	offset.y = SAVE_BUTTON_Y;
	addImage(clearImg, offset, IMG_W, IMG_H);
	offset.y = MAIN_MENU_BUTTON_Y;
	addImage(removeImg, offset, IMG_W, IMG_H);
	offset.y = CANCEL_SET_Y;
	addImage(cancelImg, offset, IMG_W, IMG_H);
	offset.y = QUIT_GAME_BUTTON_Y;
	addImage(quitGameImg, offset, IMG_W, IMG_H);
	addBoardToGameWindow(board, NULL);
	/* We finished drawing*/
	if (SDL_Flip(window) != 0) {
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		return 1;
	}
	return 0;
}

int make_main_menu(){
	SDL_Surface *newLoadImg = NULL;
	SDL_Surface *newGameImg = NULL;
	SDL_Surface *quitImg = NULL;
	SDL_Surface *chessImg = NULL;
	window = SDL_SetVideoMode(MAIN_MENU_W, MAIN_MENU_H, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if (window == NULL) {
		printf("ERROR: failed to set video mode: %s\n", SDL_GetError());
		return 1;
	}
	SDL_WM_SetCaption("Main Menu", NULL);

	newGameImg = loadImg(IMG_NEW_GAME);
	newLoadImg = loadImg(IMG_LOAD_GAME);
	quitImg = loadImg(IMG_QUIT_GAME);
	chessImg = loadImg(IMG_CHESS_LOGO_2);
	SDL_FillRect(window, NULL, 0xFFFFFF);
	offset.x = X;
	offset.y = CHESS_IMG_Y;
	addImage(chessImg, offset, IMG_W, IMG_H);
	offset.y = NEW_GAME_Y;
	addImage(newGameImg, offset, IMG_W, IMG_H);
	offset.y = LOAD_GAME_Y;
	addImage(newLoadImg, offset, IMG_W, IMG_H);
	offset.y = QUIT_GAME_Y;
	addImage(quitImg, offset, IMG_W, IMG_H);
	/* We finished drawing*/
	if (SDL_Flip(window) != 0) {
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		return 1;
	}
	return 0;
}
void gui_settings(char board[BOARD_SIZE][BOARD_SIZE]){

	int quit = 0;

	if (!make_main_menu()){
	}
	while (quit != 1){
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
			case (SDL_MOUSEBUTTONDOWN) :
				if (pressed_button(e.button, NEW_GAME_NUM)){
					init_board(board);
					players_selection_window();
					HandleSelectionEventInPlayerSelection(board);
					quit = 1;
					break;
				}
									   if (pressed_button(e.button, QUIT_GAME_NUM)){
										   quit = 1;
										   break;
									   }
									   if (pressed_button(e.button, LOAD_GAME_NUM)){
										   quit = 1;
										   saveOrLoad_window(LOAD);
										   if (HandleLoadWindow(board) == SHOULD_START_GAME){
											   makeGameWindow(board, NULL);

											   if (staticCurrentTurn == 0){

												   HandleSelectionEventInGameWindow(board, "white");
											   }
											   else if (staticCurrentTurn == 1){

												   HandleSelectionEventInGameWindow(board, "black");
											   }
										   }
										   quit = 1;
										   break;
									   }
									   break;
			default:
				break;
			}
		}
		SDL_Delay(50);
	}
}

int pressed_button(SDL_MouseButtonEvent event, int button){
	int x = buttonSettingsint[button][0];
	int y = buttonSettingsint[button][1];
	int w = buttonSettingsint[button][2];
	int h = buttonSettingsint[button][3];
	if (event.x <= x + w && event.x >= x &&event.y <= y + h && event.y >= y){
		return 1;
	}
	return 0;
}

int players_selection_window(){
	SDL_Surface *playerVsPlayerImg = NULL;
	SDL_Surface *playerVsComputerImg = NULL;
	SDL_Surface *settingsImg = NULL;
	SDL_Surface *cancelImg = NULL;
	SDL_Surface *nextPlayer = NULL;
	SDL_Surface *blackNextPlayer = NULL;
	SDL_Surface *modeImg = NULL;
	SDL_Surface *whiteNextPlayer = NULL;
	SDL_Surface *startImg = NULL;
	SDL_Surface *addReSettings = NULL;
	SDL_Rect offset;

	window = SDL_SetVideoMode(PLAYERS_SELECTION_W, PLAYERS_SELECTION_H, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if (window == NULL) {
		printf("ERROR: failed to set video mode: %s\n", SDL_GetError());
		return 1;
	}
	SDL_WM_SetCaption("Players Selection Window", NULL);
	playerVsPlayerImg = loadImg(IMG_PLAYER_VS_PLAYER);
	playerVsComputerImg = loadImg(IMG_PLAYER_VS_COMPUTER);
	modeImg = loadImg(MODE);
	addReSettings = loadImg(SET_REMOVE_IMG);
	settingsImg = loadImg(SETTINGS);
	nextPlayer = loadImg(NEXT_PLAYER_SET);
	blackNextPlayer = loadImg(BLACK_NEXT_PLAYER);
	whiteNextPlayer = loadImg(WHITE_NEXT_PLAYER);
	cancelImg = loadImg(IMG_CANCEL);
	startImg = loadImg(IMG_START_PLAYER_SELECTION);

	SDL_FillRect(window, NULL, 0xFFFFFF);
	offset.x = X + 45;
	offset.y = 15;
	addImage(modeImg, offset, GAME_MODE_W, GAME_MODE_H);
	offset.x = X + 45;
	offset.y = ADD_RE_SET_Y;
	addImage(addReSettings, offset, GAME_MODE_W, GAME_MODE_H);

	offset.x = X + 30;
	offset.y = PLAYER_VS_PLAYER_Y;
	addImage(playerVsPlayerImg, offset, IMG_W, IMG_H);
	offset.x = X + 220;

	addImage(playerVsComputerImg, offset, IMG_W, IMG_H);
	offset.y = SETTING_Y;
	offset.x = X + 120;
	addImage(settingsImg, offset, IMG_W, IMG_H);
	offset.y = NEXT_PLAYER_COLOR_Y;
	offset.x = WHITE_NEXT_X;
	addImage(whiteNextPlayer, offset, IMG_W, IMG_H);
	offset.x = BLACK_NEXT_X;
	addImage(blackNextPlayer, offset, IMG_W, IMG_H);
	offset.x = X + 45;
	offset.y = NEXT_PLAYER_Y;
	addImage(nextPlayer, offset, GAME_MODE_W, GAME_MODE_H);
	offset.x = cancel_x;
	offset.y = CANCEL_Y;
	addImage(cancelImg, offset, IMG_W, IMG_H);
	offset.x = START_SELECT_X;
	addImage(startImg, offset, IMG_W, IMG_H);
	/* We finished drawing*/
	if (SDL_Flip(window) != 0) {
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		return 1;
	}
	return 0;
}



int HandleSelectionEventInPlayerSelection(char board[BOARD_SIZE][BOARD_SIZE]){
	int quit = 0;
	while (quit != 1){
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
				/*case (SDL_QUIT):
				quit = 1;
				break;*/
			case (SDL_MOUSEBUTTONDOWN) :
				if (pressed_button(e.button, PLAYER_VS_PLAYER_NUM)){
					staticMode = 1;

					break;
				}
									   if (pressed_button(e.button, PLAYER_VS_COMPUTER_NUM)){
										   staticMode = 2;
										   break;
									   }

									   if (pressed_button(e.button, SETTINGS_NUM)){
										   SettingWindowAddRemove(board, 1,1);
										   HandleSelectionEventInaddRemoveWindow(board);
										   quit = 1;
										   break;


									   }
									   if (pressed_button(e.button, WHITE_NEXT_NUM)){
										   staticCurrentTurn = 0;
										   break;
									   }
									   if (pressed_button(e.button, BLACK_NEXT_NUM)){
										   staticCurrentTurn = 1;
										   break;
									   }if (pressed_button(e.button, START_SELECT_PLAYER)){
										   if (staticMode == 2){
											   setting_window();

											   if (staticUserColor == 0){
												   HandleSelectionEventInSettings(board);
											   }
											   else if (staticUserColor == 1){
												   HandleSelectionEventInSettings(board);
											   }

											   quit = 1;
											   break;
										   }
										   else{
											   SettingForPlayervsPlayerWindow();
											   HandleSelectionEventInPlayerVsPlayerSettings(board);

											   quit = 1;
											   break;

										   }
									   }
									   if (pressed_button(e.button, CANCEL_NUM)){
										   staticCurrentTurn = 0;
										   staticMode = 1;
										   gui_settings(board);
										   quit = 1;
										   break;
									   }
									   break;

			default:
				break;
			}
		}
		SDL_Delay(50);
	}
	return 0;
}


int SettingForPlayervsPlayerWindow(){
	SDL_Surface *difficultyImg = NULL;
	SDL_Surface *oneImg = NULL;
	SDL_Surface *twoImg = NULL;
	SDL_Surface *threeImg = NULL;
	SDL_Surface *fourImg = NULL;
	SDL_Surface *bestDepthImg = NULL;
	SDL_Surface *startImg = NULL;
	SDL_Rect offset;

	window = SDL_SetVideoMode(SETTING_WINDOW_W, SETTING_WINDOW_H, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if (window == NULL) {
		printf("ERROR: failed to set video mode: %s\n", SDL_GetError());
		return 1;
	}
	SDL_FillRect(window, NULL, 0xFFFFFF);
	SDL_WM_SetCaption("Settings Window", NULL);

	/*load images*/
	difficultyImg = loadImg(IMG_Difficulty);
	oneImg = loadImg(IMG_ONE);
	twoImg = loadImg(IMG_TWO);
	threeImg = loadImg(IMG_THREE);
	fourImg = loadImg(IMG_FOUR);
	bestDepthImg = loadImg(IMG_BEST_DEPTH);
	startImg = loadImg(IMG_START);


	offset.x = DIFFICULTY_X;
	offset.y = DIFFICULTY_Y;
	addImage(difficultyImg, offset, IMG_W, IMG_H);
	offset.x = NUMBER_1_X;
	offset.y = NUMBER_Y;
	addImage(oneImg, offset, NUMBER_WIDTH, NUMBER_HIGHT);
	offset.x = NUMBER_2_X;
	addImage(twoImg, offset, NUMBER_WIDTH, NUMBER_HIGHT);
	offset.x = NUMBER_3_X;
	addImage(threeImg, offset, NUMBER_WIDTH, NUMBER_HIGHT);
	offset.x = NUMBER_4_X;
	addImage(fourImg, offset, NUMBER_WIDTH, NUMBER_HIGHT);

	offset.y = BEST_DIFFCULTY_Y;
	offset.x = BEST_DIFFCULTY_X;
	addImage(bestDepthImg, offset, BEST_DIFFCULTY_WIDTH, BEST_DIFFCULTY_HIGHT);

	offset.x = START_X;
	offset.y = START_Y;
	addImage(startImg, offset, IMG_W, IMG_H);
	/* We finished drawing*/
	if (SDL_Flip(window) != 0) {
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		return 1;
	}

	return 0;
}

int setting_window(){
	SDL_Surface *difficultyImg = NULL;
	SDL_Surface *oneImg = NULL;
	SDL_Surface *twoImg = NULL;
	SDL_Surface *threeImg = NULL;
	SDL_Surface *fourImg = NULL;
	SDL_Surface *bestDepthImg = NULL;
	SDL_Surface *gameColorImg = NULL;
	SDL_Surface *whiteImg = NULL;
	SDL_Surface *blackImg = NULL;
	SDL_Surface *startImg = NULL;
	SDL_Rect offset;

	window = SDL_SetVideoMode(SETTING_WINDOW_W, SETTING_WINDOW_H, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if (window == NULL) {
		printf("ERROR: failed to set video mode: %s\n", SDL_GetError());
		return 1;
	}
	SDL_FillRect(window, NULL, 0xFFFFFF);
	SDL_WM_SetCaption("Settings Window", NULL);

	/*load images*/
	difficultyImg = loadImg(IMG_Difficulty);
	oneImg = loadImg(IMG_ONE);
	twoImg = loadImg(IMG_TWO);
	threeImg = loadImg(IMG_THREE);
	fourImg = loadImg(IMG_FOUR);
	bestDepthImg = loadImg(IMG_BEST_DEPTH);
	gameColorImg = loadImg(IMG_GAME_COLOR);
	whiteImg = loadImg(IMG_WHITE);
	blackImg = loadImg(IMG_BLACK);
	startImg = loadImg(IMG_START);


	offset.x = DIFFICULTY_X;
	offset.y = DIFFICULTY_Y;
	addImage(difficultyImg, offset, IMG_W, IMG_H);
	offset.x = NUMBER_1_X;
	offset.y = NUMBER_Y;
	addImage(oneImg, offset, NUMBER_WIDTH, NUMBER_HIGHT);
	offset.x = NUMBER_2_X;
	addImage(twoImg, offset, NUMBER_WIDTH, NUMBER_HIGHT);
	offset.x = NUMBER_3_X;
	addImage(threeImg, offset, NUMBER_WIDTH, NUMBER_HIGHT);
	offset.x = NUMBER_4_X;
	addImage(fourImg, offset, NUMBER_WIDTH, NUMBER_HIGHT);
	offset.x = BEST_DIFFCULTY_X;
	addImage(bestDepthImg, offset, BEST_DIFFCULTY_WIDTH, BEST_DIFFCULTY_HIGHT);
	offset.x = GAME_COLOR_X;
	offset.y = GAME_COLOR_Y;
	addImage(gameColorImg, offset, GAME_COLOR_WIDTH, GAME_COLOR_HIGHT);
	offset.x = WHITE_BUTTON_X;
	offset.y = COLOR_Y;
	addImage(whiteImg, offset, COLOR_WIDTH, COLOR_HIGHT);
	offset.x = BLACK_BUTTON_X;
	addImage(blackImg, offset, COLOR_WIDTH, COLOR_HIGHT);
	offset.x = START_X;
	offset.y = START_Y;
	addImage(startImg, offset, IMG_W, IMG_H);
	/* We finished drawing*/
	if (SDL_Flip(window) != 0) {
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		return 1;
	}

	return 0;
}

/*
*data by order: x,y,w,h
*/


void  buttonSetting(int buttons[NUM_OF_BUTTONS][NUM_OF_DATA]){
	int i = 0;
	AddToArrSettings(X, NEW_GAME_Y, IMG_W, IMG_H, buttons, NEW_GAME_NUM);
	AddToArrSettings(X, LOAD_GAME_Y, IMG_W, IMG_H, buttons, LOAD_GAME_NUM);
	AddToArrSettings(X, QUIT_GAME_Y, IMG_W, IMG_H, buttons, QUIT_GAME_NUM);
	AddToArrSettings(X, PLAYER_VS_PLAYER_Y, IMG_W, IMG_H, buttons, PLAYER_VS_PLAYER_NUM);
	AddToArrSettings(X + 220, PLAYER_VS_PLAYER_Y, IMG_W, IMG_H, buttons, PLAYER_VS_COMPUTER_NUM);
	AddToArrSettings(X + 120, SETTING_Y, IMG_W, IMG_H, buttons, SETTINGS_NUM);
	AddToArrSettings(WHITE_NEXT_X, NEXT_PLAYER_COLOR_Y, IMG_W, IMG_H, buttons, WHITE_NEXT_NUM);
	AddToArrSettings(BLACK_NEXT_X, NEXT_PLAYER_COLOR_Y, IMG_W, IMG_H, buttons, BLACK_NEXT_NUM);
	AddToArrSettings(cancel_x, CANCEL_Y, IMG_W, IMG_H, buttons, CANCEL_NUM);
	AddToArrSettings(START_SELECT_X, CANCEL_Y, IMG_W, IMG_H, buttons, START_SELECT_PLAYER);
	AddToArrSettings(NUMBER_1_X, NUMBER_Y, NUMBER_WIDTH, NUMBER_HIGHT, buttons, ONE_BUTTON_NUM);
	AddToArrSettings(NUMBER_2_X, NUMBER_Y, NUMBER_WIDTH, NUMBER_HIGHT, buttons, TWO_BUTTON_NUM);
	AddToArrSettings(NUMBER_3_X, NUMBER_Y, NUMBER_WIDTH, NUMBER_HIGHT, buttons, THREE_BUTTON_NUM);
	AddToArrSettings(NUMBER_4_X, NUMBER_Y, NUMBER_WIDTH, NUMBER_HIGHT, buttons, FOUR_BUTTON_NUM);
	AddToArrSettings(WHITE_BUTTON_X, COLOR_Y, COLOR_WIDTH, COLOR_HIGHT, buttons, WHITE_BUTTON_NUM);
	AddToArrSettings(BLACK_BUTTON_X, COLOR_Y, COLOR_WIDTH, COLOR_HIGHT, buttons, BLACK_BUTTON_NUM);
	AddToArrSettings(GAME_BUTTON_WINDOW_X, SAVE_BUTTON_Y, IMG_W, IMG_H, buttons, SAVE_BUTTON_NUM);
	AddToArrSettings(GAME_BUTTON_WINDOW_X, INS_BUTTON_Y, IMG_W, IMG_H, buttons, INS_BUTTON_NUM);

	AddToArrSettings(GAME_BUTTON_WINDOW_X, MAIN_MENU_BUTTON_Y, IMG_W, IMG_H, buttons, MAIN_MENU_BUTTON_NUM);

	AddToArrSettings(GAME_BUTTON_WINDOW_X, MAIN_MENU_BUTTON_Y, IMG_W, IMG_H, buttons, REMOVE_NUM);
	AddToArrSettings(GAME_BUTTON_WINDOW_X, CANCEL_SET_Y, IMG_W, IMG_H, buttons, SET_REMOVE_CANCEL);
	AddToArrSettings(BEST_DIFFCULTY_X, BEST_DIFFCULTY_Y, BEST_DIFFCULTY_WIDTH, BEST_DIFFCULTY_HIGHT, buttons, BEST_DIFFCULTY_NUM);
	AddToArrSettings(START_X, START_Y, IMG_W, IMG_H, buttons, START_NUM);
	AddToArrSettings(GAME_BUTTON_WINDOW_X, BEST_MOVE_BUTTON_Y, IMG_W, IMG_H, buttons, BEST_MOVE_NUM);
	AddToArrSettings(GAME_BUTTON_WINDOW_X, QUIT_GAME_BUTTON_Y, IMG_W, IMG_H, buttons, QUIT_GAME_BUTTON_NUM);
	AddToArrSettings(GAME_BUTTON_WINDOW_X, QUIT_GAME_BUTTON_Y, IMG_W, IMG_H, buttons, SET_BACK_NUM);
	AddToArrSettings(GAME_BUTTON_WINDOW_X, SAVE_BUTTON_Y, IMG_W, IMG_H, buttons, CLEAR_BUTTON_NUM);
	AddToArrSettings(GAME_BUTTON_WINDOW_X, SET_BUTTON_Y, IMG_W, IMG_H, buttons, SET_PIECE_NUM);
	AddToArrSettings(PROMOTION_ROOK_X, PROMOTION_BUTTON_Y, PROMOTION_BUTTON_WIDTH, PROMOTION_BUTTON_HIGHT, buttons, PROMOTION_ROOK_NUM);
	AddToArrSettings(PROMOTION_QUEEN_X, PROMOTION_BUTTON_Y, PROMOTION_BUTTON_WIDTH, PROMOTION_BUTTON_HIGHT, buttons, PROMOTION_QUEEN_NUM);
	AddToArrSettings(PROMOTION_KNIGHT_X, PROMOTION_BUTTON_Y, PROMOTION_BUTTON_WIDTH, PROMOTION_BUTTON_HIGHT, buttons, PROMOTION_KNIGHT_NUM);
	AddToArrSettings(PROMOTION_BISHOP_X, PROMOTION_BUTTON_Y, PROMOTION_BUTTON_WIDTH, PROMOTION_BUTTON_HIGHT, buttons, PROMOTION_BISHOP_NUM);
	AddToArrSettings(CHECK_MATE_X, CHECK_MATE_Y, CHECK_MATE_WIDTH, CHECK_MATE_HIGHT, buttons, CHECK_MATE_NUM);
	AddToArrSettings(CHECK_X, CHECK_Y, CHECK_WIDTH, CHECK_HIGHT, buttons, CHECK_NUM);
	AddToArrSettings(CHECK_MATE_X, CHECK_MATE_Y, INS_W, INS_H, buttons, INS_NUM);
	AddToArrSettings(CHECK_MATE_X, CHECK_MATE_Y, TIE_W, TIE_H, buttons, TIE_NUM);

	for (i = 0; i < NUM_OF_PIECES_ROW; i++){
		AddToArrSettings(SQUARE_HIGHT_AND_WIDTH + (SQUARE_HIGHT_AND_WIDTH*i), SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH, buttons, SET_PIECES_START_NUM_ROW1 + i);

	}

	for (i = 0; i < NUM_OF_PIECES_ROW; i++){
		AddToArrSettings(SQUARE_HIGHT_AND_WIDTH + (SQUARE_HIGHT_AND_WIDTH*i), SET_WINDOW_PIECES_Y, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH, buttons, SET_PIECES_START_NUM_ROW2 + i);
	}


	for (i = 0; i<NUM_OF_SLOTS; i++){
		AddToArrSettings(SLOT_X_DIF + (SLOT_X_DIF*i), SLOT_Y, NUMBER_WIDTH, NUMBER_HIGHT, buttons, SLOT_START_NUM + i);
	}
	slotImages[0] = IMG_GREEN;
	slotImages[1] = IMG_GRAY;
	slotImages[2] = IMG_PINK;
	slotImages[3] = IMG_BLUE;
	slotImages[4] = IMG_YELLOW;
	slotImages[5] = IMG_PURPLE;
	slotImages[6] = IMG_ORANGE;

	AddToArrSettings(SAVE_LOAD_CANCEL_X, SAVE_LOAD_CANCEL_Y, IMG_W, IMG_H, buttons, SAVE_LOAD_CANCEL_NUM);
}




void AddToArrSettings(int x, int y, int w, int h, int buttons[NUM_OF_BUTTONS][NUM_OF_DATA], int buttonNum){
	buttons[buttonNum][0] = x;
	buttons[buttonNum][1] = y;
	buttons[buttonNum][2] = w;
	buttons[buttonNum][3] = h;
}

int HandleSelectionEventInPlayerVsPlayerSettings(char board[BOARD_SIZE][BOARD_SIZE]){
	int quit = 0;
	while (quit != 1){
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
				/*case (SDL_QUIT):
				quit = 1;
				break;*/
			case (SDL_MOUSEBUTTONDOWN) :
				if (pressed_button(e.button, ONE_BUTTON_NUM)){
					staticMinMaxDepth = 1;

					break;
				}
									   if (pressed_button(e.button, TWO_BUTTON_NUM)){
										   staticMinMaxDepth = 2;
										   break;
									   }
									   if (pressed_button(e.button, THREE_BUTTON_NUM)){
										   staticMinMaxDepth = 3;
										   break;
									   }
									   if (pressed_button(e.button, FOUR_BUTTON_NUM)){
										   staticMinMaxDepth = 4;
										   break;
									   }
									   if (pressed_button(e.button, BEST_DIFFCULTY_NUM)){
										   best = 1;
										   break;
									   }
									   if (pressed_button(e.button, START_NUM)){

										   makeGameWindow(board, NULL);

										   HandleSelectionEventInGameWindow(board, "white");
										   quit = 1;
										   break;
									   }
									   break;
			default:
				break;
			}
		}
		SDL_Delay(50);
	}
	return 0;
}

int HandleSelectionEventInSettings(char board[BOARD_SIZE][BOARD_SIZE]){
	int quit = 0;
	while (quit != 1){
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
				/*case (SDL_QUIT):
				quit = 1;
				break;*/
			case (SDL_MOUSEBUTTONDOWN) :
				if (pressed_button(e.button, ONE_BUTTON_NUM)){
					staticMinMaxDepth = 1;
					break;
				}
									   if (pressed_button(e.button, TWO_BUTTON_NUM)){
										   staticMinMaxDepth = 2;
										   break;
									   }
									   if (pressed_button(e.button, THREE_BUTTON_NUM)){
										   staticMinMaxDepth = 3;
										   break;
									   }
									   if (pressed_button(e.button, FOUR_BUTTON_NUM)){
										   staticMinMaxDepth = 4;
										   break;
									   }
									   if (pressed_button(e.button, BEST_DIFFCULTY_NUM)){
										   best = 1;
										   break;
									   }
									   if (pressed_button(e.button, WHITE_BUTTON_NUM)){
										   staticUserColor = 0;
										   break;
									   }
									   if (pressed_button(e.button, BLACK_BUTTON_NUM)){
										   staticUserColor = 1;
										   break;
									   }
									   if (pressed_button(e.button, START_NUM)){

										   makeGameWindow(board, NULL);
										   if (staticUserColor == 0){
											   HandleSelectionEventInGameWindow(board, "white");
										   }
										   else if (staticUserColor == 1){
											   HandleSelectionEventInGameWindow(board, "black");

										   }

										   quit = 1;
										   break;
									   }
									   break;
			default:
				break;
			}
		}
		SDL_Delay(50);
	}
	return 0;
}

int makeGameWindow(char board[BOARD_SIZE][BOARD_SIZE], MovesList* bestForMove){
	int lastWhite = 0;
	SDL_Surface *whiteSquareImg;
	SDL_Surface *blackSquareImg;
	SDL_Surface *chessImg;

	SDL_Surface *bestMovesImg;
	SDL_Surface *saveGameImg;
	SDL_Surface *mainMenuImg;
	SDL_Surface *quitGameImg;
	SDL_Surface *insButtonImg;
	SDL_Surface *currentTurnImg;
	SDL_Rect offset;
	window = SDL_SetVideoMode(GAME_WINDOW_W, GAME_WINDOW_H, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if (window == NULL) {
		printf("ERROR: failed to set video mode: %s\n", SDL_GetError());
		return 1;
	}
	SDL_WM_SetCaption("Game window", NULL);
	insButtonImg = loadImg(IMG_INS_BUTTON);
	whiteSquareImg = loadImg(IMG_WHITE_SQUARE);
	blackSquareImg = loadImg(IMG_BLACK_SQUARE);
	chessImg = loadImg(IMG_CHESS_LOGO_2);

	saveGameImg = loadImg(IMG_SAVE_GAME);
	mainMenuImg = loadImg(IMG_MAIN_MENU);
	quitGameImg = loadImg(IMG_QUIT_GAME_BUTTON);
	bestMovesImg = loadImg(IMG_BEST_MOVE);
	if (!staticCurrentTurn){
		currentTurnImg = loadImg(WHITE_TURN);
	}
	else{
		currentTurnImg = loadImg(BLACK_TURN);
	}
	SDL_FillRect(window, NULL, 0xFFFFFF);
	for (offset.x = SQUARE_HIGHT_AND_WIDTH; offset.x <= SQUARE_HIGHT_AND_WIDTH * 8; offset.x += SQUARE_HIGHT_AND_WIDTH){
		for (offset.y = SQUARE_HIGHT_AND_WIDTH; offset.y <= SQUARE_HIGHT_AND_WIDTH * 8; offset.y += SQUARE_HIGHT_AND_WIDTH){
			if (lastWhite){
				addImage(blackSquareImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
			}
			else{
				addImage(whiteSquareImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
			}
			lastWhite = !lastWhite;
		}
		lastWhite = !lastWhite;
	}
	offset.y = CHESS_LOGO_2_Y;
	offset.x = CHESS_LOGO_2_X;
	addImage(chessImg, offset, CHESS_LOGO_2_WIDTH, CHESS_LOGO_2_HIGHT);
	offset.y =40;
	offset.x = 600;
	addImage(currentTurnImg , offset,TURN_BLACK_W, TURN_BLACK_H);
	

	offset.x = GAME_BUTTON_WINDOW_X;

	offset.y = SAVE_BUTTON_Y;
	addImage(saveGameImg, offset, IMG_W, IMG_H);
	offset.y = MAIN_MENU_BUTTON_Y;
	addImage(mainMenuImg, offset, IMG_W, IMG_H);
	offset.y = INS_BUTTON_Y;
	addImage(insButtonImg, offset, IMG_W, IMG_H);
	offset.y = BEST_MOVE_BUTTON_Y;
	addImage(bestMovesImg, offset, IMG_W, IMG_H);
	offset.y = QUIT_GAME_BUTTON_Y;
	addImage(quitGameImg, offset, IMG_W, IMG_H);
	addBoardToGameWindow(board, bestForMove);
	/* We finished drawing*/
	if (SDL_Flip(window) != 0) {
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		return 1;
	}
	return 0;
}

void computersMove(char board[BOARD_SIZE][BOARD_SIZE]){
	/* makes a computer move, returns 1 if the computer wins and the game ends,
	0 if the game continues */

	MovesList* computerBestMove;
	char* computerColor = "black";

	if (staticUserColor == 1){
		computerColor = "white";

	}

	computerBestMove = init_new_movesList(-1, -1);
	computerBestMove->start->next = init_new_move(-1, -1);
	computerBestMove->end = computerBestMove->start->next;

	if (best == 1) {
		staticMinMaxDepth = 4;
	}
	alphaBeta_maximize(board, -10000000, 10000000, staticMinMaxDepth, 0, computerColor, computerBestMove, best, 0, NULL);
	change_board_one_move(board, board, (computerBestMove)->start->col, (computerBestMove)->start->row, (computerBestMove)->start->next);
	free_movesList(computerBestMove);
	free(computerBestMove);

}

int makeAddRemoveSetWindow(char board[BOARD_SIZE][BOARD_SIZE]){
	int indexRow = SQUARE_HIGHT_AND_WIDTH;
	
	SDL_Surface *whiteKingSetImg;
	SDL_Surface *whiteQueenSetImg;
	SDL_Surface *whiteBishopSetImg;
	SDL_Surface *whiteKnightSetImg;
	SDL_Surface *whiteRookSetImg;
	SDL_Surface *whitePawnSetImg;

	SDL_Surface *blackKingSetImg;
	SDL_Surface *blackQueenSetImg;
	SDL_Surface *blackBishopSetImg;
	SDL_Surface *blackKnightSetImg;
	SDL_Surface *blackRookSetImg;
	SDL_Surface *blackPawnSetImg;

	SDL_Rect offset;

	window = SDL_SetVideoMode(SET_PIECES_WINDOW_W, SET_PIECES_WINDOW_H, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if (window == NULL) {
		printf("ERROR: failed to set video mode: %s\n", SDL_GetError());
		return 1;
	}
	SDL_WM_SetCaption("Choose a type", NULL);

	whiteKingSetImg = loadImg(IMG_SET_WHITE_KING);
	whiteQueenSetImg = loadImg(IMG_SET_WHITE_QUEEN);
	whiteBishopSetImg = loadImg(IMG_SET_WHITE_BISHOP);
	whiteKnightSetImg = loadImg(IMG_SET_WHITE_KNIGHT);
	whiteRookSetImg = loadImg(IMG_SET_WHITE_ROOK);
	whitePawnSetImg = loadImg(IMG_SET_WHITE_PAWN);

	blackKingSetImg = loadImg(IMG_SET_BLACK_KING);
	blackQueenSetImg = loadImg(IMG_SET_BLACK_QUEEN);
	blackBishopSetImg = loadImg(IMG_SET_BLACK_BISHOP);
	blackKnightSetImg = loadImg(IMG_SET_BLACK_KNIGHT);
	blackRookSetImg = loadImg(IMG_SET_BLACK_ROOK);
	blackPawnSetImg = loadImg(IMG_SET_BLACK_PAWN);


	SDL_FillRect(window, NULL, 0xFFFFFF);
	offset.y = SQUARE_HIGHT_AND_WIDTH;
	offset.x = indexRow;
	addImage(whiteKingSetImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
	offset.x = indexRow * 2;
	addImage(whiteQueenSetImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
	offset.x = indexRow * 3;
	addImage(whiteKnightSetImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
	offset.x = indexRow * 4;
	addImage(whiteRookSetImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
	offset.x = indexRow * 5;
	addImage(whiteBishopSetImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
	offset.x = indexRow * 6;
	addImage(whitePawnSetImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
	offset.x = indexRow * 7;

	offset.y = SET_WINDOW_PIECES_Y;
	offset.x = indexRow;
	addImage(blackKingSetImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
	offset.x = indexRow * 2;
	addImage(blackQueenSetImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
	offset.x = indexRow * 3;
	addImage(blackKnightSetImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
	offset.x = indexRow * 4;
	addImage(blackRookSetImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
	offset.x = indexRow * 5;
	addImage(blackBishopSetImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
	offset.x = indexRow * 6;
	addImage(blackPawnSetImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
	offset.x = indexRow * 7;

	/* We finished drawing*/
	if (SDL_Flip(window) != 0) {
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		return 1;
	}
	return 0;
}

MovesList* getPossibleOneMove(bestMovesForBoard* bestList, Move* moveToBest){


	struct bestMoveForPosition* currentMove = bestList->head;
	MovesList* bestMoves = init_new_movesList(-1, -1);
	
	while (currentMove->bestForPosition->start->col != -1){
		if (((currentMove->bestForPosition->start->col == moveToBest->col))
			&& (currentMove->bestForPosition->start->row == moveToBest->row)){
			bestMoves->end->col = currentMove->bestForPosition->end->col;
			bestMoves->end->row = currentMove->bestForPosition->end->row;
			bestMoves->end->next = init_new_move(-1, -1);
			bestMoves->end = bestMoves->end->next;
		}

		currentMove = currentMove->next;
	}
	return bestMoves;
}


int HandleSelectionEventInGameWindow(char board[BOARD_SIZE][BOARD_SIZE], char * playerColor){
	int quit;
	int startMove;
	char pieceType;
	char newType;
	int srcPawn;
	char* currentColor;
	char* enemyColor;

	MovesList* fromToMove;
	Move* moveToBest;
	bestMovesForBoard* bestMoves;
	MovesList* userBestMove;
	MovesList* forBestMove;
	
	moveToBest = (Move*)malloc(sizeof(Move));
	moveToBest->col = -1;
	moveToBest->row = -1;
	
	srcPawn = 0;
	quit = 0;
	startMove = 0;

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
			quit = 1;
			checkMate_window(enemyColor);
			HandleCheckMate();
			free(moveToBest);
			return 0;
		}
		else{
			quit = 1;
			tie_window();
			HandleTie();
			free(moveToBest);
			return 0;
		}
	}


	if (strncmp(playerColor, currentColor, strlen(playerColor)) != 0 && staticMode == 2){

		computersMove(board);

		if (check_if_color_can_Mate(board, currentColor) == 1){ /*Mate: computer wins*/
			checkMate_window(currentColor);
			HandleCheckMate();
			quit = 1;

		}
		else if (areTherePossibleMoves(board, playerColor) == 0){ /*if there aren't any possible moves*/
			if (check_if_color_can_Check(board, currentColor) == 1){
				quit = 1;
				checkMate_window(currentColor);
				HandleCheckMate();

			}
			else{ /*tie*/
				quit = 1;
				tie_window();
				HandleTie();
				
			}
		}
		else if (check_if_color_can_Check(board, currentColor) == 1){ /*Check: computer threatens the player's king*/
			check_window();
			HandleCheck();
		}

		makeGameWindow(board, NULL);

		staticCurrentTurn = abs(staticCurrentTurn - 1);
		if (staticCurrentTurn == 0){
			currentColor = "white";
		}
		else{
			currentColor = "black";
		}
	}
	while (quit != 1){
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
			case (SDL_MOUSEBUTTONDOWN) :

				if (pressed_button(e.button, SAVE_BUTTON_NUM)){
				
					saveOrLoad_window(SAVE);
					HandleSaveWindow(board);
					makeGameWindow(board, NULL);
					break;
				}
									   if (pressed_button(e.button, MAIN_MENU_BUTTON_NUM)){
										   init_board(board);
										   best = 0;
										   staticMinMaxDepth = 1;
										   staticMode = 1;
										   staticCurrentTurn = 0;  /*0-white, 1-black*/
										   staticUserColor = 0;
										   gui_settings(board);
										   quit = 1;

										   break;

									   }if (pressed_button(e.button, BEST_MOVE_NUM)){
										   if ((moveToBest->col != -1) && (moveToBest->row != -1)){
											   bestMoves = init_new_bestMovesForBoard(-1, -1);

											   userBestMove = init_new_movesList(-1, -1);
											   userBestMove->start->next = init_new_move(-1, -1);
											   userBestMove->end = userBestMove->start->next;


											   alphaBeta_maximize(board, -10000000, 10000000, staticMinMaxDepth, 0, currentColor, userBestMove, best, 1, bestMoves);
											   bestMoves = getMyBest();
											   forBestMove = getPossibleOneMove(bestMoves, moveToBest);
											   makeGameWindow(board, forBestMove);
											   startMove = 0;
											   free_bestMovesForBoard(bestMoves);
											   free(bestMoves);
											   free_movesList(userBestMove);
											   free(userBestMove);
											   free_movesList(forBestMove);
											   free(forBestMove);
											   break;
										   }

									   }
									   if (pressed_button(e.button, QUIT_GAME_BUTTON_NUM)){
										   quit = 1;
										   break;
									   } if (pressed_button(e.button, INS_BUTTON_NUM)){
										   InsWindow();
										   HandleIns();
										   makeGameWindow(board, NULL);

									   }

									   if (SQUARE_HIGHT_AND_WIDTH<e.button.x&&SQUARE_HIGHT_AND_WIDTH<e.button.y&&SQUARE_HIGHT_AND_WIDTH * 9>e.button.x&&SQUARE_HIGHT_AND_WIDTH * 9>e.button.y){
										   moveToBest->col = -1;
										   moveToBest->row = -1;
										   if (!startMove){
											   moveToBest->col = (e.button.x / SQUARE_HIGHT_AND_WIDTH) - 1;
											   moveToBest->row = (e.button.y / SQUARE_HIGHT_AND_WIDTH) - 1;
										   }

										   if (!startMove){
											   fromToMove = init_new_movesList_forGui(-1, -1);
											   fromToMove->start->col = (e.button.x / SQUARE_HIGHT_AND_WIDTH) - 1;
											   fromToMove->start->row = (e.button.y / SQUARE_HIGHT_AND_WIDTH) - 1;
											   moveToBest->col = (e.button.x / SQUARE_HIGHT_AND_WIDTH) - 1;
											   moveToBest->row = (e.button.y / SQUARE_HIGHT_AND_WIDTH) - 1;
											   if (board[fromToMove->start->col][fromToMove->start->row] != EMPTY){
												   startMove = !startMove;
											   }

										   }
										   else{
											   fromToMove->end->col = (e.button.x / SQUARE_HIGHT_AND_WIDTH) - 1;
											   fromToMove->end->row = (e.button.y / SQUARE_HIGHT_AND_WIDTH) - 1;

											   if (board[fromToMove->start->col][fromToMove->start->row] == WHITE_P || board[fromToMove->start->col][fromToMove->start->row] == BLACK_P){
												   srcPawn = 1;
											   }
											   else{
												   srcPawn = 0;
											   }
											   if (srcPawn&&canPromotePawn(currentColor, fromToMove->end->row)){
												   if (strcmp(currentColor, "white") == 0){
													   fromToMove->end->becomeTo = WHITE_R;

												   }
												   else if (strcmp(currentColor, "black") == 0){
													   fromToMove->end->becomeTo = BLACK_R;
												   }
											   }

											   if (create_userMove_for_gui(board, fromToMove) == 1){
												   if (is_legal_move(board, fromToMove, currentColor, 0) == 1){

													   if (srcPawn&&canPromotePawn(currentColor, fromToMove->end->row)){
														   promotion_question_window();
														   newType = HandlePromotionQuestion(currentColor);
														   fromToMove->end->becomeTo = newType;
														   board[fromToMove->start->col][fromToMove->start->row] = EMPTY;
														   board[fromToMove->end->col][fromToMove->end->row] = newType;
													   }

													   else if (!(srcPawn&&canPromotePawn(currentColor, fromToMove->end->row))){
														   pieceType = board[fromToMove->start->col][fromToMove->start->row];
														   board[fromToMove->start->col][fromToMove->start->row] = EMPTY;
														   board[fromToMove->end->col][fromToMove->end->row] = pieceType;
													   }
													   freeUsedList();
													   useSpace = (toFreeList *)calloc(1, sizeof(toFreeList));

													   if (strcmp(currentColor, "white") == 0){
														   enemyColor = "black";
													   }
													   else if (strcmp(currentColor, "black") == 0){
														   enemyColor = "white";
													   }


													   if (check_if_color_can_Mate(board, currentColor) == 1){
														   quit = 1;
														   checkMate_window(currentColor);
														   HandleCheckMate();
														   free_movesList(fromToMove);
														   free(fromToMove);
														   break;
													   }
													   else if (areTherePossibleMoves(board, enemyColor) == 0){ /*if there aren't any possible moves*/
														   if (check_if_color_can_Check(board, currentColor) == 1){
															   quit = 1;
															   checkMate_window(currentColor);
															   HandleCheckMate();
															   free_movesList(fromToMove);
															   free(fromToMove);
															   break;
														   }
														   else{
															   /*tie - need to do*/
															   quit = 1;
															   tie_window();
															   HandleTie();

															   free_movesList(fromToMove);
															   free(fromToMove);
															   break;
														   }
													   }
													   else if (check_if_color_can_Check(board, currentColor) == 1){ /*Check: computer threatens the player's king*/
														   check_window();
														   HandleCheck();
													   }

													   staticCurrentTurn = abs(staticCurrentTurn - 1);
													   if (staticCurrentTurn == 0){
														   currentColor = "white";
													   }
													   else{
														   currentColor = "black";
													   }

													   makeGameWindow(board, NULL);
													   free_movesList(fromToMove);


													   if (staticMode == 2){
											
														   computersMove(board);


														   if (strcmp(currentColor, "white") == 0){
															   enemyColor = "black";
														   }
														   else if (strcmp(currentColor, "black") == 0){
															   enemyColor = "white";
														   }

														   if (check_if_color_can_Mate(board, currentColor) == 1){
															   checkMate_window(currentColor);
															   HandleCheckMate();
															   quit = 1;
															   break;
														   }
														   else if (areTherePossibleMoves(board, enemyColor) == 0){ /*if there aren't any possible moves*/
															   if (check_if_color_can_Check(board, currentColor) == 1){
																   checkMate_window(currentColor);
																   HandleCheckMate();
																   quit = 1;
																   break;
															   }
															   else{

																   tie_window();
																   HandleTie();
																   quit = 1;
																   break;
															   }
														   }
														   else if (check_if_color_can_Check(board, currentColor) == 1){ /*Check: computer threatens the player's king*/
															   check_window();
															   HandleCheck();
														   }


														   staticCurrentTurn = abs(staticCurrentTurn - 1);
														   if (staticCurrentTurn == 0){
															   currentColor = "white";
														   }
														   else{
															   currentColor = "black";
														   }
														   makeGameWindow(board, NULL);

														   /* free_movesList(fromToMove);
														   free(fromToMove);*/

													   }
												   }
											   }

											   free(fromToMove);
											   startMove = !startMove;

										   }
									   }
									   break;
			default:
				break;
			}
		}
		SDL_Delay(50);
	}
	free(moveToBest);
	return 0;
}

int HandleSelectionEventInaddRemoveWindow(char board[BOARD_SIZE][BOARD_SIZE]){
	int i, j;
	int quit;


	int numWhiteKing;
	int  numBlackKing;
	Move* moveToRemove;
	Move * moveToAdd;
	quit = 0;
	numWhiteKing = 0;
	numBlackKing = 0;


	moveToRemove = (Move*)malloc(sizeof(Move));
	moveToAdd = (Move*)malloc(sizeof(Move));
	moveToRemove->col = -1;
	moveToRemove->row = -1;
	moveToAdd->col = -1;
	moveToAdd->row = -1;


	while (quit != 1){
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
			case (SDL_MOUSEBUTTONDOWN) :
				if (pressed_button(e.button, CLEAR_BUTTON_NUM)){
					for (i = 0; i < BOARD_SIZE; i++) {
						for (j = 0; j < BOARD_SIZE; j++) {
							remove_disc(board, j, i);
						}
					}
					SettingWindowAddRemove(board, 1,1);
				}
									   if (pressed_button(e.button, SET_REMOVE_CANCEL)){
										   init_board(board);
										   SettingWindowAddRemove(board, 1,1);
									   }

									   if (pressed_button(e.button, REMOVE_NUM)){
										   if ((moveToRemove->col != -1) && moveToRemove->row != -1){
											   board[moveToRemove->col][moveToRemove->row] = EMPTY;
											   SettingWindowAddRemove(board, 1,1);
										   }
										   else{
											   SettingWindowAddRemove(board, 1,0);
										   }
										  

									   }
									   if (pressed_button(e.button, SET_PIECE_NUM)){
										   makeAddRemoveSetWindow(board);
										   if ((moveToAdd->col != -1) && (moveToAdd->row != -1)){
											   HandleSelectionEventInaddRemoveForSetWindow(board, moveToAdd->col, moveToAdd->row);
											   SettingWindowAddRemove(board, 1,1);
										   }
										   else{
											   SettingWindowAddRemove(board, 1,0);
										   }
										   
									   }
									   if (pressed_button(e.button, SET_BACK_NUM)){
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

											   numWhiteKing = 0;
											   numBlackKing = 0;
											   SettingWindowAddRemove(board, 0,1);
										   }
										   else{
											   players_selection_window();
											   HandleSelectionEventInPlayerSelection(board);
											   quit = 1;
											   free(moveToRemove);
											   free(moveToAdd);
											   break;
										   }

									   }

									   if (SQUARE_HIGHT_AND_WIDTH<e.button.x&&SQUARE_HIGHT_AND_WIDTH<e.button.y&&SQUARE_HIGHT_AND_WIDTH * 9>e.button.x&&SQUARE_HIGHT_AND_WIDTH * 9>e.button.y){
										   moveToRemove->col = -1;
										   moveToRemove->row = -1;
										   moveToAdd->col = -1;
										   moveToAdd->row = -1;


										   if (board[(e.button.x / SQUARE_HIGHT_AND_WIDTH) - 1][(e.button.y / SQUARE_HIGHT_AND_WIDTH) - 1] != EMPTY){
											   moveToRemove->col = (e.button.x / SQUARE_HIGHT_AND_WIDTH) - 1;
											   moveToRemove->row = (e.button.y / SQUARE_HIGHT_AND_WIDTH) - 1;
										   }
										   else{

											   moveToAdd->col = (e.button.x / SQUARE_HIGHT_AND_WIDTH) - 1;
											   moveToAdd->row = (e.button.y / SQUARE_HIGHT_AND_WIDTH) - 1;
										   }



									   }
									   break;
			default:
				break;
			}
		}
		SDL_Delay(50);
	}
	return 0;
}





int HandleSelectionEventInaddRemoveForSetWindow(char board[BOARD_SIZE][BOARD_SIZE], int j, int i){
	int quit = 0;




	while (quit != 1){
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
			case (SDL_MOUSEBUTTONDOWN) :


				if (pressed_button(e.button, SET_PIECES_START_NUM_ROW1)){
					set(board, j, i, "white", "king",0);
					return 1;

				}
				else if (pressed_button(e.button, SET_PIECES_START_NUM_ROW1 + 1)){
					set(board, j, i, "white", "queen",0);
					return 1;
				}
				else if (pressed_button(e.button, SET_PIECES_START_NUM_ROW1 + 2)){
					set(board, j, i, "white", "knight",0);
					return 1;
				}
				else if (pressed_button(e.button, SET_PIECES_START_NUM_ROW1 + 3)){
					set(board, j, i, "white", "rook",0);
					return 1;
				}
				else if (pressed_button(e.button, SET_PIECES_START_NUM_ROW1 + 4)){
					set(board, j, i, "white", "bishop",0);
					return 1;
				}
				else if (pressed_button(e.button, SET_PIECES_START_NUM_ROW1 + 5)){
					set(board, j, i, "white", "pawn",0);
					return 1;
				}


				else if (pressed_button(e.button, SET_PIECES_START_NUM_ROW2)){
					set(board, j, i, "black", "king",0);
					return 1;

				}
				else if (pressed_button(e.button, SET_PIECES_START_NUM_ROW2 + 1)){
					set(board, j, i, "black", "queen",0);
					return 1;
				}
				else if (pressed_button(e.button, SET_PIECES_START_NUM_ROW2 + 2)){
					set(board, j, i, "black", "knight",0);
					return 1;
				}
				else if (pressed_button(e.button, SET_PIECES_START_NUM_ROW2 + 3)){
					set(board, j, i, "black", "rook",0);
					return 1;
				}
				else if (pressed_button(e.button, SET_PIECES_START_NUM_ROW2 + 4)){
					set(board, j, i, "black", "bishop",0);
					return 1;
				}
				else if (pressed_button(e.button, SET_PIECES_START_NUM_ROW2 + 5)){
					set(board, j, i, "black", "pawn",0);
					return 1;
				}

				break;
			default:
				break;
			}
		}
		SDL_Delay(50);
	}
	return 0;
}

int addBoardToGameWindow(char board[BOARD_SIZE][BOARD_SIZE], MovesList * moveList){
	int x = 0;
	int y = 0;
	SDL_Surface *whitePawnWhiteSqrImg;
	SDL_Surface *whitePawnBlackSqrImg;
	SDL_Surface *whiteRookWhiteSqrImg;
	SDL_Surface *whiteRookBlackSqrImg;
	SDL_Surface *whiteBishopWhiteSqrImg;
	SDL_Surface *whiteBishopBlackSqrImg;
	SDL_Surface *whiteKnightWhiteSqrImg;
	SDL_Surface *whiteKnightBlackSqrImg;
	SDL_Surface *whiteQueenWhiteSqrImg;
	SDL_Surface *whiteQueenBlackSqrImg;
	SDL_Surface *whiteKingWhiteSqrImg;
	SDL_Surface *whiteKingBlackSqrImg;

	SDL_Surface *blackPawnWhiteSqrImg;
	SDL_Surface *blackPawnBlackSqrImg;
	SDL_Surface *blackRookWhiteSqrImg;
	SDL_Surface *blackRookBlackSqrImg;
	SDL_Surface *blackBishopWhiteSqrImg;
	SDL_Surface *blackBishopBlackSqrImg;
	SDL_Surface *blackKnightWhiteSqrImg;
	SDL_Surface *blackKnightBlackSqrImg;
	SDL_Surface *blackQueenWhiteSqrImg;
	SDL_Surface *blackQueenBlackSqrImg;
	SDL_Surface *blackKingWhiteSqrImg;
	SDL_Surface *blackKingBlackSqrImg;
	SDL_Surface *blueSqureImg;
	SDL_Surface* notBestImg;


	SDL_Surface *whitePawnWhiteSqrBlueImg;
	SDL_Surface *whitePawnBlackSqrBlueImg;
	SDL_Surface *whiteRookWhiteSqrBlueImg;
	SDL_Surface *whiteRookBlackSqrBlueImg;
	SDL_Surface *whiteBishopWhiteSqrBlueImg;
	SDL_Surface *whiteBishopBlackSqrBlueImg;
	SDL_Surface *whiteKnightWhiteSqrBlueImg;
	SDL_Surface *whiteKnightBlackSqrBlueImg;
	SDL_Surface *whiteQueenWhiteSqrBlueImg;
	SDL_Surface *whiteQueenBlackSqrBlueImg;
	SDL_Surface *whiteKingWhiteSqrBlueImg;
	SDL_Surface *whiteKingBlackSqrBlueImg;

	SDL_Surface *blackPawnWhiteSqrBlueImg;
	SDL_Surface *blackPawnBlackSqrBlueImg;
	SDL_Surface *blackRookWhiteSqrBlueImg;
	SDL_Surface *blackRookBlackSqrBlueImg;
	SDL_Surface *blackBishopWhiteSqrBlueImg;
	SDL_Surface *blackBishopBlackSqrBlueImg;
	SDL_Surface *blackKnightWhiteSqrBlueImg;
	SDL_Surface *blackKnightBlackSqrBlueImg;
	SDL_Surface *blackQueenWhiteSqrBlueImg;
	SDL_Surface *blackQueenBlackSqrBlueImg;
	SDL_Surface *blackKingWhiteSqrBlueImg;
	SDL_Surface *blackKingBlackSqrBlueImg;
	SDL_Rect offset;

	whitePawnWhiteSqrImg = loadImg(IMG_WHITE_PAWN_WHITE_SQURE);
	whitePawnBlackSqrImg = loadImg(IMG_WHITE_PAWN_BLACK_SQURE);

	whiteRookWhiteSqrImg = loadImg(IMG_WHITE_ROOK_WHITE_SQURE);
	whiteRookBlackSqrImg = loadImg(IMG_WHITE_ROOK_BLACK_SQURE);

	whiteBishopWhiteSqrImg = loadImg(IMG_WHITE_BISHOP_WHITE_SQURE);
	whiteBishopBlackSqrImg = loadImg(IMG_WHITE_BISHOP_BLACK_SQURE);

	whiteKnightWhiteSqrImg = loadImg(IMG_WHITE_KNIGHT_WHITE_SQURE);
	whiteKnightBlackSqrImg = loadImg(IMG_WHITE_KNIGHT_BLACK_SQURE);

	whiteQueenWhiteSqrImg = loadImg(IMG_WHITE_QUEEN_WHITE_SQURE);
	whiteQueenBlackSqrImg = loadImg(IMG_WHITE_QUEEN_BLACK_SQURE);

	whiteKingWhiteSqrImg = loadImg(IMG_WHITE_KING_WHITE_SQURE);
	whiteKingBlackSqrImg = loadImg(IMG_WHITE_KING_BLACK_SQURE);

	blackPawnWhiteSqrImg = loadImg(IMG_BLACK_PAWN_WHITE_SQURE);
	blackPawnBlackSqrImg = loadImg(IMG_BLACK_PAWN_BLACK_SQURE);

	blackRookWhiteSqrImg = loadImg(IMG_BLACK_ROOK_WHITE_SQURE);
	blackRookBlackSqrImg = loadImg(IMG_BLACK_ROOK_BLACK_SQURE);

	blackBishopWhiteSqrImg = loadImg(IMG_BLACK_BISHOP_WHITE_SQURE);
	blackBishopBlackSqrImg = loadImg(IMG_BLACK_BISHOP_BLACK_SQURE);

	blackKnightWhiteSqrImg = loadImg(IMG_BLACK_KNIGHT_WHITE_SQURE);
	blackKnightBlackSqrImg = loadImg(IMG_BLACK_KNIGHT_BLACK_SQURE);

	blackQueenWhiteSqrImg = loadImg(IMG_BLACK_QUEEN_WHITE_SQURE);
	blackQueenBlackSqrImg = loadImg(IMG_BLACK_QUEEN_BLACK_SQURE);

	blackKingWhiteSqrImg = loadImg(IMG_BLACK_KING_WHITE_SQURE);
	blackKingBlackSqrImg = loadImg(IMG_BLACK_KING_BLACK_SQURE);

	blueSqureImg = loadImg(IMG_BEST_SQURE);

	whitePawnWhiteSqrBlueImg = loadImg(IMG_WHITE_PAWN_WHITE_SQURE_BLUE);
	whitePawnBlackSqrBlueImg = loadImg(IMG_WHITE_PAWN_BLACK_SQURE_BLUE);

	whiteRookWhiteSqrBlueImg = loadImg(IMG_WHITE_ROOK_WHITE_SQURE_BLUE);
	whiteRookBlackSqrBlueImg = loadImg(IMG_WHITE_ROOK_BLACK_SQURE_BLUE);

	whiteBishopWhiteSqrBlueImg = loadImg(IMG_WHITE_BISHOP_WHITE_SQURE_BLUE);
	whiteBishopBlackSqrBlueImg = loadImg(IMG_WHITE_BISHOP_BLACK_SQURE_BLUE);

	whiteKnightWhiteSqrBlueImg = loadImg(IMG_WHITE_KNIGHT_WHITE_SQURE_BLUE);
	whiteKnightBlackSqrBlueImg = loadImg(IMG_WHITE_KNIGHT_BLACK_SQURE_BLUE);

	whiteQueenWhiteSqrBlueImg = loadImg(IMG_WHITE_QUEEN_WHITE_SQURE_BLUE);
	whiteQueenBlackSqrBlueImg = loadImg(IMG_WHITE_QUEEN_BLACK_SQURE_BLUE);

	whiteKingWhiteSqrBlueImg = loadImg(IMG_WHITE_KING_WHITE_SQURE_BLUE);
	whiteKingBlackSqrBlueImg = loadImg(IMG_WHITE_KING_BLACK_SQURE_BLUE);

	blackPawnWhiteSqrBlueImg = loadImg(IMG_BLACK_PAWN_WHITE_SQURE_BLUE);
	blackPawnBlackSqrBlueImg = loadImg(IMG_BLACK_PAWN_BLACK_SQURE_BLUE);

	blackRookWhiteSqrBlueImg = loadImg(IMG_BLACK_ROOK_WHITE_SQURE_BLUE);
	blackRookBlackSqrBlueImg = loadImg(IMG_BLACK_ROOK_BLACK_SQURE_BLUE);

	blackBishopWhiteSqrBlueImg = loadImg(IMG_BLACK_BISHOP_WHITE_SQURE_BLUE);
	blackBishopBlackSqrBlueImg = loadImg(IMG_BLACK_BISHOP_BLACK_SQURE_BLUE);

	blackKnightWhiteSqrBlueImg = loadImg(IMG_BLACK_KNIGHT_WHITE_SQURE_BLUE);
	blackKnightBlackSqrBlueImg = loadImg(IMG_BLACK_KNIGHT_BLACK_SQURE_BLUE);

	blackQueenWhiteSqrBlueImg = loadImg(IMG_BLACK_QUEEN_WHITE_SQURE_BLUE);
	blackQueenBlackSqrBlueImg = loadImg(IMG_BLACK_QUEEN_BLACK_SQURE_BLUE);

	blackKingWhiteSqrBlueImg = loadImg(IMG_BLACK_KING_WHITE_SQURE_BLUE);
	blackKingBlackSqrBlueImg = loadImg(IMG_BLACK_KING_BLACK_SQURE_BLUE);

	notBestImg = loadImg(NOT_BEST_MOVE);

	for (offset.x = SQUARE_HIGHT_AND_WIDTH; x<8; offset.x += SQUARE_HIGHT_AND_WIDTH, x += 1){
		for (y = 0, offset.y = SQUARE_HIGHT_AND_WIDTH; y<8; offset.y += SQUARE_HIGHT_AND_WIDTH, y += 1){
			if (board[x][y] == WHITE_P){
				if (colorBoard[x][y] == 1){
					addImage(whitePawnBlackSqrImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
				}
				else{
					addImage(whitePawnWhiteSqrImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
				}
			}
			else if (board[x][y] == WHITE_R){
				if (colorBoard[x][y] == 1){
					addImage(whiteRookBlackSqrImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
				}
				else{
					addImage(whiteRookWhiteSqrImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
				}
			}
			else if (board[x][y] == WHITE_Q){
				if (colorBoard[x][y] == 1){
					addImage(whiteQueenBlackSqrImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
				}
				else{
					addImage(whiteQueenWhiteSqrImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
				}
			}
			else if (board[x][y] == WHITE_B){
				if (colorBoard[x][y] == 1){
					addImage(whiteBishopBlackSqrImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
				}
				else{
					addImage(whiteBishopWhiteSqrImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
				}
			}
			else if (board[x][y] == WHITE_N){
				if (colorBoard[x][y] == 1){
					addImage(whiteKnightBlackSqrImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
				}
				else{
					addImage(whiteKnightWhiteSqrImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
				}
			}
			else if (board[x][y] == WHITE_K){
				if (colorBoard[x][y] == 1){
					addImage(whiteKingBlackSqrImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);

				}
				else{
					addImage(whiteKingWhiteSqrImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
				}


			}
			else if (board[x][y] == BLACK_P){
				if (colorBoard[x][y] == 1){
					addImage(blackPawnBlackSqrImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
				}
				else{
					addImage(blackPawnWhiteSqrImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
				}
			}
			else if (board[x][y] == BLACK_R){
				if (colorBoard[x][y] == 1){
					addImage(blackRookBlackSqrImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
				}
				else{
					addImage(blackRookWhiteSqrImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
				}
			}
			else if (board[x][y] == BLACK_Q){
				if (colorBoard[x][y] == 1){
					addImage(blackQueenBlackSqrImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
				}
				else{
					addImage(blackQueenWhiteSqrImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
				}
			}
			else if (board[x][y] == BLACK_B){
				if (colorBoard[x][y] == 1){
					addImage(blackBishopBlackSqrImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
				}
				else{
					addImage(blackBishopWhiteSqrImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
				}
			}
			else if (board[x][y] == BLACK_N){
				if (colorBoard[x][y] == 1){
					addImage(blackKnightBlackSqrImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
				}
				else{
					addImage(blackKnightWhiteSqrImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
				}
			}
			else if (board[x][y] == BLACK_K){
				if (colorBoard[x][y] == 1){
					addImage(blackKingBlackSqrImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
				}
				else{
					addImage(blackKingWhiteSqrImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
				}
			}
		}
	}
	if (moveList != NULL){
		Move* move = moveList->start;
		if (move->col == -1 && move->row == -1){
			offset.y = 8;
			offset.x = 450;
			addImage(notBestImg, offset, NOT_BEST_MOVE_W, NOT_BEST_MOVE_H);
		}
		x = 0;
		while (move != moveList->end){
			for (offset.x = SQUARE_HIGHT_AND_WIDTH; x < 8; offset.x += SQUARE_HIGHT_AND_WIDTH, x += 1){
				for (y = 0, offset.y = SQUARE_HIGHT_AND_WIDTH; y < 8; offset.y += SQUARE_HIGHT_AND_WIDTH, y += 1){
					if (y == move->row && x == move->col){
						if (board[x][y] == EMPTY){
							addImage(blueSqureImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
						}
						else if (board[x][y] == WHITE_P){
							if (colorBoard[x][y] == 1){
								addImage(whitePawnBlackSqrBlueImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
							}
							else{
								addImage(whitePawnWhiteSqrBlueImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
							}
						}
						else if (board[x][y] == WHITE_R){
							if (colorBoard[x][y] == 1){
								addImage(whiteRookBlackSqrBlueImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
							}
							else{
								addImage(whiteRookWhiteSqrBlueImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
							}
						}
						else if (board[x][y] == WHITE_Q){
							if (colorBoard[x][y] == 1){
								addImage(whiteQueenBlackSqrBlueImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
							}
							else{
								addImage(whiteQueenWhiteSqrBlueImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
							}
						}
						else if (board[x][y] == WHITE_B){
							if (colorBoard[x][y] == 1){
								addImage(whiteBishopBlackSqrBlueImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
							}
							else{
								addImage(whiteBishopWhiteSqrBlueImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
							}
						}
						else if (board[x][y] == WHITE_N){
							if (colorBoard[x][y] == 1){
								addImage(whiteKnightBlackSqrBlueImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
							}
							else{
								addImage(whiteKnightWhiteSqrBlueImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
							}
						}
						else if (board[x][y] == WHITE_K){
							if (colorBoard[x][y] == 1){
								addImage(whiteKingBlackSqrBlueImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);

							}
							else{
								addImage(whiteKingWhiteSqrBlueImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
							}


						}
						else if (board[x][y] == BLACK_P){
							if (colorBoard[x][y] == 1){
								addImage(blackPawnBlackSqrBlueImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
							}
							else{
								addImage(blackPawnWhiteSqrBlueImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
							}
						}
						else if (board[x][y] == BLACK_R){
							if (colorBoard[x][y] == 1){
								addImage(blackRookBlackSqrBlueImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
							}
							else{
								addImage(blackRookWhiteSqrBlueImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
							}
						}
						else if (board[x][y] == BLACK_Q){
							if (colorBoard[x][y] == 1){
								addImage(blackQueenBlackSqrBlueImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
							}
							else{
								addImage(blackQueenWhiteSqrBlueImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
							}
						}
						else if (board[x][y] == BLACK_B){
							if (colorBoard[x][y] == 1){
								addImage(blackBishopBlackSqrBlueImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
							}
							else{
								addImage(blackBishopWhiteSqrBlueImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
							}
						}
						else if (board[x][y] == BLACK_N){
							if (colorBoard[x][y] == 1){
								addImage(blackKnightBlackSqrBlueImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
							}
							else{
								addImage(blackKnightWhiteSqrBlueImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
							}
						}
						else if (board[x][y] == BLACK_K){
							if (colorBoard[x][y] == 1){
								addImage(blackKingBlackSqrBlueImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
							}
							else{
								addImage(blackKingWhiteSqrBlueImg, offset, SQUARE_HIGHT_AND_WIDTH, SQUARE_HIGHT_AND_WIDTH);
							}
						}







					}
				}
			}
			x = 0;
			move = move->next;
		}
	}

	return 1;
}

int promotion_question_window(){
	SDL_Surface *promotionRookImg;
	SDL_Surface *promotionQueenImg;
	SDL_Surface *promotionBishopImg;
	SDL_Surface *promotionKnightImg;
	SDL_Surface *pawnPromotionOprionsImg;
	SDL_Rect offset;
	window = SDL_SetVideoMode(PROMOTION_WINDOW_W, PROMOTION_WINDOW_H, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if (window == NULL) {
		printf("ERROR: failed to set video mode: %s\n", SDL_GetError());
		return 1;
	}
	SDL_WM_SetCaption("Promotion window", NULL);
	promotionRookImg = loadImg(IMG_PROMOTION_ROOK);
	promotionQueenImg = loadImg(IMG_PROMOTION_QUEEN);
	promotionBishopImg = loadImg(IMG_PROMOTION_BISHOP);
	promotionKnightImg = loadImg(IMG_PROMOTION_KNIGHT);
	pawnPromotionOprionsImg = loadImg(IMG_PAWN_PROMOTION_OPTIONS);
	SDL_FillRect(window, NULL, 0xFFFFFF);
	offset.y = PROMOTION_BUTTON_Y;
	offset.x = PROMOTION_ROOK_X;
	addImage(promotionRookImg, offset, PROMOTION_BUTTON_WIDTH, PROMOTION_BUTTON_HIGHT);
	offset.x = PROMOTION_QUEEN_X;
	addImage(promotionQueenImg, offset, PROMOTION_BUTTON_WIDTH, PROMOTION_BUTTON_HIGHT);
	offset.x = PROMOTION_KNIGHT_X;
	addImage(promotionKnightImg, offset, PROMOTION_BUTTON_WIDTH, PROMOTION_BUTTON_HIGHT);
	offset.x = PROMOTION_BISHOP_X;
	addImage(promotionBishopImg, offset, PROMOTION_BUTTON_WIDTH, PROMOTION_BUTTON_HIGHT);
	offset.x = PAWN_PROMOTION_OPTIONS_X;
	offset.y = PAWN_PROMOTION_OPTIONS_Y;
	addImage(pawnPromotionOprionsImg, offset, IMG_W, IMG_H);
	if (SDL_Flip(window) != 0) {
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		return 1;
	}
	return 0;
}

char HandlePromotionQuestion(char* playerColor){
	int quit = 0;
	while (quit != 1){
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
				/*case (SDL_QUIT):
				quit = 1;
				break;*/
			case (SDL_MOUSEBUTTONDOWN) :
				if (pressed_button(e.button, PROMOTION_ROOK_NUM)){
					if (strcmp(playerColor, "white") == 0){
						return WHITE_R;
					}
					else{
						return BLACK_R;
					}
					quit = 1;
					break;
				}
									   if (pressed_button(e.button, PROMOTION_QUEEN_NUM)){
										   if (strcmp(playerColor, "white") == 0){
											   return WHITE_Q;
										   }
										   else{
											   return BLACK_Q;
										   }
										   quit = 1;
										   break;
									   }
									   if (pressed_button(e.button, PROMOTION_BISHOP_NUM)){
										   if (strcmp(playerColor, "white") == 0){
											   return WHITE_B;
										   }
										   else{
											   return BLACK_B;
										   }
										   quit = 1;
										   break;
									   }
									   if (pressed_button(e.button, PROMOTION_KNIGHT_NUM)){
										   if (strcmp(playerColor, "white") == 0){
											   return WHITE_N;
										   }
										   else{
											   return BLACK_N;
										   }
										   quit = 1;
										   break;
									   }

									   break;
			default:
				break;
			}
		}
		SDL_Delay(50);
	}
	return 0;
}

int check_window(){
	SDL_Surface *checkImg;
	SDL_Rect offset;
	window = SDL_SetVideoMode(CHECK_WIDTH, CHECK_HIGHT, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if (window == NULL) {
		printf("ERROR: failed to set video mode: %s\n", SDL_GetError());
		return 1;
	}
	SDL_WM_SetCaption("Check window", NULL);
	checkImg = loadImg(IMG_CHECK);
	SDL_FillRect(window, NULL, 0xFFFFFF);
	offset.x = CHECK_X;
	offset.y = CHECK_Y;
	addImage(checkImg, offset, CHECK_WIDTH, CHECK_HIGHT);
	if (SDL_Flip(window) != 0) {
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		return 1;
	}
	return 0;
}
int HandleCheck(){
	int quit = 0;
	while (quit != 1){
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
				/*case (SDL_QUIT):
				quit = 1;
				break;*/
			case (SDL_MOUSEBUTTONDOWN) :
				if (pressed_button(e.button, CHECK_NUM)){
					quit = 1;
				}
									   break;
			}
		}
	}
	return 0;
}
int checkMate_window(char* playerColor){
	SDL_Surface *checkMateImg;
	SDL_Rect offset;
	window = SDL_SetVideoMode(CHECK_MATE_WIDTH, CHECK_MATE_HIGHT, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if (window == NULL) {
		printf("ERROR: failed to set video mode: %s\n", SDL_GetError());
		return 1;
	}
	SDL_WM_SetCaption("Check Mate window", NULL);
	if (strcmp(playerColor, "white") == 0){
		checkMateImg = loadImg(IMG_WHITE_WINS);
	}
	else{
		checkMateImg = loadImg(IMG_BLACK_WINS);
	}
	SDL_FillRect(window, NULL, 0xFFFFFF);
	offset.x = CHECK_MATE_X;
	offset.y = CHECK_MATE_Y;
	addImage(checkMateImg, offset, CHECK_MATE_WIDTH, CHECK_MATE_HIGHT);
	if (SDL_Flip(window) != 0) {
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		return 1;
	}
	return 0;
}

int tie_window(){
	SDL_Surface *tieImg;
	SDL_Rect offset;
	window = SDL_SetVideoMode(CHECK_MATE_WIDTH, CHECK_MATE_HIGHT, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if (window == NULL) {
		printf("ERROR: failed to set video mode: %s\n", SDL_GetError());
		return 1;
	}
	SDL_WM_SetCaption("Check Mate window", NULL);

	tieImg = loadImg(TIE_IMG);



	SDL_FillRect(window, NULL, 0xFFFFFF);
	offset.x = CHECK_MATE_X;
	offset.y = CHECK_MATE_Y;
	addImage(tieImg, offset, TIE_W, TIE_H);
	if (SDL_Flip(window) != 0) {
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		return 1;
	}
	return 0;
}



int InsWindow(){
	SDL_Surface *insImg;
	SDL_Rect offset;
	window = SDL_SetVideoMode(CHECK_MATE_WIDTH - 30, CHECK_MATE_HIGHT - 50, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if (window == NULL) {
		printf("ERROR: failed to set video mode: %s\n", SDL_GetError());
		return 1;
	}
	SDL_WM_SetCaption("Instructions window", NULL);
	insImg = loadImg(IMG_INS);
	SDL_FillRect(window, NULL, 0xFFFFFF);
	offset.x = CHECK_MATE_X;
	offset.y = CHECK_MATE_Y;
	addImage(insImg, offset, INS_W, INS_H);
	if (SDL_Flip(window) != 0) {
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		return 1;
	}
	return 0;
}


/*one for load and zero for save*/
int saveOrLoad_window(int option){
	SDL_Surface *slot[NUM_OF_SLOTS];
	SDL_Surface *slotChoosing;
	SDL_Surface *cancel;
	SDL_Rect offset;
	int i = 0;
	window = SDL_SetVideoMode(SAVE_AND_LOAD_WINDOW_WIDTH, SAVE_AND_LOAD_WINDOW_HIGHT, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if (window == NULL) {
		printf("ERROR: failed to set video mode: %s\n", SDL_GetError());
		return 1;
	}
	if (option == LOAD){
		SDL_WM_SetCaption("Load window", NULL);
	}
	else{
		SDL_WM_SetCaption("Save window", NULL);
	}

	SDL_FillRect(window, NULL, 0xFFFFFF);
	offset.y = CHOOSE_SLOT_Y;
	offset.x = CHOOSE_SLOT_X;
	slotChoosing = loadImg(IMG_CHOOSE_SLOT);
	addImage(slotChoosing, offset, IMG_W, IMG_H);
	offset.y = SLOT_Y;
	offset.x = SLOT_X;
	for (i = 0; i<NUM_OF_SLOTS; i++){
		slot[i] = loadImg(slotImages[i%NUM_OF_COLORS]);
		offset.x += SLOT_X_DIF;
		addImage(slot[i], offset, NUMBER_WIDTH, NUMBER_HIGHT);
	}

	offset.y = SAVE_LOAD_CANCEL_Y;
	offset.x = SAVE_LOAD_CANCEL_X;
	cancel = loadImg(IMG_CANCEL);
	addImage(cancel, offset, IMG_W, IMG_H);

	if (SDL_Flip(window) != 0) {
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		return 1;
	}
	return 0;
}

int HandleIns(){
	int quit = 0;
	while (quit != 1){
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
			case (SDL_MOUSEBUTTONDOWN) :
				if (pressed_button(e.button, INS_NUM)){
					quit = 1;

				}
									   break;
			}
		}

	}
	return 0;
}




int HandleCheckMate(){
	int quit = 0;
	while (quit != 1){
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
			case (SDL_MOUSEBUTTONDOWN) :
				if (pressed_button(e.button, CHECK_MATE_NUM)){
					quit = 1;
				}
									   break;
			}
		}
	}
	return 0;
}





int HandleTie(){
	int quit = 0;
	while (quit != 1){
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
			case (SDL_MOUSEBUTTONDOWN) :
				if (pressed_button(e.button, TIE_NUM)){
					quit = 1;
				}
									   break;
			}
		}
	}
	return 0;
}
/*this function down changed check*/
int HandleSaveWindow(char board[BOARD_SIZE][BOARD_SIZE]){
	int quit = 0;
	int i = 0;
	while (quit != 1){
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
			case (SDL_MOUSEBUTTONDOWN) :
				for (i = 0; i < NUM_OF_SLOTS; i++){
					if (pressed_button(e.button, SLOT_START_NUM + i)){
						char* fileName = getSlotFileName(i);
						save_game(board, staticMode, staticMinMaxDepth, best, staticCurrentTurn, fileName);
						quit = 1;
						break;
					}
				}
									   if (pressed_button(e.button, SAVE_LOAD_CANCEL_NUM)){
										   quit = 1;
										   break;
									   }
									   break;
			}
		}
	}
	return 0;
}


int loadGameBySlots(int slot, char board[BOARD_SIZE][BOARD_SIZE]){
	char *fileSlotName = getSlotFileName(slot);

	return load_game(board, &staticMode, &staticMinMaxDepth, &best, &staticUserColor, &staticCurrentTurn, fileSlotName);
}


int HandleLoadWindow(char board[BOARD_SIZE][BOARD_SIZE]){
	int quit = 0;
	int i = 0;
	char fileName[51];
	SDL_Surface *slotIsEmpty;
	SDL_Rect offset;
	int isEmptySlotLblEnabled = 0;
	while (quit != 1){
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
			case (SDL_MOUSEBUTTONDOWN) :
				for (i = 0; i < NUM_OF_SLOTS; i++){
					if (pressed_button(e.button, SLOT_START_NUM + i)){
						sprintf(fileName, "game%d.xml", i);
						if (isFileExists(fileName)){

							loadGameBySlots(i, board);
							return SHOULD_START_GAME;
						}
						else if (!isEmptySlotLblEnabled){
							isEmptySlotLblEnabled = 1;

							offset.y = SLOT_IS_EMPTY_Y;
							offset.x = SLOT_IS_EMPTY_X;
							slotIsEmpty = loadImg(IMG_SLOT_IS_EMPTY);
							addImage(slotIsEmpty, offset, IMG_W, IMG_H);

							if (SDL_Flip(window) != 0) {
								printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
								return 1;
							}
						}

					}
				}
									   if (pressed_button(e.button, SAVE_LOAD_CANCEL_NUM)){
										   gui_settings(board);
										   quit = 1;
										   break;
									   }
									   break;
			}
		}
	}
	return 0;

}


/**
* free all data yet to be freed data not including the list that contains them.
*/
void free_freeSurfaceNode(free_surface_node * curToFreeNode){
	if (curToFreeNode != NULL){
		free_freeSurfaceNode(curToFreeNode->next);
		SDL_FreeSurface(curToFreeNode->surface);
		free(curToFreeNode);
		curToFreeNode = NULL;
	}
}

/*
* free all yet to be free data including the list that contains them.
*/
void free_used_surface_list(){
	if (useSurfaceSpace){
		free_freeSurfaceNode(useSurfaceSpace->head);
		free(useSurfaceSpace);
		useSurfaceSpace = NULL;
	}
}

/*
* add data to yet to be freed list
*/
void add_to_user_surface_list(SDL_Surface * allocated){
	free_surface_node* oldHead;
	free_surface_node * newHead = (free_surface_node *)calloc(1, sizeof(free_surface_node));
	if (newHead == NULL){

		free_used_surface_list();
		exit(0);
	}
	newHead->surface = allocated;
	if (useSurfaceSpace->head == NULL){
		useSurfaceSpace->head = newHead;
	}
	else{
		oldHead = useSurfaceSpace->head;
		useSurfaceSpace->head = newHead;
		useSurfaceSpace->head->next = oldHead;
	}
}

/*
*check if allocation and to to clean list succeed if not clean all and exit program.
*/
void validate_surface_allocate(SDL_Surface * pointer, char functionName[50]){
	if (pointer == NULL){
		free_used_surface_list();
		perror("ValidateAllocationForSurface");
		exit(0);
	}
	add_to_user_surface_list(pointer);
}
