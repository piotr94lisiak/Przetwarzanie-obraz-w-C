#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
#define OBRAZ 1400
#define DL_LINII 80

typedef struct {
  int wym_x, wym_y, max_szarosc;
  void *tab;
} t_obraz;


int czytaj(FILE *p, t_obraz *obrazek);
void wypisz(t_obraz *obrazek);
void zmien_znak(t_obraz *obrazek);
void zapisz(t_obraz *obrazek, FILE *p);
void rozmycie_poziome(t_obraz *obrazek, int x);
void rozmycie_pionowe(t_obraz *obrazek, int x);
void zmiana_poziomow(t_obraz *obrazek, int CZERN, int BIEL);
void licz_procenty(t_obraz *obrazek);
void progowanie(t_obraz *obrazek, int PROG);
void polprogowanie_czerni(t_obraz *obrazek, int PROG);
void polprogowanie_bieli(t_obraz *obrazek, int PROG);
void konturowanie(t_obraz *obrazek);
void rozciaganie_histogramu(t_obraz *obrazek, int min, int max);
void korekta_gamma(t_obraz *obrazek, float gamma);
void wyswietl (char *nazwa);
