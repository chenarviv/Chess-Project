#include "ChessGame.h"
#include <SDL.h>
#include <SDL_video.h>
#include "chessprog.h"

#define SAVE 0
#define LOAD 1
#define SHOULD_START_GAME 2
/*size of windows*/
#define PLAYERS_SELECTION_W 450
#define PLAYERS_SELECTION_H 600
#define MAIN_MENU_W 200
#define MAIN_MENU_H 300
#define SETTING_WINDOW_W 280
#define SETTING_WINDOW_H 350
#define GAME_WINDOW_W 800
#define GAME_WINDOW_H 650
#define PROMOTION_WINDOW_W 200
#define PROMOTION_WINDOW_H 150
#define SAVE_AND_LOAD_WINDOW_WIDTH 50+SLOT_X_DIF*NUM_OF_SLOTS
#define SAVE_AND_LOAD_WINDOW_HIGHT 270

/*files*/
#define IMG_NEW_GAME "Pictures/newgame.bmp"
#define IMG_LOAD_GAME "Pictures/loadgame.bmp"
#define IMG_QUIT_GAME "Pictures/quit.bmp"
#define IMG_PLAYER_VS_PLAYER "Pictures/playerVsPalyer.bmp"
#define IMG_PLAYER_VS_COMPUTER  "Pictures/playerVsComputer.bmp"
#define SETTINGS "Pictures/settings.bmp"
#define NEXT_PLAYER "Pictures/next.bmp"
#define IMG_CANCEL  "Pictures/Cancel.bmp"
#define IMG_CHESS "Pictures/ChessLogo.bmp"
#define IMG_ONE "Pictures/one.bmp"
#define IMG_TWO "Pictures/two.bmp"
#define IMG_THREE "Pictures/three.bmp"
#define IMG_FOUR "Pictures/four.bmp"
#define IMG_WHITE "Pictures/white.bmp"
#define IMG_BLACK "Pictures/black.bmp"
#define IMG_GAME_COLOR "Pictures/gameColor.bmp"
#define IMG_GAME_DIFFICULTY "Pictures/gameDifficulty.bmp"
#define IMG_SAVE_GAME "Pictures/Save.bmp"
#define IMG_REMOVE_GAME "Pictures/remove.bmp"
#define IMG_CLEAR_BOARD "Pictures/clear.bmp"
#define IMG_MAIN_MENU "Pictures/MainMenu1.bmp"
#define IMG_Difficulty "Pictures/gameDifficulty.bmp"
#define IMG_GAME_COLOR "Pictures/gameColor.bmp"
#define IMG_BEST_DEPTH "Pictures/best depth.bmp"
#define IMG_START "Pictures/start.bmp"
#define IMG_WHITE_SQUARE "Pictures/whiteSq.bmp"
#define IMG_BLACK_SQUARE "Pictures/blackSq.bmp"
#define IMG_CHESS_LOGO_2 "Pictures/ChessTitle3.bmp"
#define IMG_SET_ON_BOARD "Pictures/setOnBoard.bmp"
#define IMG_QUIT_GAME_BUTTON "Pictures/Quit1.bmp"
#define IMG_WHITE_PAWN_WHITE_SQURE "Pictures/whitePawnWhiteBoard.bmp"
#define IMG_WHITE_PAWN_BLACK_SQURE "Pictures/whitePawnBlackBoard.bmp"
#define IMG_BLACK_PAWN_WHITE_SQURE "Pictures/blackPawnWhiteBoard.bmp"
#define IMG_BLACK_PAWN_BLACK_SQURE "Pictures/blackPawnBlackBoard.bmp"
#define NEXT_PLAYER_SET "Pictures/next.bmp"
#define BLACK_NEXT_PLAYER "Pictures/blackNextPlayer.bmp"
#define WHITE_NEXT_PLAYER "Pictures/whiteNextPlayer.bmp"
#define IMG_START_PLAYER_SELECTION "Pictures/startGamePlayerSelection.bmp"
#define IMG_INS "Pictures/ins.bmp"
#define IMG_INS_BUTTON "Pictures/insButton.bmp"
#define TIE_IMG "Pictures/tie.bmp"
#define SET_BACK "Pictures/setBack.bmp"
#define WHITE_TURN "Pictures/turnWhite.bmp"
#define BLACK_TURN "Pictures/turnBlack.bmp"
#define TURN_BLACK_W 184
#define TURN_BLACK_H 24
#define SET_PIECES_WINDOW_W 550
#define SET_PIECES_WINDOW_H 300
/*game pieces*/
#define IMG_WHITE_ROOK_WHITE_SQURE "Pictures/whiteRookWhiteBoard.bmp"
#define IMG_WHITE_ROOK_BLACK_SQURE "Pictures/whiteRookBlackBoard.bmp"
#define IMG_BLACK_ROOK_WHITE_SQURE "Pictures/blackRookWhiteBoard.bmp"
#define IMG_BLACK_ROOK_BLACK_SQURE "Pictures/blackRookBlackBoard.bmp"

