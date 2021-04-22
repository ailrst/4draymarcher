
LINKS = -lSDL2 -lm -Wall -g
CC=gcc

ifeq ($(target),windows)
	CC=x86_64-w64-mingw32-gcc -DWIN32 -g
	SDL_INCLUDE = -Dmain=SDL_main -Lsdllib -lSDL2main -lSDL2main -lSDL2_mixer -lSDL2_ttf -lSDL2 -lmingw32 -lSDL2main -lSDL2 -mwindows  -Wl,-static -lpthread -lm
	LINKS=$(SDL_INCLUDE) -O3
endif


all: colours.o vect.o distfuncs.o camera.o scene.o
	$(CC) main.c colours.o vect.o distfuncs.o camera.o scene.o $(LINKS) -o blackpink

colours.o: colours.c colours.h
	$(CC) $(LINKS) -c colours.c -o colours.o

vect.o: vect.c vect.h
	$(CC) $(LINKS) -c vect.c -o vect.o

distfuncs.o: distfuncs.c distfuncs.h
	$(CC) $(LINKS) -c distfuncs.c -o distfuncs.o

camera.o: camera.c 
	$(CC) $(LINKS) -c camera.c -o camera.o

scene.o: scene.c
	$(CC) $(LINKS) -c scene.c -o scene.o

pres: presentation.md
	pandoc -t beamer presentation.md -o pres.pdf    

clean:
	rm *.o blackpink
