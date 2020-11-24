
LINKS = -lSDL2 -lm -Wall -g
GCC = g++

all: colours.o vect.o distfuncs.o camera.o scene.o
	$(GCC) main.c colours.o vect.o distfuncs.o camera.o scene.o $(LINKS) -o blackpink

colours.o: colours.c colours.h
	$(GCC) $(LINKS) -c colours.c -o colours.o

vect.o: vect.c vect.h
	$(GCC) $(LINKS) -c vect.c -o vect.o

distfuncs.o: distfuncs.c distfuncs.h
	$(GCC) $(LINKS) -c distfuncs.c -o distfuncs.o

camera.o: camera.c 
	$(GCC) $(LINKS) -c camera.c -o camera.o

scene.o: scene.c
	$(GCC) $(LINKS) -c scene.c -o scene.o

pres: presentation.md
	pandoc -t beamer presentation.md -o pres.pdf    

clean:
	rm *.o blackpink
