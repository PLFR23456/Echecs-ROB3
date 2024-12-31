#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "menu.h"
#include "piece.h"
#include "plateau.h"
#include "calculus.h"

int demander_et_convertir_case();
int choixducoup1(piece tableau[TAILLE][TAILLE], int couleur, int tab[2], int aidealavisee);
int v;
void jeu(partie *p){
    int tour = p->aquiletour; 
    int pause=0; // variable tampon pour indiquer une pause dans le jeu
    if(p->etat==1){p->etat=0;}
    while(p->etat==0){
        if(tour==0){ // TOUR A MOI JOUEUR HUMAIN
        affiche(p->plateau,0);
        int tabpositions[3]; 
        int c1=choixducoup1(p->plateau,0,tabpositions,p->aidealavisee); // la tabposition est mis à jour avec cette ligne
        if(c1==-1){pause=1;break;}
        if(c1==-2){p->etat=2;return;}
        deplacement d1 = {tabpositions[0]/10,tabpositions[0]%10};
        deplacement d2 = {tabpositions[1]/10,tabpositions[1]%10};
        applymove(p->plateau,d1,d2,0);
        tour=1;       
               

        }
        affiche(p->plateau,0);
        if(tour==1){ 
            //IA
            verifs(p,1,0);
            if(p->niveauIA==0){ //si cest du humain contre humain
                affiche(p->plateau,0);
                int tabpositions[2]; 
                int c1=choixducoup1(p->plateau,1,tabpositions,p->aidealavisee);
                if(c1==-1){pause=1;break;}
                deplacement d1 = {tabpositions[0]/10,tabpositions[0]%10};
                deplacement d2 = {tabpositions[1]/10,tabpositions[1]%10};
                applymove(p->plateau,d1,d2,0);
                tour=0;
            }
            if(p->niveauIA==1 || p->niveauIA==2 || p->niveauIA==3){
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
                    if(echec==1){printf("\nEchec et mat !\n\033[0m");}
                    else{printf("\nPat !\n\033[0m");}
                    printf("pour l'équipe %c",tour);
                    freelist(listepionjouables);
                    return;
            ////////////////////////////////////////////////////////////////////////////////// ECHEC ET MAT OU PAT
                }   
                

                //////choix du pion à déplacer
                int sizeofliste;
                sizeofliste=tailleliste(listepionjouables);
                int entier_aleatoire = rand() % sizeofliste;
                printf("liste de taille %d\n",sizeofliste);
                lireliste(listepionjouables);
                if(entier_aleatoire!=0){
                for(int i=0;i<entier_aleatoire;i++){
                    listepionjouables=listepionjouables->next;
                }}
                deplacement start={listepionjouables->x,listepionjouables->y};
                freelist(listepionjouables);
                printf("L'IA a choisi de déplacer le pion en %d%d\n",start.x,start.y);
                //////choix du pion à déplacer

                //////choix du déplacement
                deplacement* listedep=(deplacement*)malloc(sizeof(deplacement));
                listedep=possiblemove(p->plateau,start);
                if(p->niveauIA==2 || p->niveauIA==3){
                    listedep=sendattackpossibilites(p->plateau,listedep, p->niveauIA);
                }
                printf("chosiis ! ");
                lireliste(listedep);
                sizeofliste=tailleliste(listedep);
                entier_aleatoire = rand() % sizeofliste;
                if(entier_aleatoire!=0){
                for(int i=0;i<entier_aleatoire;i++){
                    listedep=listedep->next;
                }}
                printf("dep");
                deplacement arrivee={listedep->x,listedep->y};
                freelist(listedep);
                //////choix du déplacement


                if(p->niveauIA==1 || p->niveauIA==3){//timer
                //possibilité de passer le timer pour le vrai joueur
                }


                applymove(p->plateau,start,arrivee,0);
                tour=0;
            }
            p->aquiletour=tour;  
        }
    }
    if(pause==1){remplacerOuAjouterPartie(p);}
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
            if(echec==1){printf("\nEchec et mat !\n\033[0m");}
            else{printf("\nPat !\n\033[0m");}
            printf("pour l'équipe %c",couleur);
            freelist(listepionjouables);
            return -2;
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

        ////// choix du déplacement
        if(estdanslaliste(buffer,listepionjouables)){ //si pion est dans la liste des pions deplacable, on continue
            deplacement dep={buffer/10,buffer%10};
            deplacement* liste = possiblemove(tableau,dep);
            removeimpossiblemove(liste, dep, tableau);
            addtolist(dep.x,dep.y,-2,liste,tableau); //on rajoute dep à la liste avec info à -2 UNIQUEMENT pour qu'il apparaisse en orange
            affiche(tableau,liste);
            do{
                printf("Donnez la case dans laquelle déplacer le pion\033[90m [-1 : return.]\033[0m\n");
                buffer2 = demander_et_convertir_case();
                if(buffer2==-1){freelist(liste);break;}
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
    char case_str[3];  // tableau pour stocker la chaîne de la case (ex: "A1", "H8")
    int result;
    // demander une entrée en boucle
    do {printf("Entrez une case entre A1 et H8: ");
        scanf("%2s", case_str);  // Lire une chaîne de 2 caractères
        if(case_str[0]=='-' && case_str[1]=='1' && case_str[2]=='\0'){
            return -1;
            }    
        // vérifier que la colonne est entre A et H
        char colonne = toupper(case_str[0]); // toupper renvoie un char en majuscule si en minuscule
        if(colonne < 'A' || colonne > 'H') {
            printf("Entrée invalide. La colonne doit être entre A et H.\n");
            continue;
        }
        // vérifier que la ligne est un chiffre entre 1 et 8
        int ligne = case_str[1] - '0';
        if(ligne< 1 || ligne>8) {
            printf("Entrée invalide. La ligne doit être entre 1 et 8.\n");
            continue;
        }
        result=(8 - ligne) * 10 + (colonne-'A');
        return result;
    }while (1);  // Continuer à demander tant que l'entrée est invalide
}

