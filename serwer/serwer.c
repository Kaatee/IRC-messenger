#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <ctype.h>
#include <time.h>
#include <fcntl.h>


struct sockaddr_in addr;
socklen_t rozmiar;

char buffor[125];
char odp[100];

struct cln {
    int cfd;
    struct sockaddr_in caddr;
};



typedef struct uzytkownik{
    char nick[15];
    int nfd;
}uzytkownik;

typedef struct pokoj{
    char nazwa[15];
}pokoj;


uzytkownik *tabUzytkownikow;// = malloc(20*sizeof(uzytkownik));
//memset(tabUzytkownikow,0,(20*sizeof(uzytkownik)));

int zamienNaLiczbe(int poczatekLiczby, char *buffor){
    
    int setki=buffor[poczatekLiczby]-'0';
    poczatekLiczby++;
    int dziesiatki=buffor[poczatekLiczby]-'0';
    poczatekLiczby++;
    int jednosci=buffor[poczatekLiczby]-'0';
    int liczba=100*setki+10*dziesiatki+jednosci;
    
    return liczba;
}

char* pobierzDane(int rozmiar, char buffor1[],int poczatek){
            
    char *dane = malloc (sizeof (char) * (rozmiar+1));
    //memset(dane,0,(sizeof(char)*rozmiar));
   // printf("\nRozmiar danych:%d.",rozmiar);
    //odczytywanie nazwy
    int n=0;
    int i;
    int fordo=poczatek+rozmiar;
    memset(dane,0,(sizeof(char)* (rozmiar+1)));
    for(i=poczatek;i<fordo;i++){
       // printf("\n%d %d. %c.",n,i,buffor1[i]);
        dane[n]=buffor1[i];
        n++;
    }
    dane[n]='\0';

   printf("\nDane to:%s.\n",dane);
    
    return dane;
}


void wyslijZawartoscPliku(char *nazwaPliku ,struct cln* b,int ktoryCase){
     
    char tablicaDoWyslania[322];
    tablicaDoWyslania[0]=ktoryCase+'0';
    tablicaDoWyslania[1]='\t';
    FILE *plik=fopen(nazwaPliku,"rw");
                
    int i=2; //ilosc znakow w pliku
    int znak;
    
    do{
        znak=getc(plik);
        tablicaDoWyslania[i]=znak;
        i++;
    }
    while(znak!=EOF);
    tablicaDoWyslania[i-1]='\n';
    fclose(plik);
    
    //wyslanie odczytanej zawartosci pliku do uzytkownika
    write(b->cfd,&tablicaDoWyslania,i);
    
    printf("\nWyslalem tablice:%s.",tablicaDoWyslania);
    
}


