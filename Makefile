
LINKS = -std=c++20 -lSDL2 -lm -Wall -g  -rdynamic -fsanitize=address

all: colours.o vect.o distfuncs.o camera.o scene.o
	g++ main.c colours.o vect.o distfuncs.o camera.o scene.o $(LINKS) -o blackpink

colours.o: colours.c colours.h
	g++ $(LINKS) -c colours.c -o colours.o

vect.o: vect.c vect.h
	g++ $(LINKS) -c vect.c -o vect.o

distfuncs.o: distfuncs.c distfuncs.h
	g++ $(LINKS) -c distfuncs.c -o distfuncs.o

camera.o: camera.c 
	g++ $(LINKS) -c camera.c -o camera.o

scene.o: scene.c
	g++ $(LINKS) -c scene.c -o scene.o

pres: presentation.md
	pandoc -t beamer presentation.md -o pres.pdf    

clean:
	rm *.o blackpink
