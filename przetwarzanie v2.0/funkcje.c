#include "funkcje.h"

/*Funkcja wczytujaca obraz PGM*/
int czytaj(FILE *p, t_obraz *obrazek){
  char s[DL_LINII];
  int znak,koniec=0,i,j;  
  
  /*sprawdzenie czy podano prawidlowy uchwyt pliku */
  if (p==NULL) {
    fprintf(stderr,"Blad: Nie podano uchwytu do pliku\n");
    return(0);
  }
  if (fgets(s,DL_LINII,p)==NULL) koniec=1;
  /* Sprawdzenie "numeru magicznego - powinien byc P2 */
  if ( (s[0]!='P') || (s[1]!='2') || koniec) {
    fprintf(stderr,"Blad: To nie jest plik PGM\n");
    return(0);
  }
  /* Pominiecie komentarzy */  
 do{
    if ((znak=fgetc(p))=='#') {
      if (fgets(s,DL_LINII,p)==NULL) koniec=1;
    }  else {
      ungetc(znak,p);
    }
  } while (! koniec && znak=='#');
 
  /* Pobranie szerokosci obrazu */
 if (fscanf(p,"%d \n",&(obrazek->wym_x))!=1) {
    fprintf(stderr,"Blad: Brak wymiaru szerokosci obrazu\n");
    return(0);
  }       
  
  /* Pominiecie komentarzy */
  do{
    if ((znak=fgetc(p))=='#') {
      if (fgets(s,DL_LINII,p)==NULL) koniec=1;
    }  else {
      ungetc(znak,p);
    }
  } while (! koniec && znak=='#');
  
  /* Pobranie wysokosci obrazu */
  if (fscanf(p,"%d \n",&(obrazek->wym_y))!=1) {
    fprintf(stderr,"Blad: Brak wymiaru wysokosci obrazu\n");
    return(0);
  }  

  /* Pominiecie komentarzy */
  do{
    if ((znak=fgetc(p))=='#') {
      if (fgets(s,DL_LINII,p)==NULL) koniec=1;
    }  else {
      ungetc(znak,p);
    }
  } while (! koniec && znak=='#');
  
  /*  Pobieranie liczby odcieni szarosci */
  if (fscanf(p,"%d",&(obrazek->max_szarosc))!=1) {
    fprintf(stderr,"Blad: Brak liczby stopni szarosci\n");
    
    return(0);
  }

  obrazek->tab = malloc(obrazek->wym_x*obrazek->wym_y*sizeof(int));
  
  int (*piksele)[obrazek->wym_x];
  piksele = (int(*)[obrazek->wym_x]) obrazek->tab;

  /* Pobranie obrazu i zapisanie w tablicy obraz_pgm*/
  for (i=0;i<obrazek->wym_y;i++) {
    for (j=0;j<obrazek->wym_x;j++) {
      if (fscanf(p,"%d",&(piksele[i][j]))!=1) {
	fprintf(stderr,"Blad: Niewlasciwe wymiary obrazu\n");
	return(0);
      }
    }
  }
  return obrazek->wym_x*obrazek->wym_y;
}
/*Funkcja wypisujaca wartosci tablicy*/
void wypisz(t_obraz *obrazek)
{
  int i,j;

  int (*piksele)[obrazek->wym_x];
  piksele = (int(*)[obrazek->wym_x]) obrazek->tab;

  for (j=0; j<obrazek->wym_y; j++){
    for (i=0; i<obrazek->wym_x; i++){
      printf("%3d", piksele[j][i]);
    } 
    printf("\n");
  }  
}

/*Funkcja wykonujaca negatyw obrazu*/
void zmien_znak(t_obraz *obrazek){
  int i,j;

  int (*piksele)[obrazek->wym_x];
  piksele = (int(*)[obrazek->wym_x]) obrazek->tab;

  for (j=0; j<obrazek->wym_y; j++){
    for (i=0; i<obrazek->wym_x; i++){
      piksele[j][i]=obrazek->max_szarosc-piksele[j][i];
    }
  }
}
/*Funkcja zapisujaca obraz*/
void zapisz(t_obraz *obrazek, FILE *p){ 
  int i,j;

  int (*piksele)[obrazek->wym_x];
  piksele = (int(*)[obrazek->wym_x]) obrazek->tab;

  fprintf(p,"P2\n%d %d %d",obrazek->wym_x,obrazek->wym_y,obrazek->max_szarosc);
  for (i=0; i<obrazek->wym_y; i++){
    fprintf(p,"\n");
    for (j=0; j<obrazek->wym_x; j++){
      fprintf(p,"%d ",piksele[i][j]);
    }
  }
}
/*Funkcja wykonujaca rozmycie poziome. Funkcja ta wymaga okreslenia przez uzytkownika
  promien rozmywania. Im wiekszy promien tym wieksze rozmycie. Promien powinien byc
  wiekszy od zera, w przeciwnym razie program wyswietli komunikat o bledzie.*/
