#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "menu.h"
#include "piece.h"
#include "plateau.h"
#include "calculus.h"
#include "temps.h"
#include <signal.h>
#include <time.h>
#include <pthread.h> // Pour utiliser les threads
#include <unistd.h>

int demander_et_convertir_case();
int choixducoup1(piece tableau[TAILLE][TAILLE], int couleur, int tab[3], int aidealavisee);
int v;
void score(partie *p);
pthread_t timer_thread_team0, timer_thread_team1;

// Variables globales pour le suivi des timers et du contrôle
volatile int timer_running_team0 = 0;
volatile int timer_running_team1 = 0;
volatile int timer_expired = 0;  // Pour indiquer qu'un timer a expiré
volatile int current_team = 0;   // Pour savoir quelle équipe est en train de jouer

// Gestionnaire de signal pour l'expiration du timer
void timer_expired_handler(int signum) {
    timer_expired = 1;
}

void *timer_function(void *arg) {
    int *timer = (int *)arg;
    while (*timer < TEMPS) {
        attendre(1);
        (*timer)++;
        if (*timer >= TEMPS) {
            // Envoyer un signal quand le timer expire
            timer_expired = 1;
            kill(getpid(), SIGUSR1);  // Envoie un signal au processus principal
            pthread_exit(NULL);
        }
    }
    return NULL;
}

void start_timer(int *timer, int team) {
    current_team = team;
    timer_expired = 0;
    if (team == 0) {
        timer_running_team0 = 1;
        pthread_create(&timer_thread_team0, NULL, timer_function, timer);
    } else {
        timer_running_team1 = 1;
        pthread_create(&timer_thread_team1, NULL, timer_function, timer);
    }
}

void stop_timer(int team) {
    if (team == 0) {
        timer_running_team0 = 0;
        pthread_cancel(timer_thread_team0);
    } else {
        timer_running_team1 = 0;
        pthread_cancel(timer_thread_team1);
    }
}

// Nouvelle fonction pour configurer le gestionnaire de signal
void setup_timer_handler() {
    struct sigaction sa;
    sa.sa_handler = timer_expired_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGUSR1, &sa, NULL);
}

