#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
#define OBRAZ 1400
#define DL_LINII 80
#define PRAWDA 1
#define FALSZ 0
#define MENU 2

/*Funkcja wczytujaca obraz PGM*/
int czytaj(FILE *p, int tab[][OBRAZ], int *wym_x, int *wym_y, int *max_szarosc){
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
  if (fscanf(p,"%d \n",wym_x)!=1) {
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
  if (fscanf(p,"%d \n",wym_y)!=1) {
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
  if (fscanf(p,"%d",max_szarosc)!=1) {
    fprintf(stderr,"Blad: Brak liczby stopni szarosci\n");
    
    return(0);
  }

  /* Pobranie obrazu i zapisanie w tablicy obraz_pgm*/
  for (i=0;i<*wym_y;i++) {
    for (j=0;j<*wym_x;j++) {
      if (fscanf(p,"%d",&(tab[i][j]))!=1) {
	fprintf(stderr,"Blad: Niewlasciwe wymiary obrazu\n");
	return(0);
      }
    }
  }
  return *wym_x**wym_y;
}
/*Funkcja wypisujaca wartosci tablicy*/
int wypisz(int tab[][OBRAZ], int wym_x, int wym_y){
  int i,j;
  for (j=0; j<wym_y; j++){
    for (i=0; i<wym_x; i++){
      printf("%3d", tab[j][i]);
    } 
    printf("\n");
  }  
}

/*Funkcja wykonujaca negatyw obrazu*/
int zmien_znak(int tab[][OBRAZ], int wym_x, int wym_y, int max_szarosc){
  int i,j;
  for (j=0; j<wym_y; j++){
    for (i=0; i<wym_x; i++){
      tab[j][i]=max_szarosc-tab[j][i];
    }
  }
}
/*Funkcja zapisujaca obraz*/
int zapisz(int max_szarosc, int wym_y, int wym_x, int tab[][OBRAZ], FILE *p){ 
  int i,j;
  fprintf(p,"P2\n%d %d %d",wym_x,wym_y,max_szarosc);
  for (i=0; i<wym_y; i++){
    fprintf(p,"\n");
    for (j=0; j<wym_x; j++){
      fprintf(p,"%d ",tab[i][j]);
    }
  }
}
/*Funkcja wykonujaca rozmycie poziome. Funkcja ta wymaga okreslenia przez uzytkownika
  promien rozmywania. Im wiekszy promien tym wieksze rozmycie. Promien powinien byc
  wiekszy od zera, w przeciwnym razie program wyswietli komunikat o bledzie.*/
int rozmycie_poziome(int tab[][OBRAZ], int wym_x, int wym_y){
  int i,j,x;
  printf("Podaj promien rozmywania: ");
  scanf("%d",&x);
  if(x<0){
    printf("Blad: Promien mniejszy od zera\n");
  }
  else if(x==0){
    printf("Blad: Promien rowny zero\n");
  }
  else {
    for (i=0; i<wym_y; i++){
      for (j=0; j<wym_x; j++){
	if(((tab[i][j-x]+tab[i][j]+tab[i][j+x])/3)>0){
	  tab[i][j]=(tab[i][j-x]+tab[i][j]+tab[i][j+x])/3;
	}
      }
    }
    printf("Wykonano rozmycie poziome\n");
  }
}
/*Funkcja wykonujaca rozmycie pionowe. Funkcja ta wymaga okreslenia przez uzytkownika
  promien rozmywania. Im wiekszy promien tym wieksze rozmycie. Promien powinien byc
  wiekszy od zera, w przeciwnym razie program wyswietli komunikat o bledzie.*/
int rozmycie_pionowe(int tab[][OBRAZ], int wym_x, int wym_y){
  int i,j,x;
  printf("Podaj promien rozmywania: ");
  scanf("%d",&x);
  if(x<0){
    printf("Blad: Promien mniejszy od zera\n");
  }
  else if(x==0){
    printf("Blad: Promien rowny zero\n");
  }
  else{
    for (i=0; i<wym_y; i++){
      for (j=0; j<wym_x; j++){
	if(((tab[i-x][j]+tab[i][j]+tab[i+x][j])/3)>0){
	  tab[i][j]=(tab[i-x][j]+tab[i][j]+tab[i+x][j])/3;
	}
      }
    }
    printf("Wykonano rozmycie pionowe\n");
  }
}
/*Funkcja wykonujaca zmiane poziomow. Funkcja ta pozwala na podanie nowych wartosci 
  oznaczajacych biel i czern. Zarowno nowa wartosc bieli jak i czerni powinna byc liczba 
  dodatnia (wymagane jest podanie liczby a nie ilosci procentow)*/
int zmiana_poziomow(int tab[][OBRAZ], int wym_x, int wym_y, int max_szarosc, int CZERN, int BIEL){
  int i,j;
  for (i=0; i<wym_y; i++){
    for (j=0; j<wym_x; j++){
      if (tab[i][j] <= CZERN){
	tab[i][j]=0;
      }
      else if (tab[i][j] < BIEL){
	tab[i][j]=(tab[i][j]-CZERN)*max_szarosc/(BIEL-CZERN);
      }
      else{
	tab[i][j]=max_szarosc;
      }
    }
  }
}
/*Funkcja wyliczajaca procenty. Ma na celu ulatwienie uzytkownikowi programu wpisywanie
  wartosci takich jak biel, czern oraz prog. Na podstawie maksymalnej szarosci wylicza
  wartosci jakie przypisane sa poszczegolnym procentom (co 10%) */
int licz_procenty(int tab[][OBRAZ], int wym_x, int wym_y, int max_szarosc, int proc){
  int i,j,k;
  char z = 37;
  printf("\n");
  for(k=10; k<=100; k=k+10){
    proc=max_szarosc*k/100;
    printf("\tZa %d%c odpowiada liczba %d\n",k,z,proc);
  }
}
/*Funkcja wykonujaca progowanie. Polega na klasyfikacji piksela do jednej z dwoch klas - zbioru
  punktow czarnych lub zbioru punktow bialych. Wymaganie jest podanie wartosci progu ktory bedzie
  decydowal ktory piksel zostanie zakwalifikowany do pikseli czarnych a ktory do bialych.
  Piksele powyzej progu beda biale natomiast piksele ponizej beda czarne. Wartosc progu powinna byc 
  wieksza od zera.*/
int progowanie(int tab[][OBRAZ], int wym_x, int wym_y, int max_szarosc, int PROG){
  int i,j;
  for (i=0; i<wym_y; i++){
    for (j=0; j<wym_x; j++){
      if(tab[i][j] <= PROG){
	tab[i][j] = 0;
      }
      else
	tab[i][j] = max_szarosc;
    }
  }
}
/*Funkcja wykonujaca polprogowanie czerni. Operacja podobna do progowania. Wymaganie jest podanie 
  wartosci progu ktory bedzie decydowal ktory piksel zostanie zakwalifikowany do pikseli czarnych 
  a ktory pozostanie bez zmian. Piksele powyzej progu beda bez zmian natomiast piksele ponizej beda 
  czarne. Wartosc progu powinna byc wieksza od zera.*/
int polprogowanie_czerni(int tab[][OBRAZ], int wym_x, int wym_y, int PROG){
  int i,j;
  for (i=0; i<wym_y; i++){
    for (j=0; j<wym_x; j++){
      if(tab[i][j] <= PROG){
	tab[i][j] = 0;
      }
      else
	tab[i][j] = tab[i][j];
    }
  }
}
/*Funkcja wykonujaca polprogowanie bieli. Operacja podobna do progowania. Wymaganie jest podanie 
  wartosci progu ktory bedzie decydowal ktory piksel zostanie zakwalifikowany do wartosci
  max szarosc a ktory pozostanie bez zmian. Piksele powyzej progu beda mialy wartosc max szarosci
  natomiast piksele ponizej beda biale. Wartosc progu powinna byc wieksza od zera.*/
int polprogowanie_bieli(int tab[][OBRAZ], int wym_x, int wym_y, int max_szarosc, int PROG){
  int i,j;
  for (i=0; i<wym_y; i++){
    for (j=0; j<wym_x; j++){
      if(tab[i][j] <= PROG){
	tab[i][j] = tab[i][j];
      }
      else
	tab[i][j] = max_szarosc;
    }
  }
}
/*Funkcja wykonujaca konturowanie. Funkcja ta rozjasnia piksele ktore roznia sie od otoczenia
  natomiast przyciemnia te ktore sa podobne do niego*/
int konturowanie(int tab[][OBRAZ], int wym_x, int wym_y){
  int i,j; 
  for (i=0; i<wym_y; i++){
    for (j=0; j<wym_x; j++){
      tab[i][j]=abs(tab[i+1][j]-tab[i][j])+abs(tab[i][j+1]-tab[i][j]);
    }
  }
}
/*Funkcja wykonujaca rozciaganie histogramu. Wykonuje sie ja gdy jasnosci pikseli obrazu nie 
pokrywaja calego zakresu szarosci obrazu. Rozciagniecie prowadzi do takiej konwersji zakresu wartosci,
aby jasnoci pikseli obejmowaly caly zakres jasnosci dostepny w obrazie. */
int rozciaganie_histogramu(int tab[][OBRAZ], int wym_x, int wym_y, int max_szarosc, int max ,int min){
  int i,j; 
  min=max_szarosc;
  for (i=0; i<wym_y; i++){
    for (j=0; j<wym_x; j++){
      if(tab[i][j]>max){
	max=tab[i][j];
      }
      if(tab[i][j]<min){
	min=tab[i][j];
      }
    }    
  }
  for (i=0; i<wym_y; i++){
    for (j=0; j<wym_x; j++){
      tab[i][j]=(tab[i][j]-min)*(max_szarosc/(max-min));
    }
  }
}
/*Funkcja wykonujaca korekte gamma. Jest to operacja przeskalowujaca wartosci jasnosci pikseli obrazu.
  Wymagane jest podanie przez uzytkownika parametru gamma. Parametr powinien byc dodatni. Wartosci
  miedzy 0 a 1 przyciemniaja obraz natomiast wartosci powyzej 1 (wlacznie z 1) rozjasniaja obraz.  */
int korekta_gamma(int tab[][OBRAZ], int wym_x, int wym_y, int max_szarosc, float gamma){
  int i,j;
  for (i=0; i<wym_y; i++){
    for (j=0; j<wym_x; j++){
      tab[i][j] = (pow(tab[i][j],1/gamma))/(pow(max_szarosc,(1-gamma)/gamma));
    }
  }
}
/*Funkcja wyswietlajaca zapisany wczesniej obraz za pomoca programu display*/
void wyswietl (char *nazwa){
  /* Wyswietlenie obrazu o zadanej nazwie za pomoca programu "display" */
  char polecenie[1024];
  strcpy(polecenie,"display ");
  strcat(polecenie,nazwa);
  strcat(polecenie," &");
  printf("%s\n",polecenie);
  system(polecenie);
}

int main(){
  int i,j;
  char opcja[2];
  int tab[OBRAZ][OBRAZ];
  FILE *plik;
  char nazwa[100];
  int wym_x,wym_y,max_szarosc;
  int odczytano = 0;
  FILE *plik_zapis;
  char nazwa_zapis[100];
  int BIEL,CZERN,PROG;
  int max = 0,min;
  int proc;
  float gamma;
  int koniec,wroc;

  do{
    printf("Opcje menu:\n");                      /*MENU GLOWNE*/
    printf("\t 1 - Wczytaj plik\n");
    printf("\t 2 - Zapisz obraz\n");
    printf("\t 3 - Funkcje\n");
    printf("\t 4 - Pokaz zapisany wczesniej obraz (Display)\n");
    printf("\t 5 - Zakoncz dzialanie\n");
    printf("\t Twoj wybor:");
      
    scanf("%s",opcja);      
    switch (opcja[0]){
      
    case '1':  { 
      printf("Podaj nazwe pliku do odczytu:\n");
      scanf("%s",nazwa);
      plik=fopen(nazwa,"r");
      
      if (plik != NULL){         /* co spowoduje zakomentowanie tego warunku */
	odczytano = czytaj (plik,tab,&wym_x,&wym_y,&max_szarosc);    /*wczytanie obrazu*/
	koniec = FALSZ;     /*uruchomienie "przelacznika" do dalszych funkcji*/
      }
      else 
	printf("Nie odczytano pliku o tej nazwie\n");
      fclose(plik);      
      break;
    }

    case '2':  {
      if(koniec==FALSZ){            /*dziala tylko gdy wczytano obraz*/
      printf("Podaj nazwe pliku do zapisu: \n");
      scanf("%s",nazwa_zapis);
      plik_zapis=fopen(nazwa_zapis,"w");
      if (plik_zapis != NULL){
	zapisz(max_szarosc,wym_y,wym_x,tab,plik_zapis);
	printf("Zapisano plik\n");
      }
      fclose(plik_zapis);
      }
      else  {
	printf("Blad: Najpierw wczytaj obraz\n");
      }
      break;
    }

    case '3':  {
      if(koniec==FALSZ){    /*dziala tylko gdy wczytano obraz*//*MENU FUNKCJI*/
      do{
	printf("Menu funkcji:\n");
	printf("\t 1 - Negatyw\n");
	printf("\t 2 - Rozmycie\n");
	printf("\t 3 - Zmiana Poziomow\n");
	printf("\t 4 - Progowanie\n");
	printf("\t 5 - Konturowanie\n");
	printf("\t 6 - Rozciaganie histogramu\n");
	printf("\t 7 - Korekta Gamma\n");
	printf("\t 8 - Wypisz\n");
	printf("\t 9 - Wroc do poprzedniego menu\n");
	printf("\t Twoj wybor: ");
	scanf("%s",opcja);
	
	switch(opcja[0]){
	  
	case '1':  { 
	  zmien_znak (tab,wym_x,wym_y,max_szarosc);   /*funkcja negatyw*/
	  printf("Wykonano negatyw\n");
	  break;
	}
	case '2':  {
	  do{
	    printf("Rozmycia:\n");               /*MENU ROZMYCIA*/
	    printf("\t 1 - Rozmycie poziome\n");	
	    printf("\t 2 - Rozmycie pionowe\n");
	    printf("\t 3 - Wroc do poprzedniego menu\n");
	    printf("\t Twoj wybor: ");
	    scanf("%s",opcja);
	    
	    switch(opcja[0]){
	      
	    case '1':  {
	      rozmycie_poziome (tab,wym_x,wym_y);   /*Funkcja rozmycia poziomego*/
	      break;
	    }
	    case '2':  {
	      rozmycie_pionowe (tab,wym_x,wym_y);    /*Funkcja rozmycia pionowego*/
	      break;
	    }
	    case '3':  {
	      wroc=MENU;          /*uruchomienie "przelacznika" powrotu do poprzedniego menu*/
	      break;
	    }
	    default:  {printf("Nieznana opcja\n"); break;}
	    }
	  }while(wroc != MENU);          /*wyjscia z menu rozmycia*/
	  wroc=0;           /*zerowanie "przelacznika powrotu do poprzedniego menu aby byl gotowy do ponownego uzycia*/
	  break;	
	}
	  
	case '3':  {
	  licz_procenty (tab,wym_x,wym_y,max_szarosc,proc);  /*funkcja wyliczajaca procenty z max szarosci*/
	  printf("\nPodaj wartosc bieli: ");
	  scanf("%d",&BIEL);
	  printf("Podaj wartosc czerni: ");
	  scanf("%d",&CZERN);
	  if(BIEL<0 || CZERN<0){
	    printf("Blad: Wartosc bieli lub czerni mniejsza od zera\n");
	  }
	  else{
	    zmiana_poziomow(tab,wym_x,wym_y,max_szarosc,CZERN,BIEL);   /*funkcja zmiany poziomow*/
	    printf("Wykonano zmiane poziomow\n");
	  }
	  break;
	}
	  
	case '4':  {
	  do{
	    printf("Progowania:\n");                     /*MENU PROGOWANIA*/
	    printf("\t 1 - Progowanie zwykle\n");	
	    printf("\t 2 - Polprogowanie bieli\n");
	    printf("\t 3 - Polprogowanie czerni\n");
	    printf("\t 4 - Wroc do poprzedniego menu\n");
	    printf("\t Twoj wybor: ");
	    scanf("%s",opcja);
	    
	    switch(opcja[0]){
	      
	    case '1':  {      
	      licz_procenty (tab,wym_x,wym_y,max_szarosc,proc);  /*funkcja wyliczajaca procenty z max szarosci*/
	      printf("\nPodaj wartosc progu: ");
	      scanf("%d",&PROG);
	      if(PROG<0){
		printf("Blad: PROG mniejszy od zera\n");
	      }
	      else{
		progowanie(tab,wym_x,wym_y,max_szarosc,PROG);  /*funkcja progowania*/
		printf("Wykonano progowanie\n");
	      }
	      break;
	    }
	      
	    case '2':   {      
	      licz_procenty (tab,wym_x,wym_y,max_szarosc,proc);  /*funkcja wyliczajaca procenty z max szarosci*/
	      printf("\nPodaj wartosc progu: ");
	      scanf("%d",&PROG);
	      if(PROG<0){
		printf("Blad: PROG mniejszy od zera\n");
	      }
	      else{
		polprogowanie_bieli(tab,wym_x,wym_y,max_szarosc,PROG);   /*funkcja polprogowania bieli*/
		printf("Wykonano polprogowanie bieli\n");
	      }
	      break;
	    }  
	      
	    case '3':  {      
	      licz_procenty (tab,wym_x,wym_y,max_szarosc,proc);   /*funkcja wyliczajaca procenty z max szarosci*/
	      printf("\nPodaj wartosc progu: ");
	      scanf("%d",&PROG);
	      if(PROG<0){
		printf("Blad: PROG mniejszy od zera\n");
	      }
	      else{
		polprogowanie_czerni(tab,wym_x,wym_y,PROG);   /*funkcja polprogowania czerni*/
		printf("Wykonano polprogowanie czerni\n");
	      }
	      break;
	    }
	      
	    case '4':  { 
	      wroc=MENU;         /*uruchomienie "przelacznika" powrotu do poprzedniego menu*/
	      break;
	    }
	      
	    default:  printf("Nieznana opcja\n"); break;
	    }
	  }while(wroc != MENU);   /*wyjscie z menu progowania*/
	  wroc=0;
	  break;
	}
	  
	case '5':  {      
	  konturowanie(tab,wym_x,wym_y);   /*funkcja konturowania*/
	  printf("Wykonano konturowanie\n");
	  break;
	}
	  
	case '6':  {
	  rozciaganie_histogramu(tab,wym_x,wym_y,max_szarosc,max,min);   /*funkcja rozciagania histogramu*/
	  printf("Wykonano rozciaganie histogramu\n");
	  break;
	}
	  
	case '7':  {
	  printf("Podaj parametr gamma: ");
	  scanf("%f",&gamma);
	  if(gamma<0){
	    printf("Blad: Gamma mniejsza od zera\n");
	  }
	  else if(gamma==0){
	    printf("Blad: Gamma rowna zero\n");
	  }
	  else{
	    korekta_gamma(tab,wym_x,wym_y,max_szarosc,gamma);   /*funkcja korekty gamma*/
	    printf("Wykonano korekte gamma\n");
	  }
	  break;
	}
	  
	case '8':  wypisz (tab,wym_x,wym_y); break;
	  
	case '9':  {
	  wroc=MENU;           /*uruchomienie "przelacznika" powrotu do poprzedniego menu*/
	  break;
	}	  
	default:  printf("Nieznana opcja\n"); break;
	}
      }while(wroc != MENU);             /*wyjscie z menu funkcji*/
      wroc=0;
      }
      else  {
	printf("Blad: Najpierw wczytaj obraz\n");
      }
      break;
    }

    case '4':  {
      if(koniec==FALSZ){
      plik=fopen(nazwa,"r");      
      if (odczytano !=0)
	wyswietl(nazwa);  /*funkcja wyswietlajaca obraz*/
      fclose(plik);
      return odczytano;
      }
      else {
	printf("Blad: Najpierw wczytaj obraz\n");
      }
      break;
    }
      
    case '5':  {
      printf("Koniec dzialania programu\n"); 
      koniec = PRAWDA;
      break;
    }
    default:  printf("Nieznana opcja\n"); break;
    }
  }while(koniec != PRAWDA);     /*wyjscie z glownego menu*/
  return 0;
}
