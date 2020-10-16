
LINKS = -lSDL2 -lm

all:
	gcc main.c $(LINKS) -g -o blackpink
