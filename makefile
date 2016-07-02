all: chessprog 

LIBS = -L /usr/local/lib/SDL_image-1.2.12/lib -lSDL_image
INCLUDES = -I /usr/local/lib/SDL_image-1.2.12/include/

clean:
	-rm chessprog.o Moves.o ChessGame.o minmax.o xmlFiles.o Gui.o 

chessprog: chessprog.o Moves.o minmax.o ChessGame.o xmlFiles.o Gui.o 
	gcc $(LIBS) -o chessprog chessprog.o Moves.o minmax.o ChessGame.o xmlFiles.o Gui.o -lm -ansi -pedantic-errors -g `sdl-config --libs`

chessprog.o: chessprog.c chessprog.h Moves.h minmax.h ChessGame.h xmlFiles.h Gui.h
	gcc $(INCLUDES) -ansi -pedantic-errors -c -Wall -g -lm chessprog.c `sdl-config --cflags`


Moves.o: Moves.c Moves.h chessprog.h minmax.h ChessGame.h xmlFiles.h 
	gcc $(INCLUDES) -ansi -pedantic-errors -c -Wall -g -lm Moves.c `sdl-config --cflags`

ChessGame.o: ChessGame.c ChessGame.h Moves.h chessprog.h xmlFiles.h minmax.h
	gcc $(INCLUDES) -ansi -pedantic-errors -c -Wall -g -lm ChessGame.c `sdl-config --cflags`

xMlFiles.o: xmlFiles.c xmlFiles.h chessprog.h Moves.h ChessGame.h minmax.h Gui.h 
	gcc $(INCLUDES) -ansi -pedantic-errors -c -Wall -g -lm xmlFiles.c `sdl-config --cflags`

Gui.o: Gui.c chessprog.h Moves.h ChessGame.h xmlFiles.h Gui.h minmax.h
	gcc $(INCLUDES) -ansi -pedantic-errors -c -Wall -g -lm Gui.c `sdl-config --cflags`

minmax.o:  minmax.c minmax.h chessprog.h Moves.h ChessGame.h Gui.h
	gcc $(INCLUDES) -ansi -pedantic-errors -c -Wall -g -lm minmax.c `sdl-config --cflags` 