#define IMG_WHITE_QUEEN_WHITE_SQURE "Pictures/whiteQueenWhiteBoard.bmp"
#define IMG_WHITE_QUEEN_BLACK_SQURE "Pictures/whiteQueenBlackBoard.bmp"
#define IMG_BLACK_QUEEN_WHITE_SQURE "Pictures/blackQueenWhiteBoard.bmp"
#define IMG_BLACK_QUEEN_BLACK_SQURE "Pictures/blackQueenBlackBoard.bmp"

#define IMG_WHITE_BISHOP_WHITE_SQURE "Pictures/whiteBishopWhiteBoard.bmp"
#define IMG_WHITE_BISHOP_BLACK_SQURE "Pictures/whiteBishopBlackBoard.bmp"
#define IMG_BLACK_BISHOP_WHITE_SQURE "Pictures/blackBishopWhiteBoard.bmp"
#define IMG_BLACK_BISHOP_BLACK_SQURE "Pictures/blackBishopBlackBoard.bmp"

#define IMG_WHITE_KNIGHT_WHITE_SQURE "Pictures/whiteKnightWhiteBoard.bmp"
#define IMG_WHITE_KNIGHT_BLACK_SQURE "Pictures/whiteKnightBlackBoard.bmp"
#define IMG_BLACK_KNIGHT_WHITE_SQURE "Pictures/blackKnightWhiteBoard.bmp"
#define IMG_BLACK_KNIGHT_BLACK_SQURE "Pictures/blackKnightBlackBoard.bmp"

/*game pieces for best moves*/
#define IMG_WHITE_KING_WHITE_SQURE_BLUE "Pictures/whiteKingWhiteBoardBlue.bmp"
#define IMG_WHITE_KING_BLACK_SQURE_BLUE "Pictures/whiteKingBlackBoardBlue.bmp"
#define IMG_BLACK_KING_WHITE_SQURE_BLUE "Pictures/blackKingWhiteBoardBlue.bmp"
#define IMG_BLACK_KING_BLACK_SQURE_BLUE "Pictures/blackKingBlackBoardBlue.bmp"


#define IMG_WHITE_ROOK_WHITE_SQURE_BLUE "Pictures/whiteRookWhiteBoardBlue.bmp"
#define IMG_WHITE_ROOK_BLACK_SQURE_BLUE "Pictures/whiteRookBlackBoardBlue.bmp"
#define IMG_BLACK_ROOK_WHITE_SQURE_BLUE "Pictures/blackRookWhiteBoardBlue.bmp"
#define IMG_BLACK_ROOK_BLACK_SQURE_BLUE "Pictures/blackRookBlackBoardBlue.bmp"

#define IMG_WHITE_QUEEN_WHITE_SQURE_BLUE "Pictures/whiteQueenWhiteBoardBlue.bmp"
#define IMG_WHITE_QUEEN_BLACK_SQURE_BLUE "Pictures/whiteQueenBlackBoardBlue.bmp"
#define IMG_BLACK_QUEEN_WHITE_SQURE_BLUE "Pictures/blackQueenWhiteBoardBlue.bmp"
#define IMG_BLACK_QUEEN_BLACK_SQURE_BLUE "Pictures/blackQueenBlackBoardBlue.bmp"

#define IMG_WHITE_BISHOP_WHITE_SQURE_BLUE "Pictures/whiteBishopWhiteBoardBlue.bmp"
#define IMG_WHITE_BISHOP_BLACK_SQURE_BLUE "Pictures/whiteBishopBlackBoardBlue.bmp"
#define IMG_BLACK_BISHOP_WHITE_SQURE_BLUE "Pictures/blackBishopWhiteBoardBlue.bmp"
#define IMG_BLACK_BISHOP_BLACK_SQURE_BLUE "Pictures/blackBishopBlackBoardBlue.bmp"

