all:
	gcc -o rngs.o -c rngs.c
	gcc avvia_simulazione.c -lm -o simu rngs.o
debug:
	gcc -o rngs.o -c rngs.c
	gcc avvia_simulazione.c -DDEBUG -lm -o simu rngs.o
