brog: main.o ennemi.o
	gcc main.o ennemi.o -o brog -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf

main.o: main.c
	gcc -c main.c -g

ennemi.o: ennemi.c
	gcc -c ennemi.c -g

