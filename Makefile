
LINKS = -lSDL2 -lm -Wall

all: colours.o
	gcc main.c $(LINKS) colours.o -o blackpink

colours.o: colours.c colours.h
	gcc $(LINKS) -c colours.c -o colours.o
