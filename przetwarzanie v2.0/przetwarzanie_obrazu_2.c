#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
#include "funkcje.h"
#define DL_LINII 80 /*Max rozmiar nazwy pliku*/ 
#define PRAWDA 1 /*Dopoki koniec != PRAWDA menu jest wlaczone*/
#define FALSZ 3 /*Jesli koniec=FALSZ mozna wlaczyc funkcje, zapisac obraz oraz wyswietlic go. koniec=FALSZ gdy wczytano obraz */
#define MENU 2 /*Gdy wroc=MENU mozna powrocic do poprzedniego menu. */
#define B_NIEPOPRAWNAOPCJA -1 
#define B_BRAKNAZWY -2

/************************************************************************/
/* Funkcja main rozpoznaje opcje wywolania programu zapisane w tablicy  */
/* argv                                                                 */
/*                                                                      */
/* Skladnia opcji wywolania programu:                                   */
/*    program [-i nazwa] [-p liczba] [-n] [inne opcje] [-o nazwa] [-d]  */
/* Argumenty funkcji:                                                   */
/*    argc  -  liczba argumentow wywolania wraz z nazwa programu        */
/*    argv  -  tablica argumentow wywolania                             */
/* PRE:                                                                 */
/*      brak                                                            */
/* POST:                                                                */
/*      funkcja otwiera odpowiednie pliki i zwraca uchwyty do nich      */
/* UWAGA:                                                               */
/*      funkcja nie sprawdza istnienia i praw dostepu do plikow         */
/*      w takich przypadkach zwracane uchwyty maja wartosc NULL         */
/************************************************************************/


