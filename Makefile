all:
	gcc -o rngs.o -c rngs.c
	gcc avvia_simulazione.c -lm -o simu rngs.o
	gcc starter.c -o start
debug:
	gcc -o rngs.o -c rngs.c
	gcc avvia_simulazione.c -DDEBUG -lm -o simu rngs.o
	gcc starter.c -o start
