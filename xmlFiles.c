
#include "xmlFiles.h"
#include "chessprog.h"


FILE* create_xml_file(char* fileName){
	/* creates xml file of the given file */

	FILE* xmlFile;

	xmlFile = fopen(fileName, "w+");

	if (xmlFile == NULL) {
		return xmlFile;
	}

	if (fprintf(xmlFile, "%s", "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n") < 0){
		perror("Error: standard function printf has failed");
		exit(1);
	}

	return xmlFile;
}




void write_game_to_xml_file(char board[BOARD_SIZE][BOARD_SIZE], FILE* xmlFile, int mode, int difficulty, int best, char* userColor, char* nextTurn){
	/* writes game information (game settings and board) to the given xml file */

	int i, j;
	char* diff;
	char num[10] = { 0 };

	if (best == 1){
		diff = "best";
	}
	else{
		num[0] = (char)(difficulty + '0');
		diff = num;
	}

	/*write game settings*/
	if (fprintf(xmlFile, "%s\n", "<game>") < 0){
		perror("Error: standard function printf has failed");
		exit(1);
	}

	if (mode == 2) { /*player vs. AI*/
		if (strcmp(userColor, "white") == 0){
			if (fprintf(xmlFile, "\t%s%s%s\n\t%s%d%s\n\t%s%s%s\n\t%s%s%s\n", "<next_turn>", "White", "</next_turn>", "<game_mode>", mode, "</game_mode>", "<difficulty>", diff, "</difficulty>", "<user_color>", "White", "</user_color>") < 0) {
				perror("Error: standard function printf has failed");
				exit(1);
			}
		}
		else{
			if (fprintf(xmlFile, "\t%s%s%s\n\t%s%d%s\n\t%s%s%s\n\t%s%s%s\n", "<next_turn>", "Black", "</next_turn>", "<game_mode>", mode, "</game_mode>", "<difficulty>", diff, "</difficulty>", "<user_color>", "Black", "</user_color>") < 0){
				perror("Error: standard function printf has failed");
				exit(1);
			}
		}
	}
	else{ /*player vs. player*/
		if (strcmp(nextTurn, "white") == 0){
			if (fprintf(xmlFile, "\t%s%s%s\n\t%s%d%s\n\t%s\n\t%s\n", "<next_turn>", "White", "</next_turn>", "<game_mode>", mode, "</game_mode>", "<difficulty></difficulty>", "<user_color></user_color>") < 0){
				perror("Error: standard function printf has failed");
				exit(1);
			}
		}
		else{
			if (fprintf(xmlFile, "\t%s%s%s\n\t%s%d%s\n\t%s\n\t%s\n", "<next_turn>", "Black", "</next_turn>", "<game_mode>", mode, "</game_mode>", "<difficulty></difficulty>", "<user_color></user_color>") < 0){
				perror("Error: standard function printf has failed");
				exit(1);
			}
		}
	}

	/*write board*/
	if (fprintf(xmlFile, "\t%s\n", "<board>") < 0){
		perror("Error: standard function printf has failed");
		exit(1);
	}

	for (j = BOARD_SIZE - 1; j >= 0; j--) {
		if (fprintf(xmlFile, "\t\t%s%d%s", "<row_", j + 1, ">") < 0){
			perror("Error: standard function printf has failed");
			exit(1);
		}
		for (i = 0; i < BOARD_SIZE; i++)  {
			if (board[i][j] == EMPTY) {
				if (fprintf(xmlFile, "%c", '_') < 0) {
					perror("Error: standard function printf has failed");
					exit(1);
				}
			}
			else{
				if (fprintf(xmlFile, "%c", board[i][j]) < 0){
					perror("Error: standard function printf has failed");
					exit(1);
				}
			}
		}
		if (fprintf(xmlFile, "%s%d%s%c", "</row_", j + 1, ">", '\n') < 0){
			perror("Error: standard function printf has failed");
			exit(1);
		}
	}
	if (fprintf(xmlFile, "\t%s\n", "</board>") < 0) {
		perror("Error: standard function printf has failed");;
		exit(1);
	}
	if (fprintf(xmlFile, "%s\n", "</game>") < 0){
		perror("Error: standard function printf has failed");
		exit(1);
	}
}


