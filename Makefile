
LINKS = -lSDL2 -lm -Wall -g

all: colours.o vect.o distfuncs.o camera.o scene.o
	gcc main.c colours.o vect.o distfuncs.o camera.o scene.o $(LINKS) -o blackpink

colours.o: colours.c colours.h
	gcc $(LINKS) -c colours.c -o colours.o

vect.o: vect.c vect.h
	gcc $(LINKS) -c vect.c -o vect.o

distfuncs.o: distfuncs.c distfuncs.h
	gcc $(LINKS) -c distfuncs.c -o distfuncs.o

camera.o: camera.c 
	gcc $(LINKS) -c camera.c -o camera.o

scene.o: scene.c
	gcc $(LINKS) -c scene.c -o scene.o

clean:
	rm *.o blackpink