int main(int argc, char ** argv) {
  int i,x;  /*i-zmienna przemieszczajaca sie po argumentach wywolania, x-promien rozmywania*/
  char opcja[2]; /* wybor opcji w menu prze wlaczeniu programu bez argumentow wywolania */
  t_obraz obrazek; /*zmienna zawierajaca informacje o obrazku */
  FILE *plik; 
  char nazwa[80];
  int odczytano = 0;
  FILE *plik_zapis;
  int min = 0, max = 0; /* min szarosci i max szarosci */
  char nazwa_zapis[80];
  int BIEL,CZERN,PROG; 
  float gamma = 0.0;
  int koniec,wroc; 
  char *nazwa1;
  char *nazwa_zapis1;
  
    
  for (i=1; i<argc; i++){   
    if (argv[i][0] != '-'){   /*sprawdzenie czy pierwszym znakiem w argumencie jest '-'*/ 
      fprintf(stderr,"Blad: Zly argument wywolania. Sprawdz '--help'\n");
      return B_NIEPOPRAWNAOPCJA;
    }
    if (argv[1][0]=='-' && argv[1][1]=='-' && argv[1][2]=='h' && argv[1][3]=='e' && argv[1][4]=='l' && argv[1][5]=='p'){ /*Gdy pierwszy argument wywolania to '--help'to pokaz specyfikacje programu*/
      fprintf(stderr,"\n*****************************************************************************************");
      fprintf(stderr,"\n* Program do przetwarzania obrazow o formacie pgm. Gdy program wykona ktoras z opcji    *\n");
      fprintf(stderr,"* powinien wyswietlic o tym komunikat.                                                  *\n");
      fprintf(stderr,"*                                                                                       *\n");
      fprintf(stderr,"* Skladnia opcji wywolania programu:                                                    *\n");
      fprintf(stderr,"* program [-i nazwa] [-p liczba] [-n] [inne opcje] [-o nazwa] [-d]                      *\n");
      fprintf(stderr,"*                                                                                       *\n");
      fprintf(stderr,"* Dostepne opcje:                                                                       *\n");
      fprintf(stderr,"*---------------------------------------------------------------------------------------*");
      fprintf(stderr,"\n* [-i nazwa]   -   Funkcja wczytujaca obraz. Musi byc wywolana na poczatku.             *\n");
      fprintf(stderr,"*                  Jako kolejny argument wywolania musi zostac podana nazwa pliku       *\n");
      fprintf(stderr,"*                  Przykladowe uzycie funkcji:                                          *\n");
      fprintf(stderr,"*                                                                                       *\n");
      fprintf(stderr,"*        ./a.out -i obraz.pgm                                                           *\n");
      fprintf(stderr,"*---------------------------------------------------------------------------------------*");
      fprintf(stderr,"\n* [-o nazwa]   -   Funkcja zapisujaca obraz. Musi byc wywolana na koncu lub             *\n");
      fprintf(stderr,"*                  przed opcja -d. Jako kolejny argument wywolania musi zostac podana   *\n");
      fprintf(stderr,"*                  nazwa pliku do zapisu. Przykladowe uzycie funkcji:                   *\n");
      fprintf(stderr,"*                                                                                       *\n");
      fprintf(stderr,"*        ./a.out -i obraz.pgm [operacje na obrazie] -o obraz2.pgm                       *\n");
      fprintf(stderr,"*---------------------------------------------------------------------------------------*");
      fprintf(stderr,"\n* [-n]      -      Funkcja wykonujaca negatyw. Musi byc wywolana po wczytaniu           *\n");
      fprintf(stderr,"*                  obrazu oraz przed zapisaniem pliku. Przykladowe uzycie funkcji:      *\n");
      fprintf(stderr,"*                                                                                       *\n");
      fprintf(stderr,"*        ./a.out -i obraz.pgm -n -o obraz2.pgm                                          *\n");
      fprintf(stderr,"*---------------------------------------------------------------------------------------*");
      fprintf(stderr,"\n* [-p liczba]  -   Funkcja wykonujaca progowanie. Musi byc wywolana po wczytaniu        *\n"); 
      fprintf(stderr,"*                  obrazu oraz przed zapisaniem pliku. Po uzyciu opcji -p nalezy        *\n");
      fprintf(stderr,"*                  podac wartosc progu. Przykladowe uzycie funkcji:                     *\n");
      fprintf(stderr,"*                                                                                       *\n");
      fprintf(stderr,"*        ./a.out -i obraz.pgm -p 50 -o obraz2.pgm                                       *\n");
      fprintf(stderr,"*---------------------------------------------------------------------------------------*");
      fprintf(stderr,"\n* [-pc liczba]  -  Funkcja wykonujaca polprogowanie czerni. Musi byc wywolana           *\n"); 
      fprintf(stderr,"*                  po wczytaniu obrazu oraz przed zapisaniem pliku. Po uzyciu           *\n");
      fprintf(stderr,"*                  opcji -p nalezy podac wartosc progu. Przykladowe uzycie funkcji:     *\n");
      fprintf(stderr,"*                                                                                       *\n");
      fprintf(stderr,"*        ./a.out -i obraz.pgm -pc 100 -o obraz2.pgm                                     *\n");
      fprintf(stderr,"*---------------------------------------------------------------------------------------*");
      fprintf(stderr,"\n* [-pb liczba]  -  Funkcja wykonujaca polprogowanie bieli. Musi byc wywolana            *\n"); 
      fprintf(stderr,"*                  po wczytaniu obrazu oraz przed zapisaniem pliku. Po uzyciu           *\n");
      fprintf(stderr,"*                  opcji -p nalezy podac wartosc progu. Przykladowe uzycie funkcji:     *\n");
      fprintf(stderr,"*                                                                                       *\n");
      fprintf(stderr,"*        ./a.out -i obraz.pgm -pb 100 -o obraz2.pgm                                     *\n");
      fprintf(stderr,"*---------------------------------------------------------------------------------------*");
      fprintf(stderr,"\n* [-k]      -      Funkcja wykonujaca Konturowanie. Musi byc wywolana po wczytaniu      *\n");
      fprintf(stderr,"*                  obrazu oraz przed zapisaniem pliku. Przykladowe uzycie funkcji:      *\n");
      fprintf(stderr,"*                                                                                       *\n");
      fprintf(stderr,"*        ./a.out -i obraz.pgm -k -o obraz2.pgm                                          *\n");
      fprintf(stderr,"*---------------------------------------------------------------------------------------*");
      fprintf(stderr,"\n* [-h]      -      Funkcja wykonujaca rozciaganie histogramu. Musi byc wywolana po      *\n");
      fprintf(stderr,"*                  wczytaniu obrazu oraz przed zapisaniem pliku. Przykladowe uzycie     *\n");
      fprintf(stderr,"*                  funkcji:                                                             *\n");
      fprintf(stderr,"*                                                                                       *\n");
      fprintf(stderr,"*        ./a.out -i obraz.pgm -h -o obraz2.pgm                                          *\n");
      fprintf(stderr,"*---------------------------------------------------------------------------------------*");
      fprintf(stderr,"\n* [-g liczba]  -   Funkcja wykonujaca korekte gamma. Musi byc wywolana po wczytaniu     *\n"); 
      fprintf(stderr,"*                  obrazu oraz przed zapisaniem pliku. Po uzyciu opcji -g nalezy        *\n");
      fprintf(stderr,"*                  podac wartosc parametru gamma. Przykladowe uzycie funkcji:           *\n");
      fprintf(stderr,"*                                                                                       *\n");
      fprintf(stderr,"*        ./a.out -i obraz.pgm -g 0.5 -o obraz2.pgm                                      *\n");
      fprintf(stderr,"*---------------------------------------------------------------------------------------*");
      fprintf(stderr,"\n* [-rx liczba]  -  Funkcja wykonujaca rozmycie poziome. Musi byc wywolana               *\n"); 
      fprintf(stderr,"*                  po wczytaniu obrazu oraz przed zapisaniem pliku. Po uzyciu           *\n");
      fprintf(stderr,"*                  opcji -rx nalezy podac promien rozmywania. Przykladowe uzycie        *\n");
      fprintf(stderr,"*                  funkcji:                                                             *\n");
      fprintf(stderr,"*                                                                                       *\n");
      fprintf(stderr,"*        ./a.out -i obraz.pgm -rx 5 -o obraz2.pgm                                       *\n");
      fprintf(stderr,"*---------------------------------------------------------------------------------------*");
      fprintf(stderr,"\n* [-ry liczba]  -  Funkcja wykonujaca rozmycie pionowe. Musi byc wywolana               *\n"); 
      fprintf(stderr,"*                  po wczytaniu obrazu oraz przed zapisaniem pliku. Po uzyciu           *\n");
      fprintf(stderr,"*                  opcji -ry nalezy podac promien rozmywania. Przykladowe uzycie        *\n");
      fprintf(stderr,"*                  funkcji:                                                             *\n");
      fprintf(stderr,"*                                                                                       *\n");
      fprintf(stderr,"*        ./a.out -i obraz.pgm -ry 4 -o obraz2.pgm                                       *\n");
      fprintf(stderr,"*---------------------------------------------------------------------------------------*");
      fprintf(stderr,"\n* [-z CZERN BIEL]  -  Funkcja wykonujaca zmiane poziomow. Musi byc wywolana             *\n"); 
      fprintf(stderr,"*                  po wczytaniu obrazu oraz przed zapisaniem pliku. Po uzyciu           *\n");
      fprintf(stderr,"*                  opcji -z nalezy podac wartosc czerni a nastepnie bieli.              *\n"); 
      fprintf(stderr,"*                  Przykladowe uzycie funkcji:                                          *\n");
      fprintf(stderr,"*                                                                                       *\n");
      fprintf(stderr,"*         ./a.out -i obraz.pgm -z 40 60 -o obraz2.pgm                                   *\n");
      fprintf(stderr,"*---------------------------------------------------------------------------------------*");
      fprintf(stderr,"\n* [-d]      -      Funkcja wyswietlajaca obraz za pomoca programu display. Musi byc     *\n"); 
      fprintf(stderr,"*                  wywolana po zapisaniu obrazu. Przykladowe uzycie funkcji             *\n");
      fprintf(stderr,"*                                                                                       *\n");
      fprintf(stderr,"*        ./a.out -i obraz.pgm [operacje na obrazie] -o obraz2.pgm -d                    *\n");
      fprintf(stderr,"*                                                                                       *\n");
      fprintf(stderr,"*****************************************************************************************\n");

      return 0;
    }
    switch (argv[i][1]) {
    case'i': { /*Wczytanie pliku*/
      
      if (++i<argc) {   /*kolejny argument wywolania to nazwa pliku wejsciowego*/
	nazwa1=argv[i]; 
	plik=fopen(nazwa1,"r");
	
	if (plik != NULL){         /* co spowoduje zakomentowanie tego warunku */
	  odczytano = czytaj (plik,&obrazek);    /*wczytanie obrazu*/
	  fprintf(stderr,"Wczytano obraz\n");
	}
	else 
	  fprintf(stderr,"Nie odczytano pliku o tej nazwie\n");	
	fclose(plik);      
      }
      else 
	return B_BRAKNAZWY;	
      break;
    }
    case 'o': {    /*Zapisanie pliku*/
      if (++i<argc) {   /*kolejny argument wywolania to nazwa pliku wyjsciowego*/
	nazwa_zapis1=argv[i];
	plik_zapis=fopen(nazwa_zapis1,"w");
	
	if (plik_zapis != NULL){
	  zapisz(&obrazek,plik_zapis);
	  fprintf(stderr,"Zapisano plik\n");
	}
	fclose(plik_zapis);
      }
      else 
	return B_BRAKNAZWY;
      break;
    }
    case 'n': {  /*Wykonaj negatyw*/
      zmien_znak (&obrazek); 
      fprintf(stderr,"Wykonano negatyw\n");
      break;
    }
    case 'k': {   /*Wykonaj konturowania*/
      konturowanie(&obrazek); 
      fprintf(stderr,"Wykonano konturowanie\n");
      break;
    }
    case 'h': {   /*Wykonaj rozciaganie histogramu*/
      rozciaganie_histogramu(&obrazek, min, max);   
      fprintf(stderr,"Wykonano rozciaganie histogramu\n");
      break; 
    }
    case 'p': {   /*Wykonaj progowanie*/
      if(argv[i][2]=='\0'){   /*Jesli po 'p' jest pusty znak to wykonaj progowanie*/
	if (++i<argc) {   /*Kolejny argument wywolania jest wartoscia progu*/
	  if (sscanf(argv[i],"%d",&PROG)==1){
	    if(PROG<0){
	      fprintf(stderr,"Blad: PROG mniejszy od zera\n");
	    }
	    else{
	      progowanie(&obrazek,PROG);  /*funkcja progowania*/
	      fprintf(stderr,"Wykonano progowanie\n");
	    }
	  }
	  else {
	    fprintf(stderr,"Blad: Nie podano wartosci progowania.\n");
	  }
	}
      }
      else if (argv[i][2]=='c' && argv[i][3]=='\0'){   /*Jesli po 'p' jest 'c' to wykonaj polprogowanie czerni*/
	if (++i<argc) {    /*Kolejny argument wywolania jest wartoscia progu*/
	  if (sscanf(argv[i],"%d",&PROG)==1){
	    
	    if(PROG<0){
	      fprintf(stderr,"Blad: PROG mniejszy od zera\n");
	    }
	    else{
	      polprogowanie_czerni(&obrazek,PROG);   /*funkcja polprogowania czerni*/
	      fprintf(stderr,"Wykonano polprogowanie czerni\n");
	    }
	  }
	  else {
	    fprintf(stderr,"Blad: Nie podano wartosci progowania.\n");
	  }
	}
      }
      else if (argv[i][2]=='b' && argv[i][3]=='\0'){   /*Jesli po 'p' jest 'b' to wykonaj polprogowanie bieli*/
	if (++i<argc) {    /*Kolejny argument wywolania jest wartoscia progu*/
	  if (sscanf(argv[i],"%d",&PROG)==1){
	    
	    if(PROG<0){
	      fprintf(stderr,"Blad: PROG mniejszy od zera\n");
	    }
	    else{
	      polprogowanie_bieli(&obrazek,PROG);   /*funkcja polprogowania bieli*/
	      fprintf(stderr,"Wykonano polprogowanie bieli\n");
	    }
	  }
	  else {
	    fprintf(stderr,"Blad: Nie podano wartosci progowania.\n");
	  }
	}
      }
      break;
    }	
    case 'g': {   /*Wykonaj korekte gamma*/
      if (++i<argc) {   /*Kolejny argument wywolania jest parametrem gamma*/
	if (sscanf(argv[i],"%f",&gamma)==1){
	  
	  if(gamma<0){
	    fprintf(stderr,"Blad: Gamma mniejsza od zera\n");
	  }
	  else if(gamma==0){
	    fprintf(stderr,"Blad: Gamma rowna zero\n");
	  }
	  else{
	    korekta_gamma(&obrazek,gamma);   /*funkcja korekty gamma*/
	    fprintf(stderr,"Wykonano korekte gamma\n");
	  }
	}
      }
      break; 
    }
    case 'z': {   /*Wykonaj zmiane poziomow*/
      if (++i<argc) {   /*Kolejny argument wywolania to CZERN*/
	if (sscanf(argv[i],"%d",&CZERN)==1){
	  if (++i<argc) {   /*Kolejny argument wywolania to BIEL*/
	    if (sscanf(argv[i],"%d",&BIEL)==1){
	      if(BIEL<0 || CZERN<0){
		fprintf(stderr,"Blad: Wartosc bieli lub czerni mniejsza od zera\n");
	      }
	      else{
		zmiana_poziomow(&obrazek,CZERN,BIEL);   /*funkcja zmiany poziomow*/
		fprintf(stderr,"Wykonano zmiane poziomow\n");
	      }
	    }
	  else {
	    fprintf(stderr,"Blad: Nie podano wartosci bieli.\n");
	  }
	  }
	}
	  else {
	    fprintf(stderr,"Blad: Nie podano wartosci czerni.\n");
	  }
      }
      break;
    }
    case 'r': {   /*Wykonaj rozmycie*/
      if(argv[i][2]=='x' && argv[i][3]=='\0'){   /*Jesli kolejny znak po 'r' to 'x' to wykonaj rozmycie poziome*/
	if (++i<argc) {   /*Kolejny argument wywolania to wartosc promienia rozmywania*/
	  if (sscanf(argv[i],"%d",&x)==1){
	    if(x<0){
	      fprintf(stderr,"Blad: Promien mniejszy od zera\n");
	    }
	    else if(x==0){
	      fprintf(stderr,"Blad: Promien rowny zero\n");
	    }		
	    else {
	      rozmycie_poziome (&obrazek,x);    /*Funkcja rozmycia poziomego*/
	    }
	  }
	  else {
	    fprintf(stderr,"Blad: Nie podano wartosci promienia rozmywania.\n");
	  }
	}
      }
      else if(argv[i][2]=='y' && argv[i][3]=='\0'){  /*Jesli kolejny znak po 'r' to 'y' to wykonaj rozmycie poziome*/
	if (++i<argc) {   /*Kolejny argument wywolania to wartosc promienia rozmywania*/
	  if (sscanf(argv[i],"%d",&x)==1){
	    if(x<0){
	      fprintf(stderr,"Blad: Promien mniejszy od zera\n");
	    }
	    else if(x==0){
	      fprintf(stderr,"Blad: Promien rowny zero\n");
	    }		
	    else {
	      rozmycie_pionowe (&obrazek,x);    /*Funkcja rozmycia pionowego*/
	    }
	  }
	  else {
	    fprintf(stderr,"Blad: Nie podano wartosci promienia rozmywania.\n");
	  }
	}
      }
      break;
    } 
    case 'd': {  /*Pokaz obrazek*/
      plik=fopen(nazwa_zapis1,"r");      
      if (odczytano != 0){
	wyswietl(nazwa_zapis1);  /*funkcja wyswietlajaca obraz*/
	fclose(plik);
      }
      break;
    }
    }
  }
  
  if(argc==1){   /*Gdy jedyny argument wywolania programu to nazwa tego programu wlacz tryb wolnego menu*/
    
    do{
      fprintf(stderr,"Opcje menu:\n");                      /*MENU GLOWNE*/
      fprintf(stderr,"\t 1 - Wczytaj plik\n");
      fprintf(stderr,"\t 2 - Zapisz obraz\n");
      fprintf(stderr,"\t 3 - Funkcje\n");
      fprintf(stderr,"\t 4 - Pokaz zapisany wczesniej obraz (Display)\n");
      fprintf(stderr,"\t 5 - Zakoncz dzialanie\n");
      fprintf(stderr,"\t Twoj wybor:");
      
      scanf("%s",opcja);      
      switch (opcja[0]){
	
      case '1':  { 
	fprintf(stderr,"Podaj nazwe pliku do odczytu:\n");
	scanf("%s",nazwa);
	plik=fopen(nazwa,"r");
	
	if (plik != NULL){         /* co spowoduje zakomentowanie tego warunku */
	  odczytano = czytaj (plik,&obrazek);    /*wczytanie obrazu*/
	  koniec = FALSZ;     /*uruchomienie "przelacznika" do dalszych funkcji*/
	}
	else 
	  fprintf(stderr,"Nie odczytano pliku o tej nazwie\n");
	fclose(plik);  
	break;
      }
	
      case '2':  {
	if(koniec==FALSZ){            /*Obraz zostal wczytany*/
	  fprintf(stderr,"Podaj nazwe pliku do zapisu: \n");
	  scanf("%s",nazwa_zapis);
	  plik_zapis=fopen(nazwa_zapis,"w");
      if (plik_zapis != NULL){
	zapisz(&obrazek,plik_zapis);
	fprintf(stderr,"Zapisano plik\n");
      }
      fclose(plik_zapis);
	}
	else  {
	  fprintf(stderr,"Blad: Najpierw wczytaj obraz\n");
	}
	break;
      }

      case '3':  {
	if(koniec==FALSZ){    /*Obraz zostal wczytany*//*MENU FUNKCJI*/
	  do{
	    fprintf(stderr,"Menu funkcji:\n");
	    fprintf(stderr,"\t 1 - Negatyw\n");
	    fprintf(stderr,"\t 2 - Rozmycie\n");
	    fprintf(stderr,"\t 3 - Zmiana Poziomow\n");
	    fprintf(stderr,"\t 4 - Progowanie\n");
	    fprintf(stderr,"\t 5 - Konturowanie\n");
	    fprintf(stderr,"\t 6 - Rozciaganie histogramu\n");
	    fprintf(stderr,"\t 7 - Korekta Gamma\n");
	    fprintf(stderr,"\t 8 - Wypisz\n");
	    fprintf(stderr,"\t 9 - Wroc do poprzedniego menu\n");
	fprintf(stderr,"\t Twoj wybor: ");
	scanf("%s",opcja);
	
	switch(opcja[0]){
	  
	case '1':  { 
	  zmien_znak (&obrazek);   /*funkcja negatyw*/
	  fprintf(stderr,"Wykonano negatyw\n");
	  break;
	}
	case '2':  {
	  do{
	    fprintf(stderr,"Rozmycia:\n");               /*MENU ROZMYCIA*/
	    fprintf(stderr,"\t 1 - Rozmycie poziome\n");	
	    fprintf(stderr,"\t 2 - Rozmycie pionowe\n");
	    fprintf(stderr,"\t 3 - Wroc do poprzedniego menu\n");
	    fprintf(stderr,"\t Twoj wybor: ");
	    scanf("%s",opcja);
	    
	    switch(opcja[0]){
	      
	    case '1':  {
	      fprintf(stderr,"Podaj promien rozmywania:");
	      scanf("%d",&x);
	      if(x<0){
		fprintf(stderr,"Blad: Promien mniejszy od zera\n");
	      }
	      else if(x==0){
		fprintf(stderr,"Blad: Promien rowny zero\n");
	      }		
	      else {
		rozmycie_poziome (&obrazek,x);    /*Funkcja rozmycia poziomego*/
	      }
	      break;
	    }
	    case '2':  {
	      fprintf(stderr,"Podaj promien rozmywania:");
	      scanf("%d",&x);
	      if(x<0){
		fprintf(stderr,"Blad: Promien mniejszy od zera\n");
	      }
	      else if(x==0){
		fprintf(stderr,"Blad: Promien rowny zero\n");
	      }		
	      else {
		rozmycie_pionowe (&obrazek,x);    /*Funkcja rozmycia pionowego*/
	      }	       
	      break;
	    }
	    case '3':  {
	      wroc=MENU;          /*uruchomienie "przelacznika" powrotu do poprzedniego menu*/
	      break;
	    }
	    default:  {fprintf(stderr,"Nieznana opcja\n"); break;}
	    }
	  }while(wroc != MENU);          /*wyjscia z menu rozmycia*/
	  wroc=0;           /*zerowanie "przelacznika powrotu do poprzedniego menu aby byl gotowy do ponownego uzycia*/
	  break;	
	}
	  
	case '3':  {
	  licz_procenty (&obrazek);  /*funkcja wyliczajaca procenty z max szarosci*/
	  fprintf(stderr,"\nPodaj wartosc bieli: ");
	  scanf("%d",&BIEL);
	  fprintf(stderr,"Podaj wartosc czerni: ");
	  scanf("%d",&CZERN);
	  if(BIEL<0 || CZERN<0){
	    fprintf(stderr,"Blad: Wartosc bieli lub czerni mniejsza od zera\n");
	  }
	  else{
	    zmiana_poziomow(&obrazek,CZERN,BIEL);   /*funkcja zmiany poziomow*/
	    fprintf(stderr,"Wykonano zmiane poziomow\n");
	  }
	  break;
	}
	  
	case '4':  {
	  do{
	    fprintf(stderr,"Progowania:\n");                     /*MENU PROGOWANIA*/
	    fprintf(stderr,"\t 1 - Progowanie zwykle\n");	
	    fprintf(stderr,"\t 2 - Polprogowanie bieli\n");
	    fprintf(stderr,"\t 3 - Polprogowanie czerni\n");
	    fprintf(stderr,"\t 4 - Wroc do poprzedniego menu\n");
	    fprintf(stderr,"\t Twoj wybor: ");
	    scanf("%s",opcja);
	    
	    switch(opcja[0]){
	      
	    case '1':  {      
	      licz_procenty (&obrazek);  /*funkcja wyliczajaca procenty z max szarosci*/
	      fprintf(stderr,"\nPodaj wartosc progu: ");
	      scanf("%d",&PROG);
	      if(PROG<0){
		fprintf(stderr,"Blad: PROG mniejszy od zera\n");
	      }
	      else{
		progowanie(&obrazek,PROG);  /*funkcja progowania*/
		fprintf(stderr,"Wykonano progowanie\n");
	      }
	      break;
	    }
	      
	    case '2':   {      
	      licz_procenty (&obrazek);  /*funkcja wyliczajaca procenty z max szarosci*/
	      fprintf(stderr,"\nPodaj wartosc progu: ");
	      scanf("%d",&PROG);
	      if(PROG<0){
		fprintf(stderr,"Blad: PROG mniejszy od zera\n");
	      }
	      else{
		polprogowanie_bieli(&obrazek,PROG);   /*funkcja polprogowania bieli*/
		fprintf(stderr,"Wykonano polprogowanie bieli\n");
	      }
	      break;
	    }  
	      
	    case '3':  {      
	      licz_procenty (&obrazek);   /*funkcja wyliczajaca procenty z max szarosci*/
	      fprintf(stderr,"\nPodaj wartosc progu: ");
	      scanf("%d",&PROG);
	      if(PROG<0){
		fprintf(stderr,"Blad: PROG mniejszy od zera\n");
	      }
	      else{
		polprogowanie_czerni(&obrazek,PROG);   /*funkcja polprogowania czerni*/
		fprintf(stderr,"Wykonano polprogowanie czerni\n");
	      }
	      break;
	    }
	      
	    case '4':  { 
	      wroc=MENU;         /*uruchomienie "przelacznika" powrotu do poprzedniego menu*/
	      break;
	    }
	      
	    default:  fprintf(stderr,"Nieznana opcja\n"); break;
	    }
	  }while(wroc != MENU);   /*wyjscie z menu progowania*/
	  wroc=0;
	  break;
	}
	  
	case '5':  {      
	  konturowanie(&obrazek);   /*funkcja konturowania*/
	  fprintf(stderr,"Wykonano konturowanie\n");
	  break;
	}
	  
	case '6':  {
	  rozciaganie_histogramu(&obrazek, min, max);   
	  fprintf(stderr,"Wykonano rozciaganie histogramu\n");
	  break;
	}
	  
	case '7':  {
	  fprintf(stderr,"Podaj parametr gamma: ");
	  scanf("%f",&gamma);
	  if(gamma<0){
	    fprintf(stderr,"Blad: Gamma mniejsza od zera\n");
	  }
	  else if(gamma==0){
	    fprintf(stderr,"Blad: Gamma rowna zero\n");
	  }
	  else{
	    korekta_gamma(&obrazek,gamma);   /*funkcja korekty gamma*/
	    fprintf(stderr,"Wykonano korekte gamma\n");
	  }
	  break;
	}
	  
	case '8':  wypisz (&obrazek); break;
	  
	case '9':  {
	  wroc=MENU;           /*uruchomienie "przelacznika" powrotu do poprzedniego menu*/
	  break;
	}	  
	default:  fprintf(stderr,"Nieznana opcja\n"); break;
	}
	  }while(wroc != MENU);             /*wyjscie z menu funkcji*/
	  wroc=0;
	}
	else  {
	  fprintf(stderr,"Blad: Najpierw wczytaj obraz\n");
	}
	break;
    }
	
      case '4':  {
	if(koniec==FALSZ){   /*Obraz zostal wczytany*/
	  plik=fopen(nazwa_zapis,"r");      
	  if (odczytano != 0)
	    wyswietl(nazwa_zapis);  /*funkcja wyswietlajaca obraz*/
	  fclose(plik);
	}
	else {
	  fprintf(stderr,"Blad: Najpierw wczytaj obraz\n");
	}
	break;
      }
	
      case '5':  {
	fprintf(stderr,"Koniec dzialania programu\n"); 
	koniec = PRAWDA;
	break;
      }
      default:  fprintf(stderr,"Nieznana opcja\n"); break;
      }
    }while(koniec != PRAWDA);     /*wyjscie z glownego menu*/
  }
  return 0;
}