void jeu(partie *p){
    int tour = p->aquiletour; 
    setup_timer_handler();  // Configure le gestionnaire de signal
    while(p->etat==0){
        ////////////////// TOUR DE L'HUMAIN / JOUEUR 0
        if(tour==0){
        clearecran();
        affiche(p->plateau,0);
        int tabpositions[3]; 
        printf("Il vous reste %d secondes.  \033[90m%ds pour l'adversaire.\n\033[0m\n",TEMPS-p->tempsteam0,TEMPS-p->tempsteam1);
        start_timer(&p->tempsteam0,0);
        int c1=choixducoup1(p->plateau,0,tabpositions,p->aidealavisee); // la tabposition est mis à jour avec cette ligne
        if (timer_expired || c1 == -99) {
                stop_timer(0);
                p->etat = 2;
                p->gagnant = 1;  // L'autre équipe gagne
                clearecran();
                printf("\n\n\033[1;91m /!\\ Time out /!\\\033[0m\n\n");
                attendre(3);
                break;
            }
        if(c1==-1){break;}
        if(c1==-2){p->etat=2;p->gagnant=1;break;}
        if(c1==-3){p->etat=2;p->gagnant=2;break;}
        p->coupsjoues++;
        deplacement d1 = {tabpositions[0]/10,tabpositions[0]%10};
        deplacement d2 = {tabpositions[1]/10,tabpositions[1]%10};
        applymove(p->plateau,d1,d2,0,&p->scorewhite,&p->scoreblack,&p->mortsnoirs,&p->mortsblancs);
        verifs(p,0,0);
        stop_timer(0);
        tour=1;      
        }

        ////////////////// TOUR DE L'IA / JOUEUR 1
        if(tour==1){ 
            if(p->niveauIA==0){ //si cest du humain contre humain
                start_timer(&p->tempsteam1,1);
                printf("Il vous reste %d secondes.  \033[90m%ds pour l'adversaire.\n\033[0m\n",TEMPS-p->tempsteam1,TEMPS-p->tempsteam0);
                clearecran();   
                affiche(p->plateau,0);
                int tabpositions[3]; 
                int c1=choixducoup1(p->plateau,1,tabpositions,p->aidealavisee);
                if (timer_expired || c1 == -99) {
                    stop_timer(1);
                    p->etat = 2;
                    p->gagnant = 0;  // L'autre équipe gagne
                    clearecran();
                    printf("\n\n\033[1;91m /!\\ Time out /!\\\033[0m\n\n");
                    attendre(3);
                    break;
                }
                if(c1==-1){break;}
                if(c1==-2){p->etat=2;p->gagnant=0;break;}
                if(c1==-3){p->etat=2;p->gagnant=2;break;}
                deplacement d1 = {tabpositions[0]/10,tabpositions[0]%10};
                deplacement d2 = {tabpositions[1]/10,tabpositions[1]%10};
                applymove(p->plateau,d1,d2,0,&p->scorewhite,&p->scoreblack,&p->mortsnoirs,&p->mortsblancs);
                stop_timer(1);
                tour=0;
            }
            if(p->niveauIA==1 || p->niveauIA==2 || p->niveauIA==3){
                p->tempsteam1=p->tempsteam1+rand()%60/((p->niveauIA)); //temps généré artificiellement
                if(p->tempsteam1>TEMPS){p->etat=2;p->gagnant=0;break;} // si l'IA n'a plus de temps!
                printf("\033[31mIA\033[0m en cours de réflexion...\n");
                
                ////// Initialisation
                int roix=0;
                int roiy=0;
                while(p->plateau[roix][roiy].couleur!=tour || p->plateau[roix][roiy].rang!=ROI){roix++;if(roix==TAILLE){roiy++;roix=0;}}
                deplacement roi={roix,roiy};
                int echec=roienechec(p->plateau,roi); //detecte si nous somme actuellement en situation d'échec
                if(echec==1){printf("\033[38;5;214m! Vous êtes en situation d'échec !\033[0m\n");}
                deplacement* listepionjouables=(deplacement*)malloc(sizeof(deplacement));
                listepionjouables->info=-1;
                listedemespions(p->plateau,tour,listepionjouables, p->niveauIA);
                ////// Initialisation


            ////////////////////////////////////////////////////////////////////////////////// ECHEC ET MAT OU PAT
                if(listepionjouables->info==-1){
                    printf("\033[91m/!\\ Plus de movements disponibles /!\\");
                    if(echec==1){printf("\n\033[1;0Echec et mat !\n\033[0m");}
                    else{printf("\n\033[1;0Pat !\n\033[0m");}
                    printf("pour l'équipe %d\n",tour);
                    freelist(listepionjouables);
                    if(echec==1){p->etat=2;p->gagnant=1;}
                    else{p->etat=2;p->gagnant=2;}
                    break;
            ////////////////////////////////////////////////////////////////////////////////// ECHEC ET MAT OU PAT
                }  
                

                //////choix du pion à déplacer
                int sizeofliste;
                sizeofliste=tailleliste(listepionjouables);
                int entier_aleatoire = rand() % sizeofliste;
                if(entier_aleatoire!=0){
                for(int i=0;i<entier_aleatoire;i++){
                    listepionjouables=listepionjouables->next;
                }}
                deplacement start={listepionjouables->x,listepionjouables->y};
                freelist(listepionjouables);
                printf("L'IA a choisi de déplacer le pion en x=%d y=%d\n",TAILLE-start.x,start.y+1);
                //////choix du pion à déplacer

                //////choix du déplacement
                deplacement* listedep=(deplacement*)malloc(sizeof(deplacement));
                listedep=possiblemove(p->plateau,start);
                removeimpossiblemove(listedep,start,p->plateau);                
                if(p->niveauIA==2 || p->niveauIA==3){
                    listedep=sendattackpossibilites(p->plateau,listedep, p->niveauIA);
                }
                sizeofliste=tailleliste(listedep);
                entier_aleatoire = rand() % sizeofliste;
                if(entier_aleatoire!=0){
                for(int i=0;i<entier_aleatoire;i++){
                    listedep=listedep->next;
                }}
                deplacement arrivee={listedep->x,listedep->y};
                freelist(listedep);
                //////choix du déplacement


                if(p->niveauIA==1 || p->niveauIA==3){//timer
                //possibilité de passer le timer pour le vrai joueur
                }


                applymove(p->plateau,start,arrivee,0,&p->scorewhite,&p->scoreblack,&p->mortsnoirs,&p->mortsblancs);
                tour=0;
            }
            p->aquiletour=tour;
            p->coupsjoues++;
            verifs(p,1,0);  
        }
    }
    remplacerOuAjouterPartie(p);
    score(p);
    attendre(5);
    return;
}