#define IMG_WHITE_KNIGHT_WHITE_SQURE_BLUE "Pictures/whiteKnightWhiteBoardBlue.bmp"
#define IMG_WHITE_KNIGHT_BLACK_SQURE_BLUE "Pictures/whiteKnightBlackBoardBlue.bmp"
#define IMG_BLACK_KNIGHT_WHITE_SQURE_BLUE "Pictures/blackKnightWhiteBoardBlue.bmp"
#define IMG_BLACK_KNIGHT_BLACK_SQURE_BLUE "Pictures/blackKnightBlackBoardBlue.bmp"

#define IMG_WHITE_PAWN_WHITE_SQURE_BLUE "Pictures/whitePawnWhiteBoardBlue.bmp"
#define IMG_WHITE_PAWN_BLACK_SQURE_BLUE "Pictures/whitePawnBlackBoardBlue.bmp"
#define IMG_BLACK_PAWN_WHITE_SQURE_BLUE "Pictures/blackPawnWhiteBoardBlue.bmp"
#define IMG_BLACK_PAWN_BLACK_SQURE_BLUE "Pictures/blackPawnBlackBoardBlue.bmp"

#define IMG_WHITE_KING_WHITE_SQURE "Pictures/whiteKingWhiteBoard.bmp"
#define IMG_WHITE_KING_BLACK_SQURE "Pictures/whiteKingBlackBoard.bmp"
#define IMG_BLACK_KING_WHITE_SQURE "Pictures/blackKingWhiteBoard.bmp"
#define IMG_BLACK_KING_BLACK_SQURE "Pictures/blackKingBlackBoard.bmp"
/*game pieces for set window*/
#define IMG_SET_WHITE_KING "Pictures/whiteKing.bmp"
#define IMG_SET_WHITE_QUEEN "Pictures/whiteQueen.bmp"
#define IMG_SET_WHITE_BISHOP "Pictures/whiteBishop.bmp"
#define IMG_SET_WHITE_KNIGHT "Pictures/whiteKnight.bmp"
#define IMG_SET_WHITE_ROOK "Pictures/whiteRook.bmp"
#define IMG_SET_WHITE_PAWN "Pictures/whitePawn.bmp"

#define IMG_SET_BLACK_KING "Pictures/blackKing.bmp"
#define IMG_SET_BLACK_QUEEN "Pictures/blackQueen.bmp"
#define IMG_SET_BLACK_BISHOP "Pictures/blackBishop.bmp"
#define IMG_SET_BLACK_KNIGHT "Pictures/blackKnight.bmp"
#define IMG_SET_BLACK_ROOK "Pictures/blackRook.bmp"
#define IMG_SET_BLACK_PAWN "Pictures/blackPawn.bmp"
/*promotion pictures*/
#define IMG_PROMOTION_BISHOP "Pictures/promotionBishop.bmp"
#define IMG_PROMOTION_QUEEN "Pictures/promotionQueen.bmp"
#define IMG_PROMOTION_ROOK "Pictures/promotionRook.bmp"
#define IMG_PROMOTION_KNIGHT "Pictures/promptionKnight.bmp"
#define IMG_PAWN_PROMOTION_OPTIONS "Pictures/pawnPromtionOptions.bmp"
#define NOT_BEST_MOVE "Pictures/notBestMove.bmp"
#define NOT_BEST_MOVE_W 154
#define NOT_BEST_MOVE_H 61
#define IMG_CHECK "Pictures/check.bmp"
#define IMG_BLACK_WINS "Pictures/checkMateBlackWin.bmp"
#define IMG_WHITE_WINS "Pictures/checkMateWhiteWin.bmp"
#define IMG_CHOOSE_SLOT "Pictures/slotChoosing.bmp"
/*slots*/
#define IMG_GREEN "Pictures/green.bmp"
#define IMG_YELLOW "Pictures/yellow.bmp"
#define IMG_GRAY "Pictures/gray.bmp"
#define IMG_BLUE "Pictures/blue.bmp"
#define IMG_PINK "Pictures/pink.bmp"
#define IMG_PURPLE "Pictures/purple.bmp"
#define IMG_ORANGE "Pictures/orange.bmp"
#define IMG_SLOT_IS_EMPTY "Pictures/slotIsEmpty.bmp"
#define CHOOSE_SET "Pictures/chooseSet.bmp"
#define CHOOSE_SET_W 154
#define CHOOSE_SET_H 61
#define IMG_BACK "Pictures/back.bmp"
#define IMG_ILLEGAL_START "Pictures/illegal.bmp"
#define ILLEGAL_W 128
#define ILLEGAL_H 68
#define IMG_CANCEL_SET "Pictures/cancelSet.bmp"
#define IMG_BEST_MOVE "Pictures/bestMoves.bmp"
#define IMG_BEST_SQURE "Pictures/bestSqure.bmp"
#define MODE "Pictures/mode.bmp"
#define SET_REMOVE_IMG "Pictures/setremove.bmp"
/* num of buttons and data*/
#define NUM_OF_BUTTONS 45+NUM_OF_SLOTS+ NUM_OF_PIECES_ROW*2
#define NUM_OF_DATA 4
#define NUM_OF_SLOTS 7
#define NUM_OF_COLORS 7
#define NUM_OF_PIECES_ROW 6