void* cthread (void* arg) {
    printf("\nJESTEM W WATKU");
    
    struct cln* c = (struct cln*)arg;
    
    //tablica aktualnie zalogowanych uzytkownikow
    //uzytkownik *tabUzytkownikow = malloc(20*sizeof(uzytkownik));
    //memset(tabUzytkownikow,0,(20*sizeof(uzytkownik)));
    
    
    printf("\nPolaczylem sie z:%s.\n", inet_ntoa((struct in_addr)c->caddr.sin_addr));
    int i,j;
    char pomCHAR;
    FILE *plik;
    int plik3;
    
    
    while(1){
        memset(buffor,0,sizeof(buffor));
        i=0;
        do{
            read(c->cfd,&pomCHAR,1);
            buffor[i]=pomCHAR;
           // printf("\nPOM:%c.\n",pomCHAR);
            //sleep(3);
            i++;
        }
        while(i<125);
        int wybor=buffor[0]-'0';
       
        printf("\nWybrales opcje:%d.\n",wybor);
        
        if(wybor<0) {
            exit(0);
            printf("Jestem mniejszy od zera");
        }
        
        
        switch(wybor){
            //------------------------------------------------------------------------------------------------------------------------------------------------
            //                                                      L O G O W A N I E
            //------------------------------------------------------------------------------------------------------------------------------------------------
            case 1:{
                printf("\n\n\nCASE 1");
                
                int rozmiarNicka=zamienNaLiczbe(1,buffor);
                
                char * nick = malloc (sizeof (char) * rozmiarNicka);
                nick = pobierzDane(rozmiarNicka,buffor,4);
                printf("\nnick to:%s.",nick);

                
                //odczytywanie obecnych uzytkownikow z pliku
                plik=fopen("uzytkownicy.txt","rw");
                
                
                uzytkownik *tabUzytkownikowWPliku = malloc(20*sizeof(uzytkownik));
                //oczytywanie z pliku znak po znaku do tablicy uzytkownikow
                int znak;
                i=0;
                j=0;
                do{
                    znak=getc(plik);
                    if(znak!=9){ //\t =
                        tabUzytkownikowWPliku[i].nick[j]=znak;
                        tabUzytkownikowWPliku[i].nfd=c->cfd;
                        j++;
                    }
                    else{
                        i++;
                        j=0;
                    }
                }
                while(znak!=EOF);
                
                //liczenie czy w tablicy zalogowanych sa uzytkownicy o tej samej nazwie
                int ile=0;
                for(i=0;i<20;i++){
                    if(strncmp(tabUzytkownikow[i].nick,nick,rozmiarNicka)==0){
                        ile++;
                        break;
                    }
                }
                
                
                //liczenie czy w tablicy wszystkich uzytkownikow sa uzytkownicy o tej samej nazwie
                int ileWPliku=0;
                for(i=0;i<20;i++){
                    if(strncmp(tabUzytkownikowWPliku[i].nick,nick,rozmiarNicka)==0){
                        ileWPliku++;
                        break;
                    }
                }
                
                char udaloSieZalogowac[3]="11\n";
                char juzZalogowany[3]="12\n";
                
                //wczytywanie nowej nazwy do pliku
                plik3=open("uzytkownicy.txt", O_WRONLY|O_APPEND);
               
                //znajdowanie indeksu pierwszego wolnego miejsca w tab uzyta
                int indeks=0;
                for(i=0;i<20;i++){
                    if(tabUzytkownikow[i].nick[0]==0)
                       break;
                }
                indeks=i;
                
                printf("\nIle w pliku:%d",ileWPliku);
                printf("\nIle:%d",ileWPliku);
                
                if(ileWPliku==0){
                    write(plik3,nick,rozmiarNicka);
                    write(plik3,"\t",1);
                    printf("\nZapisalem do pliku");
                    write(c->cfd,udaloSieZalogowac,3);
                    strcpy(tabUzytkownikow[indeks].nick,nick);
                    tabUzytkownikow[indeks].nfd=c->cfd;
                    printf("\nCFD UZYTKOWNIKA:%d\n",tabUzytkownikow[indeks].nfd); 
                }
                if(ileWPliku>0 && ile==0){
                    printf("\nNazwa istnieje, ale nie jest zalogowany, loguje");
                    strcpy(tabUzytkownikow[indeks].nick,nick);
                    tabUzytkownikow[indeks].nfd=c->cfd;
                    write(c->cfd,udaloSieZalogowac,3);
                    printf("\nCFD UZYTKOWNIKA:%d\n",tabUzytkownikow[indeks].nfd); 
                }
                
                if(ileWPliku>0 && ile>0){
                    printf("\nUzytkownik o podanej nazwie jest juz zalogowany");
                    write(c->cfd,juzZalogowany,3);
                    close(c->cfd);
                    goto EndWhile;
                }
                
                //pomocnicze wyswietlenie tablicy uzytkownikow
                for(i=0;i<20;i++){
                    printf("X:%s.",tabUzytkownikow[i].nick);
                }
                memset(buffor,0,sizeof(buffor));
                fclose(plik);
                close(plik3);
                printf("\nZakonczylem case 1");
                break;
                
        }
                
                
              
                
            //------------------------------------------------------------------------------------------------------------------------------------------------
            //                           W Y S Y L A N I E      L I S T Y     Z A L O G O W A N Y C H     U Z Y T K O W N I K O W 
            //------------------------------------------------------------------------------------------------------------------------------------------------
            case 2:{
                printf("\n\n\nCASE 2");
                wyslijZawartoscPliku("uzytkownicy.txt",c,2);
                printf("\nZakonczylem case 2");
                break;                
            }
            //------------------------------------------------------------------------------------------------------------------------------------------------
            //                                      W Y S Y L A N I E      L I S T Y    P O K O I
            //------------------------------------------------------------------------------------------------------------------------------------------------
            case 3:{
                printf("\n\n\nCASE 3");
                wyslijZawartoscPliku("pokoje.txt",c,3);
                printf("\nZakonczylem case 2");
                break;                
            }
                
            //------------------------------------------------------------------------------------------------------------------------------------------------
            //                    W Y S Y L A N I E      L I S T Y    U Z Y T K O W N I K O W      W      D A N Y M     P O  K O J U  
            //------------------------------------------------------------------------------------------------------------------------------------------------
            case 4:{
                printf("\n\n\nCASE 4");
                int rozmiarPokoju=zamienNaLiczbe(1,buffor);
                
                char * nazwaPokoju = malloc (sizeof (char) * rozmiarPokoju);
                nazwaPokoju = pobierzDane(rozmiarPokoju,buffor,4);
                
                printf("\nNazwa to:%s.\n",nazwaPokoju);
                
                wyslijZawartoscPliku(nazwaPokoju,c,4);
                
                printf("\nZakonczylem case 4");
                break;
                
            }
                
                
                 
            //------------------------------------------------------------------------------------------------------------------------------------------------
            //                                                  N O W A     G R U P A
            //------------------------------------------------------------------------------------------------------------------------------------------------
            case 5:{
                printf("\n\n\nCASE 5");
                
                int rozmiarNazwy=zamienNaLiczbe(1,buffor);
                char * nazwa = malloc (sizeof (char) * rozmiarNazwy);
                nazwa = pobierzDane(rozmiarNazwy,buffor,4);
                printf("\nNazwa to:%s.\n",nazwa);
                
                
                //odczyt nicku admina
                int polozenieRozmiaru=4+rozmiarNazwy;
                int rozmiarAdmina=zamienNaLiczbe(polozenieRozmiaru,buffor);
                
             
                int skadOdczytywac=polozenieRozmiaru+3;
                char * nickAdmina = malloc (sizeof (char) * rozmiarAdmina);
                nickAdmina = pobierzDane(rozmiarAdmina,buffor,skadOdczytywac);
                printf("\nNick admina to:%s.",nickAdmina);
                
                
                
                //odczytywanie nazwy pokoju bez .txt
                int polozenieCzystejNazwy=skadOdczytywac+rozmiarAdmina;
                int rozmiarCzystejNazwy=zamienNaLiczbe(polozenieCzystejNazwy,buffor);
                
                skadOdczytywac=polozenieCzystejNazwy+3;
                char * czystaNazwa = malloc (sizeof (char) * rozmiarCzystejNazwy);
                czystaNazwa = pobierzDane(rozmiarCzystejNazwy,buffor,skadOdczytywac);
                printf("\nCzysta nazwa pokoju to:%s.",czystaNazwa);
                
                //zapis do pliku nazwy pokoju
                plik3=open("pokoje.txt", O_WRONLY|O_APPEND);
                write(plik3,czystaNazwa,rozmiarCzystejNazwy);
                write(plik3,"\t",1);
                close(plik3);
                
                //odczyt listy uzytkownikow
                polozenieRozmiaru=skadOdczytywac+rozmiarCzystejNazwy;
                skadOdczytywac=polozenieRozmiaru+3;
                
                int rozmiarListy=zamienNaLiczbe(polozenieRozmiaru,buffor);
                char * lista = malloc (sizeof (char) * rozmiarListy);
                lista = pobierzDane(rozmiarListy,buffor,skadOdczytywac);
                printf("\nLista uzytkownikow to:%s.",lista);
                
                //sleep(10);
                //utworzenie i zapis do pliku
                plik=fopen(nazwa,"aw+");  
                fclose(plik);
                int nowyPlik=open(nazwa,O_WRONLY|O_APPEND);
                write(nowyPlik,nickAdmina,rozmiarAdmina);
                write(nowyPlik,"\t",1);
                write(nowyPlik,lista,rozmiarListy);
                close(nowyPlik);
                
                //wysylanie potwierdzenia utworzenia
                char potwierdzenie[4]="5\t1\n";
                write(c->cfd,potwierdzenie,4);
                              
                printf("\nZakonczylem case 5");
                
              //  fclose(plik);   TU ZAKOMENTOWALAM 
                break;
                
            }
            
                
                
            //------------------------------------------------------------------------------------------------------------------------------------------------
            //                               W Y S Y L A N I E      W I A D O M O S C I     D O     P O K O J U
            //------------------------------------------------------------------------------------------------------------------------------------------------
            case 6:{
              //  printf("\n\n\nCASE 6");
                
                //odczyt nicku nadawcy
                int rozmiarNickuNadawcy=zamienNaLiczbe(1,buffor);
                
                //char nickNadawcy[rozmiarNickuNadawcy];
                //strncpy(nickNadawcy,pobierzDane(rozmiarNickuNadawcy,buffor,4),rozmiarNickuNadawcy);
                char * nickNadawcy = malloc (sizeof (char) * rozmiarNickuNadawcy);
                nickNadawcy = pobierzDane(rozmiarNickuNadawcy,buffor,4);
                
              // printf("\nNick nadawcy to:%s.\n",nickNadawcy);
                
                
                //odczyt nazwy pokoju
                int x=4+rozmiarNickuNadawcy;
                int rozmiarPokoju=zamienNaLiczbe(x,buffor);
      
                
                int pom=4+rozmiarNickuNadawcy+3;
                //strncpy(nazwaPokoju,pobierzDane(rozmiarPokoju,buffor,pom),rozmiarPokoju);
                char * nazwaPokoju = malloc (sizeof (char) * rozmiarPokoju);
                nazwaPokoju = pobierzDane(rozmiarPokoju,buffor,pom);
               
            //    printf("\nNazwa Pokojuto:%s.",nazwaPokoju);
                
                 pom=4+rozmiarPokoju+3+rozmiarNickuNadawcy;
                //odczyt tresci wiadomosci
                int rozmiarWiadomosci=zamienNaLiczbe(pom,buffor);
                //char wiadomosc[rozmiarWiadomosci];
                
                pom=pom+3;
                //strncpy(nazwaPokoju,pobierzDane(rozmiarPokoju,buffor,pom),rozmiarPokoju);
                char * wiadomosc = malloc (sizeof (char) * rozmiarWiadomosci);
                printf("\nRozmiar wiadomosci:%d.\n",rozmiarWiadomosci);
                wiadomosc = pobierzDane(rozmiarWiadomosci,buffor,pom);
                
               // printf("\nTresc wiadomosci to:%s.\n",wiadomosc);
                  
                //odczytywanie z pliku nickow osob znajdujacych sie w pokoju
                FILE *plik=fopen(nazwaPokoju,"rw");
                uzytkownik *tabUzytkownikowwPokoju = malloc(20*sizeof(uzytkownik));
                memset(tabUzytkownikowwPokoju,0,20*sizeof(uzytkownik));
                
                int znak;
                i=0;
                j=0;
                do{
                    znak=getc(plik);
                    //printf("To jest znak : %d",znak);
                     if(znak<0) break;
                    if(znak!=9 && znak!=13 && znak!=10){ //\t =
                        tabUzytkownikowwPokoju[i].nick[j]=znak;
                        j++;
                    }
                    else{
                        i++;
                        j=0;
                    }
                }
                while(znak!=EOF && znak>0);
                fclose(plik);
                
                //przygotowanie wiadomosci do tablicaDoWyslania
                char * gotowaWiadomosc = malloc (sizeof (char) * (1+1+rozmiarNickuNadawcy+1+rozmiarPokoju+1+rozmiarWiadomosci+1+1));
                int skip = 0;
                gotowaWiadomosc[0]='6';
                gotowaWiadomosc[1]='\t';
                skip += 2;
                for(i=0;i<rozmiarNickuNadawcy;i++){
                    gotowaWiadomosc[skip+i]=nickNadawcy[i];
                }
                skip +=rozmiarNickuNadawcy;
                gotowaWiadomosc[skip]='\t';
                skip+=1;
                
                for(i=0;i<rozmiarPokoju;i++){
                    gotowaWiadomosc[skip+i]=nazwaPokoju[i];
                }
                skip +=rozmiarPokoju;
                gotowaWiadomosc[skip]='\t';
                skip+=1;
                
                for(i=0;i<rozmiarWiadomosci;i++){
                    gotowaWiadomosc[skip+i]=wiadomosc[i];
                }
                skip +=rozmiarWiadomosci;
                gotowaWiadomosc[skip]='\n';
                skip+=1;
                gotowaWiadomosc[skip]='\0';
              //  skip+=1;
                     
               printf("\nWiadomosc:%s.\n",gotowaWiadomosc);
                
                //int ileWyslac=sizeof (char) * (1+1+rozmiarNickuNadawcy+1+rozmiarPokoju+1+rozmiarWiadomosci+1+1);
                
                for(i=0;i<20;i++){
                    if(strcmp(tabUzytkownikowwPokoju[i].nick,nickNadawcy)!=0){
                      //  printf("\nJESTEM TUUU\n");
                        printf("\nNICK TAB NADAWCY:%s. NICK:%s.\n",tabUzytkownikowwPokoju[i].nick,nickNadawcy);
                        for(j=0;j<20;j++){
                            if(strncmp(tabUzytkownikow[j].nick,tabUzytkownikowwPokoju[i].nick,15)==0 && tabUzytkownikow[j].nfd>0){
                                printf("\n .%d. .%s. \n",tabUzytkownikow[j].nfd,tabUzytkownikow[j].nick);
                                write(tabUzytkownikow[j].nfd,gotowaWiadomosc,skip);
                                // write(tabUzytkownikow[j].nfd,buffor,skip);
                                printf("\nWIADOMOSC:\n");
                                write(1,gotowaWiadomosc,skip);
                                printf(".\n");
                                //printf("CFD GLOWNEGO:%d");
                            }
                        }
                    }
                    else{
                        write(c->cfd,"6\t1\n",4);
                        printf("\nJESTEM W ELSE\n");
                    }
                }

                
               printf("\nZakonczylem case 6");
                break;
            }
        
            //------------------------------------------------------------------------------------------------------------------------------------------------
            //                                                     W Y L O G O W A N I E
            //------------------------------------------------------------------------------------------------------------------------------------------------
            case 7:{
                int rozmiarNicka=zamienNaLiczbe(1,buffor);
                
                char * nick = malloc (sizeof (char) * rozmiarNicka);
                nick = pobierzDane(rozmiarNicka,buffor,4);
                printf("\nnick do usuniecia to:%s.",nick);
                
                for(i=0;i<20;i++){
                    if(strncmp(tabUzytkownikow[i].nick,nick,15)==0 && tabUzytkownikow[i].nick[0]!=0){
                        tabUzytkownikow[i].nfd=0;
                        for(j=0;j<15;j++){
                            tabUzytkownikow[i].nick[j]='\0';
                        }
                        
                    }
                }
               close(c->cfd);
               goto EndWhile;
               printf("\nWYLOGOWALEM");
               break;
            }
            
            //------------------------------------------------------------------------------------------------------------------------------------------------
            //                                                       U S U W A N I E    G R U P Y
            //------------------------------------------------------------------------------------------------------------------------------------------------
            case 8:{
                int rozmiarNazwy=zamienNaLiczbe(1,buffor);
                char * nazwatxt = malloc (sizeof (char) * rozmiarNazwy);
                nazwatxt = pobierzDane(rozmiarNazwy,buffor,4);
                printf("\nNazwa pokoju do usuniecia to:%s.\n",nazwatxt);
                
                
                //odczyt nicku admina
                int polozenieRozmiaru=4+rozmiarNazwy;
                int rozmiarUzytkownika=zamienNaLiczbe(polozenieRozmiaru,buffor);
                
             
                int skadOdczytywac=polozenieRozmiaru+3;
                char * nickUzytkownika = malloc (sizeof (char) * rozmiarUzytkownika);
                nickUzytkownika = pobierzDane(rozmiarUzytkownika,buffor,skadOdczytywac);
                printf("\nNick uzytkownika:%s.",nickUzytkownika);
                
                
                //odczytywanie nazwy pokoju bez .txt
                int polozenieRozmiaruCzystejNazwy=skadOdczytywac+rozmiarUzytkownika;
                int rozmiarCzystejNazwy=zamienNaLiczbe(polozenieRozmiaruCzystejNazwy,buffor);
                printf("\nPolozenie rozmiaru czystej nazwy:%d.",polozenieRozmiaruCzystejNazwy);
                printf("\nRozmiar czystej nazwy:%d.",rozmiarCzystejNazwy);
                
                skadOdczytywac=polozenieRozmiaruCzystejNazwy+3;
                printf("\nSkad oczytywac czyata nazwe:%d.",skadOdczytywac);
                char * czystaNazwa = malloc (sizeof (char) * rozmiarCzystejNazwy);
                
                czystaNazwa = pobierzDane(rozmiarCzystejNazwy,buffor,skadOdczytywac);
                
                printf("\nCzysta nazwa pokoju to:%s.\n",czystaNazwa);
                
                
                pokoj *tablicaPokoi = malloc(20*sizeof(pokoj));
                memset(tablicaPokoi,0,(20*sizeof(pokoj)));
                //oczytywanie z pliku znak po znaku do tablicy uzytkownikow
                int znak;
                i=0;
                j=0;
                
                plik=fopen("pokoje.txt","aw+");
                
                do{
                    znak=getc(plik);
                    if(znak<0) break;
                    printf("\nTo jest znak : .%d.\n",znak);
                    if(znak!=9){ //\t =
                        tablicaPokoi[i].nazwa[j]=znak;
                        j++;
                    }
                    else{
                        i++;
                        j=0;
                    }
                }
                while(znak!=EOF && znak>=0);
                
                
                
                for(i=0;i<20;i++){
                    printf("\nTablica pokoi od %d =%c, d=%d.\n",i,tablicaPokoi[i].nazwa[0], tablicaPokoi[i].nazwa[0]);
                }
                int czymoge=0;
                char nowePokoje[320];
                
                
                //zczytywanie pierwszego uzytkownika w danym pokoju (admina)
                plik=fopen(nazwatxt,"aw+");
                i=0;
                
                char adminPokoju[15];
                
                do{
                    znak=getc(plik);
                    if(znak!=9){
                        adminPokoju[i]=znak;
                        i++;
                    }
                    else
                        adminPokoju[i]='\0';
                }
                while(znak!=9);
                
                
                fclose(plik);
                int ileWpisac=0;
               //sprawdzanie czy mozna usunac plik i przygotowanie nowego
                if(strncmp(adminPokoju,nickUzytkownika,15)!=0){
                    printf("\nNie moge usunac pokoju bo nie jestes adminem");
                    char nieJestesAdminem[4]="8\t2\n";
                    write(c->cfd,nieJestesAdminem,4);
                    ileWpisac=0;
                }
                else{
                    printf("\nJestem adminem\n");
                    czymoge=1;
                    
                    //usuwanie nazwe z pliku pokoje.txt
                    int n=0;
                    for(i=0;i<20;i++){
                        if(tablicaPokoi[i].nazwa[0]=='\0') break;
                        if(strncmp(tablicaPokoi[i].nazwa,czystaNazwa,15)==0) {
                            continue;
                        }
                        else{
                            for(j=0;j<15;j++){
                                if(tablicaPokoi[i].nazwa[j]!='\0'){
                                    nowePokoje[n]=tablicaPokoi[i].nazwa[j];
                                    n++;
                                }
                                else{
                                    printf("\nJESTEM W ELSE\n");
                                    nowePokoje[n]='\t';
                                    n++;
                                    break;
                                }
                            }
                        }
                    }
                    ileWpisac=n;
                    //nowePokoje[n]='\0';
                    //nowePokoje[n+1]='\0';
                    
                }
                
                printf("\nTABLICA NOWYCH POKOI:%s.\n",nowePokoje);
                
                
                if(czymoge==1){
                    
                    if(remove(nazwatxt)==0)
                        printf("\nUsunieto pomyslnie plik nazwa.txt");
                    else
                        printf("\nNie udalo sie skasowac pliku.");
                        
                        
                    if(remove("pokoje.txt")==0){
                        printf("\nUsunieto pomyslnie plik pokoje");
                    }
                    else
                        printf("\nNie udalo sie skasowac pliku.");
                        
                 
                     //utworzenie i zapis do pliku
                    plik=fopen("pokoje.txt","aw+");  
                    fclose(plik);
                    int nowyPlik=open("pokoje.txt",O_WRONLY|O_APPEND);
                    write(nowyPlik,nowePokoje,ileWpisac);
                  //  write(nowyPlik,"\t",1);
                    close(nowyPlik);
                    
                    char udaloSieUsunacGrupe[4]="8\t1\n";
                    write(c->cfd,udaloSieUsunacGrupe,4);
            }
            printf("\nKoniec case 8\n");
            break;
                
            }
            
            
            
            default : {
                printf("\n\nNIE ZNALEZIONO POLECENIA\n");
                goto EndWhile;
                break;
            }
        }
    }
EndWhile: ;

    //sleep(3);
   // write(c->cfd, odp, strlen(odp));
    close(c->cfd);
   // printf("Wyslano do: %s message: %s\n", inet_ntoa((struct in_addr)c->caddr.sin_addr), odp);
    free(c);
    return 0;
}



