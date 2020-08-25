all:
	gcc -o rngs.o -c rngs.c
	gcc avvia_simulazione.c -lm -o simu rngs.o
