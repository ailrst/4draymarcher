
LINKS = -lSDL2 -lm -Wall -g

all: colours.o
	gcc main.c colours.o $(LINKS) -o blackpink

colours.o: colours.c colours.h
	gcc $(LINKS) -c colours.c -o colours.o

clean:
