main: main.o fonctions.o
	gcc -o main main.o fonctions.o

main.o:
	gcc -g -c main.c

fonctions.o: fonctions.h
	gcc -c fonctions.c fonctions.h

clean:
	rm -f *.o *.gch main