/*numbers of buttons*/
#define NEW_GAME_NUM 0
#define LOAD_GAME_NUM 1
#define QUIT_GAME_NUM 2
#define PLAYER_VS_PLAYER_NUM 4
#define PLAYER_VS_COMPUTER_NUM 5
#define SETTINGS_NUM 6
#define CANCEL_NUM 7
#define ONE_BUTTON_NUM 8
#define TWO_BUTTON_NUM 9
#define THREE_BUTTON_NUM 10
#define FOUR_BUTTON_NUM 11
#define WHITE_BUTTON_NUM 12
#define BLACK_BUTTON_NUM 3
#define CLEAR_BUTTON_NUM 13
#define SAVE_BUTTON_NUM 14
#define MAIN_MENU_BUTTON_NUM 15
#define BEST_DIFFCULTY_NUM 16
#define START_NUM 17
#define QUIT_GAME_BUTTON_NUM 18
#define PROMOTION_ROOK_NUM 19
#define PROMOTION_QUEEN_NUM 20
#define PROMOTION_BISHOP_NUM 21
#define PROMOTION_KNIGHT_NUM 22
#define CHECK_NUM 23
#define CHECK_MATE_NUM 24
#define SLOT_START_NUM 28
#define SET_PIECES_START_NUM_ROW1 36
#define SET_PIECES_START_NUM_ROW2 42
#define SET_PIECE_NUM 27
#define REMOVE_NUM 26
#define SAVE_LOAD_CANCEL_NUM SLOT_START_NUM+NUM_OF_SLOTS
#define SET_REMOVE_CANCEL 49
#define WHITE_NEXT_NUM 50
#define	BLACK_NEXT_NUM 51
#define START_SELECT_PLAYER 52
#define BEST_MOVE_NUM 53
#define INS_BUTTON_NUM 54
#define INS_NUM 56
#define TIE_NUM 57
#define SET_BACK_NUM 58
/*size of buttons*/
#define Y_MENU_DIFFRENCE 67
#define IMG_W 170
#define IMG_H 75
#define NUMBER_WIDTH 27
#define NUMBER_HIGHT 35
#define NUMBER_1_X 21
#define NUMBER_2_X 58
#define NUMBER_3_X 95
#define NUMBER_4_X 132
#define NUMBER_Y 100
#define DIFFICULTY_Y 33
#define DIFFICULTY_X 60
#define CHOOSE_SLOT_Y 33
#define CHOOSE_SLOT_X 50
#define BEST_DIFFCULTY_X 163
#define BEST_DIFFCULTY_Y 167
#define BEST_DIFFCULTY_WIDTH 120
#define BEST_DIFFCULTY_HIGHT 35
#define GAME_COLOR_Y 150
#define GAME_COLOR_X 75
#define GAME_COLOR_WIDTH 142
#define GAME_COLOR_HIGHT 24
#define CHESS_IMG_Y 20
#define NEW_GAME_Y 100
#define LOAD_GAME_Y 167
#define QUIT_GAME_Y 234
#define WHITE_BUTTON_X 41
#define BLACK_BUTTON_X 132
#define COLOR_Y 200
#define START_Y 250
#define START_X 47
#define COLOR_WIDTH 71
#define COLOR_HIGHT 38

