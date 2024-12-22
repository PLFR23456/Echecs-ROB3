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
void verifs(piece plateau[TAILLE][TAILLE],int couleur,int infoatransmettre, int niveauIA);
void jeu(partie *p){
    int tour = p->aquiletour; 
    int pause=0; // variable tampon pour indiquer une pause dans le jeu
    if(p->etat==1){p->etat=0;}
    while(p->etat==0){
        if(tour==0){ // TOUR A MOI JOUEUR HUMAIN
            verifs(p->plateau,0,0,0);
            //verifs : promotion + pion2->0 + [si 0 move dispo + echec et maths]
            affiche(p->plateau,0);
            int tabpositions[2]; 
            int c1=choixducoup1(p->plateau,0,tabpositions,p->aidealavisee); // la tabposition est mis à jour avec cette ligne
            if(c1==-1){pause=1;break;}
            deplacement d1 = {tabpositions[0]/10,tabpositions[0]%10};
            deplacement d2 = {tabpositions[1]/10,tabpositions[1]%10};
            applymove(p->plateau,d1,d2);
            tour=1;       
               

        }
        affiche(p->plateau,0);
        if(tour==1){
            //IA
            verifs(p->plateau,1,0,p->niveauIA);
            if(p->niveauIA==0){ //si cest du humain contre humain
                affiche(p->plateau,0);
                int tabpositions[2]; 
                int c1=choixducoup1(p->plateau,1,tabpositions,p->aidealavisee);
                if(c1==-1){pause=1;break;}
                deplacement d1 = {tabpositions[0]/10,tabpositions[0]%10};
                deplacement d2 = {tabpositions[1]/10,tabpositions[1]%10};
                applymove(p->plateau,d1,d2);
                tour=0;
            }
            if(p->niveauIA==1 || p->niveauIA==2 || p->niveauIA==3){
                if(p->niveauIA==1 || p->niveauIA==3){//timer
                }
                deplacement* liste1=(deplacement*)malloc(sizeof(deplacement));
                liste1->info=-1;
                listedemespions(p->plateau,1,liste1);
                if(liste1->info==-1){printf("plus de pions nullos!!\n");return;} //avant ca il y a echec et math donc a suppr
                int a=0;
                a=tailleliste(liste1);
                int entier_aleatoire = rand() % a;
                for(int i=0;i<entier_aleatoire;i++){
                    liste1=liste1->next;
                }
                deplacement start={liste1->x,liste1->y};
                freelist(liste1);
                deplacement* liste2=(deplacement*)malloc(sizeof(deplacement));
                liste2=possiblemove(p->plateau,start);
                if(liste2->info==-1){printf("plus de move nullos!!\n");return;} //cas impossible donc ligne a suppr
                a=0;
                a=tailleliste(liste2);
                entier_aleatoire = rand() % a;
                for(int i=0;i<entier_aleatoire;i++){
                    liste2=liste2->next;
                }
                deplacement arrivee={liste2->x,liste2->y};
                freelist(liste2);
                applymove(p->plateau,start,arrivee);
                tour=0;
            }
            p->aquiletour=tour;  
        }
        
        /*while(pas fini){
        achaque debut de tour verifier si un de nos pions esst 2 POUR LEM mettre en 0
        + verifier si ya promotion a faire ou pas
            si c ton tour {
                Pause /quitter  ->>> fct pause + enregistrer
                choisissez une case ->*/
                //si aide a la visee
                //si pas de move dispo (sur tous les pions!!)
                    // pat? /game fini
                //analyse de chaque move possible -> trouver si echec et mat etc.. -- boucle for dans une fonction
                    // echec et mat et +++
                //{WHILE
                //demander une case 1 // pause quitter sauvegarder -> -1 entrée
                    //si case pas bonne -> re demander une case1
                    //si case bonne mais sans move possible & aide activée -> re demander une case1
                //}
                //options : retour -1 -> demander une case 1 OU demander une case 2
                    // si case pas bonne 
                        //  si aide activee redemander case 2
                        // sinon enlever 5 secondes au timer
                    //si roi toujours en echec redemander une case 2
                //si timer est fini : mouvement random + au tour de l'autre joueur
                /*tour = IA;
                }
        si c tour de lIA {
            iadecision (plateau, niveauIA, equipeIA) ->> renvoie un coup (= un pion + son emplacement)
            }
    remplacer partie des la fin du coup
    }*/
    }
    if(pause==1){remplacerOuAjouterPartie(p);}
    return;
}