char* xml_file_to_string(char* xmlFileName) {
	/* returns the xml file content as a string */

	char* fileContentString = NULL;

	FILE* fp = fopen(xmlFileName, "r");
	if (fp == NULL) {
		return NULL;
	}

	if (fseek(fp, 0L, SEEK_END) != 0) {  /*fseek fail*/
		fputs("Error occured while loading file", stderr);
		fclose(fp);
		return NULL;
	}

	int bufferSize = ftell(fp);
	if (bufferSize == -1) {		/*ftell fail*/
		fputs("Error occured while loading file", stderr);
		fclose(fp);
		return NULL;
	}

	fileContentString = malloc(sizeof(char)* (bufferSize + 1));

	if (fileContentString == NULL){
		perror("Error: standard function printf has failed");
		exit(1);
	}

	if (fseek(fp, 0L, SEEK_SET) != 0) {  /*fseek fail*/
		fputs("Error occured while loading file", stderr);
		fclose(fp);
		return NULL;
	}

	if (fread(fileContentString, sizeof(char), bufferSize, fp) == 0){ /* 0 elements were read*/
		fputs("Error reading file", stderr);
		fclose(fp);
		return NULL;
	}
	else {
		fileContentString[bufferSize] = '\0';
		fclose(fp);
		return fileContentString;
	}

}


char* get_turn_mode_difficulty_color(char* xmlFileString, char* field){
	/* given a field <next_turn> or <game_mode> or <difficulty> or <user_color>, returns the field's value in the given xml file */

	char* valPointer;
	char* fieldValue = NULL;
	char num[10] = { 0 };
	valPointer = strstr(xmlFileString, field) + strlen(field);

	if ((strcmp(field, "<next_turn>") == 0) || (strcmp(field, "<user_color>") == 0)){
		if (valPointer[0] == 'W'){
			fieldValue = "white";
		}
		else{
			fieldValue = "black";
		}
	}
	else if (strcmp(field, "<game_mode>") == 0){
		if (valPointer[0] == '1'){
			fieldValue = "1";
		}
		else{
			fieldValue = "2";
		}
	}
	else if (strcmp(field, "<difficulty>") == 0){
		if (valPointer[0] == 'b'){
			fieldValue = "best";
		}
		else{
			num[0] = valPointer[0];
			fieldValue = num;
		}
	}
	return fieldValue;

}


void xmlRow_to_consoleRow(char board[BOARD_SIZE][BOARD_SIZE], char* xmlFileString, char* field, int row){
	/* updates row x of the console game board according to the details of field <row x> in the xml file */

	char* xmlRow;
	int j;
	xmlRow = strstr(xmlFileString, field) + strlen(field);

	for (j = 0; j < BOARD_SIZE; ++j){
		if (xmlRow[j] == '_'){
			board[j][row] = EMPTY;
		}
		else{
			board[j][row] = xmlRow[j];
		}
	}
}


void xmlBoard_to_consoleBoard(char board[BOARD_SIZE][BOARD_SIZE], char* xmlFileString){
	/* updates the console game board according to the board described in the xml file */

	xmlRow_to_consoleRow(board, xmlFileString, "<row_8>", 7);
	xmlRow_to_consoleRow(board, xmlFileString, "<row_7>", 6);
	xmlRow_to_consoleRow(board, xmlFileString, "<row_6>", 5);
	xmlRow_to_consoleRow(board, xmlFileString, "<row_5>", 4);
	xmlRow_to_consoleRow(board, xmlFileString, "<row_4>", 3);
	xmlRow_to_consoleRow(board, xmlFileString, "<row_3>", 2);
	xmlRow_to_consoleRow(board, xmlFileString, "<row_2>", 1);
	xmlRow_to_consoleRow(board, xmlFileString, "<row_1>", 0);

}