#define PLAYER_VS_PLAYER_Y 85 
#define PLAYER_VS_COMPUTER_Y 200
#define CANCEL_Y 470
#define NEXT_PLAYER_Y 310
#define GAME_BUTTON_WINDOW_X 650
#define SET_BUTTON_Y 333
#define SAVE_BUTTON_Y 233
#define INS_BUTTON_Y 333
#define MAIN_MENU_BUTTON_Y 133
#define BEST_MOVE_BUTTON_Y 433
#define QUIT_GAME_BUTTON_Y 533
#define SQUARE_HIGHT_AND_WIDTH 70
#define SET_WINDOW_PIECES_Y 150
#define X 12
#define SETTING_Y 230
#define WHITE_NEXT_X X + 30
#define BLACK_NEXT_X X+220
#define ADD_RE_SET_Y 160
#define NEXT_PLAYER_COLOR_Y 380
#define INS_W 719
#define INS_H 546
#define CHESS_LOGO_2_X 300
#define CHESS_LOGO_2_Y 1
#define CHESS_LOGO_2_HIGHT 90
#define CHESS_LOGO_2_WIDTH 223
#define CENTER_PIECE 10
#define PROMOTION_BUTTON_HIGHT 45
#define PROMOTION_BUTTON_WIDTH 35
#define PROMOTION_BUTTON_Y 100
#define PROMOTION_ROOK_X 20
#define PROMOTION_QUEEN_X 60
#define PROMOTION_KNIGHT_X 100
#define PROMOTION_BISHOP_X 140
#define PAWN_PROMOTION_OPTIONS_X 15
#define PAWN_PROMOTION_OPTIONS_Y 20
#define CHECK_MATE_WIDTH 750
#define CHECK_MATE_HIGHT 600
#define CHECK_MATE_X 0
#define CHECK_MATE_Y 0
#define CHECK_WIDTH 252
#define CHECK_HIGHT 351
#define CHECK_X 0
#define CHECK_Y 0
#define SLOT_X 0
#define SLOT_X_DIF 37
#define SLOT_Y 100
#define SLOT_IS_EMPTY_X 65
#define SLOT_IS_EMPTY_Y 150
#define SAVE_LOAD_CANCEL_X 43
#define SAVE_LOAD_CANCEL_Y 190
#define CANCEL_SET_Y 433
#define START_SELECT_X 240
#define INS_WIN_X 719
#define INS_WIN_Y 546
#define GAME_MODE_W 315
#define GAME_MODE_H 59
#define cancel_x X + 30
#define TIE_W 750 
#define TIE_H 600


/*functions*/
SDL_Surface * loadImg(char* path);
int addImage(SDL_Surface * img, SDL_Rect offset, int imgW, int imgH);
/*general*/
int make_main_menu();
int gui_mode();
void gui_settings(char board[BOARD_SIZE][BOARD_SIZE]);
int pressed_button(SDL_MouseButtonEvent event, int button);
void buttonSetting(int[NUM_OF_BUTTONS][NUM_OF_DATA]);
void AddToArrSettings(int x, int y, int w, int h, int buttons[NUM_OF_BUTTONS][NUM_OF_DATA], int buttonNum);
int MainMenuClear();
int addBoardToGameWindow(char board[BOARD_SIZE][BOARD_SIZE], MovesList * moveList);
/*windows*/
int InsWindow(void);
int players_selection_window();
int setting_window();
int SettingForPlayervsPlayerWindow(void);
int promotion_question_window();
int checkMate_window(char* playerColor);
int check_window();
int tie_window();
int saveOrLoad_window(int option);
int makeGameWindow(char board[BOARD_SIZE][BOARD_SIZE], MovesList* bestForMove);
/*handles*/
int HandleSelectionEventInPlayerSelection();
int HandleSelectionEventInPlayerVsPlayerSettings(char board[BOARD_SIZE][BOARD_SIZE]);
int HandleSelectionEventInSettings();
int HandleSelectionEventInGameWindow(char board[BOARD_SIZE][BOARD_SIZE], char *playerColor);
int HandleIns(void);
int HandleCheck(void);
int HandleTie(void);
int HandleSelectionEventInaddRemoveWindow(char board[BOARD_SIZE][BOARD_SIZE]);
int HandleSelectionEventInaddRemoveForSetWindow(char board[BOARD_SIZE][BOARD_SIZE], int j, int i);
int HandleSelectionEventInPlayerSelection(char board[BOARD_SIZE][BOARD_SIZE]);
char HandlePromotionQuestion(char* playerColor);
int HandleLoadWindow();
int HandleSaveWindow();
int HandleCheckMate();



SDL_Event e;

SDL_Rect offset;

/*
* a node of data yet to be freed
*/


struct free_surface_node{
	SDL_Surface *surface;
	struct free_surface_node *next;

};

typedef struct free_surface_node free_surface_node;

/*
* a list of all data to be freed
*/
struct free_surface_list{
	free_surface_node *head;
};
typedef struct free_surface_list free_surface_list;

free_surface_list *useSurfaceSpace;
void free_freeSurfaceNode(free_surface_node * curToFreeNode);
void validate_surface_allocate(SDL_Surface * pointer, char functionName[50]);
void add_to_user_surface_list(SDL_Surface * allocated);
void free_used_surface_list();