void rozmycie_poziome(t_obraz *obrazek, int x){
  int i,j;

  int (*piksele)[obrazek->wym_x];
  piksele = (int(*)[obrazek->wym_x]) obrazek->tab;

    for (i=0; i<obrazek->wym_y; i++){
      for (j=0; j<obrazek->wym_x; j++){
	if(((piksele[i][j-x]+piksele[i][j]+piksele[i][j+x])/3)>0){
	  piksele[i][j]=(piksele[i][j-x]+piksele[i][j]+piksele[i][j+x])/3;
	}
      }
    }
    fprintf(stderr,"Wykonano rozmycie poziome\n");
}
/*Funkcja wykonujaca rozmycie pionowe. Funkcja ta wymaga okreslenia przez uzytkownika
  promien rozmywania. Im wiekszy promien tym wieksze rozmycie. Promien powinien byc
  wiekszy od zera, w przeciwnym razie program wyswietli komunikat o bledzie.*/
void rozmycie_pionowe(t_obraz *obrazek, int x){
  int i,j;

  int (*piksele)[obrazek->wym_x];
  piksele = (int(*)[obrazek->wym_x]) obrazek->tab;		      
  
  for (i=0; i<obrazek->wym_y; i++){
    for (j=0; j<obrazek->wym_x; j++){
      if(i-x<0){
	piksele[i][j]=(1.5*piksele[i][j]+piksele[i+x][j])/3;
      }
      else{
	if(((piksele[i-x][j]+piksele[i][j]+piksele[i+x][j])/3)>0){
	  piksele[i][j]=(piksele[i-x][j]+piksele[i][j]+piksele[i+x][j])/3;
	}
      }
    }
  }
fprintf(stderr,"Wykonano rozmycie pionowe\n");
}
/*Funkcja wykonujaca zmiane poziomow. Funkcja ta pozwala na podanie nowych wartosci 
  oznaczajacych biel i czern. Zarowno nowa wartosc bieli jak i czerni powinna byc liczba 
  dodatnia (wymagane jest podanie liczby a nie ilosci procentow)*/
void zmiana_poziomow(t_obraz *obrazek, int CZERN, int BIEL){
  int i,j;

  int (*piksele)[obrazek->wym_x];
  piksele = (int(*)[obrazek->wym_x]) obrazek->tab;

  for (i=0; i<obrazek->wym_y; i++){
    for (j=0; j<obrazek->wym_x; j++){
      if (piksele[i][j] <= CZERN){
	piksele[i][j]=0;
      }
      else if (piksele[i][j] < BIEL){
	piksele[i][j]=(piksele[i][j]-CZERN)*obrazek->max_szarosc/(BIEL-CZERN);
      }
      else{
	piksele[i][j]=obrazek->max_szarosc;
      }
    }
  }
}
/*Funkcja wyliczajaca procenty. Ma na celu ulatwienie uzytkownikowi programu wpisywanie
  wartosci takich jak biel, czern oraz prog. Na podstawie maksymalnej szarosci wylicza
  wartosci jakie przypisane sa poszczegolnym procentom (co 10%) */
void licz_procenty(t_obraz *obrazek){
  int k;
  int proc = 0;
  char z = 37;
  printf("\n");
  for(k=10; k<=100; k=k+10){
    proc=obrazek->max_szarosc*k/100;
    printf("\tZa %d%c odpowiada liczba %d\n",k,z,proc);
  }
}
/*Funkcja wykonujaca progowanie. Polega na klasyfikacji piksela do jednej z dwoch klas - zbioru
  punktow czarnych lub zbioru punktow bialych. Wymaganie jest podanie wartosci progu ktory bedzie
  decydowal ktory piksel zostanie zakwalifikowany do pikseli czarnych a ktory do bialych.
  Piksele powyzej progu beda biale natomiast piksele ponizej beda czarne. Wartosc progu powinna byc 
  wieksza od zera.*/
void progowanie(t_obraz *obrazek, int PROG){
  int i,j;

  int (*piksele)[obrazek->wym_x];
  piksele = (int(*)[obrazek->wym_x]) obrazek->tab;

  for (i=0; i<obrazek->wym_y; i++){
    for (j=0; j<obrazek->wym_x; j++){
      if(piksele[i][j] <= PROG){
	piksele[i][j] = 0;
      }
      else
	piksele[i][j] = obrazek->max_szarosc;
    }
  }
}
/*Funkcja wykonujaca polprogowanie czerni. Operacja podobna do progowania. Wymaganie jest podanie 
  wartosci progu ktory bedzie decydowal ktory piksel zostanie zakwalifikowany do pikseli czarnych 
  a ktory pozostanie bez zmian. Piksele powyzej progu beda bez zmian natomiast piksele ponizej beda 
  czarne. Wartosc progu powinna byc wieksza od zera.*/