int choixducoup1(piece tableau[TAILLE][TAILLE], int couleur, int tab[3],int aidealavisee){
    ////// Initialisation
    int buffer,buffer2;
    int roix=0;
    int roiy=0;
    while(tableau[roix][roiy].couleur!=couleur || tableau[roix][roiy].rang!=ROI){roix++;if(roix==TAILLE){roiy++;roix=0;}}
    deplacement roi={roix,roiy};
    int echec=roienechec(tableau,roi); //detecte si nous somme actuellement en situation d'échec
    ////// Initialisation

    ////// choix du pion à déplacer
    do{
        deplacement* listepionjouables=(deplacement*)malloc(sizeof(deplacement));
        listepionjouables->info=-1;
        listedemespions(tableau,couleur,listepionjouables,0);
        ////////////////////////////////////////////////////////////////////////////////// ECHEC ET MAT OU PAT
        if(listepionjouables->info==-1){
            printf("\033[91m/!\\ Plus de movements disponibles /!\\");
            if(echec==1){printf("\n\033[1;0mEchec et mat !\n\033[0m");}
            else{printf("\n\033[1;0Pat !\n\033[0m");}
            printf("pour l'équipe %d\n",couleur);
            freelist(listepionjouables);
            if(echec==1){return -2;}
            else{return -3;}
        }
        ////////////////////////////////////////////////////////////////////////////////// ECHEC ET MAT OU PAT
        printf("Donnez la case du pion à déplacer !");
        if(couleur==0){printf("\033[34m team bleue :\033[0m\033[90m [-1 : leave the game.]\033[0m\n");}
        else{printf("\033[31m team rouge :\033[0m\n");}
        buffer = demander_et_convertir_case();
        ////////////////////////////////////////////////////////////////////////////////// PAUSE
        if(buffer==-1){freelist(listepionjouables);return-1; //si le joueur demande une pause
        ////////////////////////////////////////////////////////////////////////////////// PAUSE
    }
        ////////////////////////////////////////////////////////////////////////////////// TIMER OVER
        if(buffer==-99){freelist(listepionjouables);return-99;} 
        ////////////////////////////////////////////////////////////////////////////////// TIMER OVER

        ////// choix du déplacement
        if(estdanslaliste(buffer,listepionjouables)){ //si pion est dans la liste des pions deplacable, on continue
            deplacement dep={buffer/10,buffer%10};
            deplacement* liste = possiblemove(tableau,dep);
            removeimpossiblemove(liste, dep, tableau);
            addtolist(dep.x,dep.y,-2,liste,tableau); //on rajoute dep à la liste avec info à -2 UNIQUEMENT pour qu'il apparaisse en orange
            clearecran();
            affiche(tableau,liste);
            do{
                printf("Donnez la case dans laquelle déplacer le pion\033[90m [-1 : return.]\033[0m\n");
                buffer2 = demander_et_convertir_case();
                if(buffer2==-1){freelist(liste);break;}
                ////////////////////////////////////////////////////////////////////////////////// TIMER OVER
                if(buffer2==-99){freelist(listepionjouables);freelist(liste);return-99;} 
                ////////////////////////////////////////////////////////////////////////////////// TIMER OVER
                int infosurlecoup;
                if(estunvraicoup(buffer2, liste,&infosurlecoup)==1){ //on peut pas se retrouver bloquer sans coup car cest verifier juste avant
                    //bon!!
                    tab[0]=buffer;
                    tab[1]=buffer2;
                    tab[2]=infosurlecoup;
                    break;
                }
                else{
                    if(aidealavisee==1){
                        printf("\033[91mMouvement n'existe pas !\n\033[0m");
                        continue;
                    }
                else{
                    //timer -5 !!
                }}
        }while(1);
        if(buffer2==-1){continue;} // on repart au debut on redemande un pion
        freelist(liste);
        break;}
        ////// choix du déplacement
        else{printf("\033[91mPas de mouvements disponibles sur ce pion !\n\033[0m");freelist(listepionjouables);}
    }while(1);
    return 0;
}







int demander_et_convertir_case() {
    char case_str[3];
    int result;
    
    do {
        printf("Entrez une case entre A1 et H8: ");
        fflush(stdout);
        // Utiliser read() au lieu de scanf() pour pouvoir être interrompu
        int n = read(STDIN_FILENO, case_str, 3);
        
        if (timer_expired) {
            return -99;  // Code spécial pour indiquer l'expiration du timer
        }
        
        if (n <= 0) continue;
        
        case_str[n] = '\0';
        
        if(case_str[0]=='-' && case_str[1]=='1' && case_str[2]=='\0'){
            return -1;
        }
        
        char colonne = toupper(case_str[0]);
        if(colonne < 'A' || colonne > 'H') {
            printf("Entrée invalide. La colonne doit être entre A et H.\n");
            continue;
        }
        
        int ligne = case_str[1] - '0';
        if(ligne < 1 || ligne > 8) {
            printf("Entrée invalide. La ligne doit être entre 1 et 8.\n");
            continue;
        }
        
        result = (8 - ligne) * 10 + (colonne-'A');
        return result;
    } while (1);
}


void score(partie *p){
    clearecran();
    if(p->etat==2){printf("Partie terminée !\n");
    printf("-- -- -- -- -- -- -- -- -- --\n");
    printf("Score final\n");
    }
    else{
    printf("-- Partie en pause -- !\n");}
    if(p->etat==2){if(p->gagnant==0){printf("\033[1;34mJoueur bleu gagnant\033[0m\n");}
    else if(p->gagnant==1){printf("\033[1;31mJoueur rouge gagnant\033[0m\n");}
    else{printf("Match nul !");}
    printf("Score bleu : %d\n",p->scoreblack);
    printf("Score rouge : %d\n",p->scorewhite);
    printf("Nombre de coups joués : %d\n",p->coupsjoues);
    printf("Nombre de pièces perdues par le joueur bleu : %d\n",p->mortsnoirs);
    printf("Nombre de pièces perdues par le joueur rouge : %d\n",p->mortsblancs);
    printf("Temps de jeu restant: %d secondes \t \033[90m%d secondes\n\033[0m",TEMPS-p->tempsteam0,TEMPS-p->tempsteam1);
    }
    return;
}