int choixducoup1(piece tableau[TAILLE][TAILLE], int couleur, int tab[2],int aidealavisee){
    int buffer,buffer2;
    do{
        printf("Donnez la case du pion à déplacer !");
        if(couleur==0){printf("\033[34m team bleue :\033[0m\033[90m [-1 : leave the game.]\033[0m\n");}
        else{printf("\033[31m team rouge :\033[0m\n");}
        buffer = demander_et_convertir_case();
        if(buffer==-1){return-1;}
        if(tableau[buffer/10][buffer%10].couleur==couleur){ //si mouvement valable (pion de labonne couleur)
            deplacement dep={buffer/10,buffer%10};
            deplacement* liste = possiblemove(tableau,dep);
            if(liste->info==-1){
                printf("\033[91mPas de mouvements disponibles sur ce pion !\n\033[0m");continue;}
            addtolist(buffer/10, buffer%10, -2, liste,tableau);
            //on rajoute dep à liste
            affiche(tableau,liste);
            do{
                printf("Donnez la case dans laquelle déplacer le pion\033[90m [-1 : return.]\033[0m\n");
                buffer2 = demander_et_convertir_case();
                if(buffer2==-1){freelist(liste);break;}
                if(estunvraicoup(buffer2, liste)==1){ //on peut pas se retrouver bloquer sans coup car cest verifier juste avant
                    //bon!!
                    tab[0]=buffer;
                    tab[1]=buffer2;
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
    }while(1);
    return buffer;
}







int demander_et_convertir_case() {
    char case_str[3];  // tableau pour stocker la chaîne de la case (ex: "A1", "H8")
    int result;
    // Demander une entrée en boucle
    do {printf("Entrez une case entre A1 et H8: ");
        scanf("%2s", case_str);  // Lire une chaîne de 2 caractères
        if(case_str[0]=='-' && case_str[1]=='1' && case_str[2]=='\0'){
            return -1;
            }    
        // Vérifier que la colonne est entre A et H
        char colonne = toupper(case_str[0]); // toupper renvoie un char en majuscule si en minuscule
        if(colonne < 'A' || colonne > 'H') {
            printf("Entrée invalide. La colonne doit être entre A et H.\n");
            continue;
        }
        // Vérifier que la ligne est un chiffre entre 1 et 8
        int ligne = case_str[1] - '0';
        if(ligne< 1 || ligne>8) {
            printf("Entrée invalide. La ligne doit être entre 1 et 8.\n");
            continue;
        }
        result=(8 - ligne) * 10 + (colonne-'A');
        return result;
    }while (1);  // Continuer à demander tant que l'entrée est invalide
}

void verifs(piece plateau[TAILLE][TAILLE],int couleur,int infoatransmettre, int niveauIA){
    int echec=0;
    for(int i=0;i<TAILLE;i++){
        for(int j=0;j<TAILLE;j++){
            if(plateau[i][j].opt==2 && plateau[i][j].couleur==couleur){plateau[i][j].opt=0;}
            if(plateau[i][j].rang==PION && plateau[i][j].couleur==couleur && i==7*(1-couleur)){
                if(niveauIA==0 || couleur==0){
                    printf("promotion !!");
                    deplacement montrer={i,j,-2};
                    affiche(plateau,&montrer);
                    promotion(i, j, plateau); //promotion !!
                }
                else{
                    int r = rand()%4;
                    plateau[i][j].rang=r+1; // 0= pion, 1à4 = CAVALIER,FOU... 5=ROI
                }
            }
        }
    }
    //verif echec
    deplacement* listepionsteamadverse=(deplacement*)malloc(sizeof(deplacement));
    listepionsteamadverse->info=-1;
    listedemespions(plateau,1-couleur,listepionsteamadverse); //1-couleur -> 0:1  1:0
    deplacement* adresse = listepionsteamadverse;
    if(listepionsteamadverse->info!=-1){ //aucun coup dispo pas besoin de cette ligne si on verifie avant quaucun coup est dispo
    while (adresse != NULL) {
        deplacement coup={adresse->x,adresse->y};
        printf("verif ! %d%d",adresse->x,adresse->y);
        deplacement* listecoup = possiblemove(plateau,coup);
        deplacement* adresse2 = listecoup;
        if(listecoup->info!=-1){
            while (adresse2 != NULL) {
                if (adresse2->info==6) {
                    //ECHECS!!
                    echec=1;
                    printf("%d",echec);
                }
                adresse2 = adresse2->next;
            }}
        adresse = adresse->next;
    }}
    //listepionsteamadverse
    //for listepionsteamadvers:
    //    possiblemove(i)
    //    for(j in possiblemove): -- parcourir possiblemove la liste
    //              if(head->info==ECHEC): echec =1

}



