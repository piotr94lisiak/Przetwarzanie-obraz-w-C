a.out: przetwarzanie_obrazu.o funkcje.o
	gcc przetwarzanie_obrazu.o funkcje.o -lm

przetwarzanie_obrazu.o: przetwarzanie_obrazu.c funkcje.h
	gcc -std=c99  -c -Wall -pedantic -lm przetwarzanie_obrazu.c

funkcje.o: funkcje.h funkcje.c
	gcc  -std=c99 -c -Wall -pedantic -lm funkcje.c