void polprogowanie_czerni(t_obraz *obrazek, int PROG){
  int i,j;

  int (*piksele)[obrazek->wym_x];
  piksele = (int(*)[obrazek->wym_x]) obrazek->tab;

  for (i=0; i<obrazek->wym_y; i++){
    for (j=0; j<obrazek->wym_x; j++){
      if(piksele[i][j] <= PROG){
	piksele[i][j] = 0;
      }
      else
	piksele[i][j] = piksele[i][j];
    }
  }
}
/*Funkcja wykonujaca polprogowanie bieli. Operacja podobna do progowania. Wymaganie jest podanie 
  wartosci progu ktory bedzie decydowal ktory piksel zostanie zakwalifikowany do wartosci
  max szarosc a ktory pozostanie bez zmian. Piksele powyzej progu beda mialy wartosc max szarosci
  natomiast piksele ponizej beda biale. Wartosc progu powinna byc wieksza od zera.*/
void polprogowanie_bieli(t_obraz *obrazek, int PROG){
  int i,j;

  int (*piksele)[obrazek->wym_x];
  piksele = (int(*)[obrazek->wym_x]) obrazek->tab;

  for (i=0; i<obrazek->wym_y; i++){
    for (j=0; j<obrazek->wym_x; j++){
      if(piksele[i][j] <= PROG){
	piksele[i][j] = piksele[i][j];
      }
      else
	piksele[i][j] = obrazek->max_szarosc;
    }
  }
}
/*Funkcja wykonujaca konturowanie. Funkcja ta rozjasnia piksele ktore roznia sie od otoczenia
  natomiast przyciemnia te ktore sa podobne do niego*/
void konturowanie(t_obraz *obrazek){
  int i,j;

  int (*piksele)[obrazek->wym_x];
  piksele = (int(*)[obrazek->wym_x]) obrazek->tab;

  for (i=0; i<obrazek->wym_y; i++){
    for (j=0; j<obrazek->wym_x; j++){
      piksele[i][j]=abs(piksele[i+1][j]-piksele[i][j])+abs(piksele[i][j+1]-piksele[i][j]);
    }
  }
}
/*Funkcja wykonujaca rozciaganie histogramu. Wykonuje sie ja gdy jasnosci pikseli obrazu nie 
pokrywaja calego zakresu szarosci obrazu. Rozciagniecie prowadzi do takiej konwersji zakresu wartosci,
aby jasnoci pikseli obejmowaly caly zakres jasnosci dostepny w obrazie. */
void rozciaganie_histogramu(t_obraz *obrazek, int min, int max){ 
  int i,j;
  min = obrazek->max_szarosc;

  int (*piksele)[obrazek->wym_x];
  piksele = (int(*)[obrazek->wym_x]) obrazek->tab;

  for (i=0; i<obrazek->wym_y; i++){
    for (j=0; j<obrazek->wym_x; j++){
      if(piksele[i][j]>max){
	max=piksele[i][j];
      }
      if(piksele[i][j]<min){
	min=piksele[i][j];
      }
    }    
  }
  for (i=0; i<obrazek->wym_y; i++){
    for (j=0; j<obrazek->wym_x; j++){
      piksele[i][j]=(piksele[i][j]-min)*(obrazek->max_szarosc/(max-min));
    }
  }
}
/*Funkcja wykonujaca korekte gamma. Jest to operacja przeskalowujaca wartosci jasnosci pikseli obrazu.
  Wymagane jest podanie przez uzytkownika parametru gamma. Parametr powinien byc dodatni. Wartosci
  miedzy 0 a 1 przyciemniaja obraz natomiast wartosci powyzej 1 (wlacznie z 1) rozjasniaja obraz.  */
void korekta_gamma(t_obraz *obrazek, float gamma){
  int i,j;

  int (*piksele)[obrazek->wym_x];
  piksele = (int(*)[obrazek->wym_x]) obrazek->tab;

  for (i=0; i<obrazek->wym_y; i++){
    for (j=0; j<obrazek->wym_x; j++){
      piksele[i][j] = (pow(piksele[i][j],1/gamma))/(pow(obrazek->max_szarosc,(1-gamma)/gamma));
    }
  }
}
/*Funkcja wyswietlajaca zapisany wczesniej obraz za pomoca programu display*/
void wyswietl (char *nazwa_zapis) {
  /* Wyswietlenie obrazu o zadanej nazwie za pomoca programu "display" */
  char polecenie[1024];
  strcpy(polecenie,"display ");
  strcat(polecenie,nazwa_zapis);
  strcat(polecenie," &");
  printf("%s\n",polecenie);
  system(polecenie);
}