int main()
{
    
    tabUzytkownikow = malloc(20*sizeof(uzytkownik));
    
    pthread_t tid;
    
    uint16_t port = 1233;
    //char ip[30] = "192.168.0.15";//INADDR_ANY;
    //strcpy(ip,INADDR_ANY);
    addr.sin_family = PF_INET;
    addr.sin_port = htons(port);
    printf("START\n");
    addr.sin_addr.s_addr = INADDR_ANY;
    
    int fd = socket(PF_INET, SOCK_STREAM, 0);
    int on = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
    
    bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(fd, 5);         //fd zawsze ten z funkcji socket, liczba oczekujacych polaczen
    
    while(1)
    {
        printf("\nJESTEM W WHILE");
        struct cln* c = malloc(sizeof(struct cln));
        rozmiar = sizeof(c->caddr);
        c->cfd = accept(fd, (struct sockaddr*)&c->caddr, &rozmiar); // blokujaca, przekazuje IP i adres portu klienta, zwraca deskryptor nowego gniazda - sluzy tylko do komunikacji z tym klientem, gniazdo glowne jest stosowane tylko do akceptacji
        //read/write z new_fd(nwd)
        pthread_create(&tid, NULL, cthread, c);
        pthread_detach(tid);
        
    };
    
    close(fd);
    
}