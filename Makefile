
LINKS = -lSDL2 -lm -Wall -g

all: colours.o vect.o distfuncs.o
	gcc main.c colours.o vect.o distfuncs.o $(LINKS) -o blackpink

colours.o: colours.c colours.h
	gcc $(LINKS) -c colours.c -o colours.o

vect.o: vect.c vect.h
	gcc $(LINKS) -c vect.c -o vect.o

distfuncs.o: vect.c vect.h
	gcc $(LINKS) -c distfuncs.c -o distfuncs.o

clean:
