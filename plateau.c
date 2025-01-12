#include <stdlib.h>
#include <stdio.h>
#include "plateau.h"
#include <wchar.h>
#include "piece.h"
#include "calculus.h"
void checkAndPrint(deplacement* head, int x, int y);
void affiche(piece plateau[TAILLE][TAILLE], deplacement *liste){
    for(int i=0;i<TAILLE;i++){printf("-+-");}
    printf("-+\n");
    for(int i=0;i<TAILLE;i++){
        printf("%d",8-i);
        for(int j=0;j<TAILLE;j++){
            printf("|");
            if(plateau[i][j].couleur==1){
                printf("\033[31m");
                }
            else if (plateau[i][j].couleur==0)
            {
                printf("\033[34m");
            }
            if(liste!=0){checkAndPrint(liste,i,j);}
            switch (plateau[i][j].rang)
            {
            case PION:
                printf("♟ ");
                break;
            case CAVALIER:
                printf("♞ ");
                break;
            case FOU:
                printf("♝ ");
                break;
            case TOUR:
                printf("♜ ");
                break;
            case DAME:
                printf("♛ ");
                break;
            case ROI:
                printf("♚ ");
                break;
            default:
                printf("🙾");
                break;
            }
            printf("\033[0m");
        }printf("|\n");
    }
    for(int i=0;i<TAILLE;i++){printf("-+-");}
    printf("-+\n  ");
    for(int i=0;i<TAILLE;i++){printf("%c  ",'a'+i);}
    printf("\n\n");
}


void checkAndPrint(deplacement* head, int x, int y) {
    deplacement* adresse = head;
    if(head->info==-1){return;}
    while (adresse != NULL) {  // Changer la condition pour aussi vérifier le dernier noeud
        if (adresse->x == x && adresse->y == y) {
            if(adresse->info==-2){printf("\033[38;5;214m");return;}
            printf("\033[33m"); // Afficher en jaune si la case est dans la liste des déplacements possibles
            return;  // Si un coup est trouvé, on n'a pas besoin de continuer à chercher
        }
        adresse = adresse->next;
    }
}




void initialise_plateau(piece plateau[TAILLE][TAILLE]){
    enum rangs liste1[TAILLE] = {TOUR, CAVALIER, FOU, DAME, ROI, FOU, CAVALIER, TOUR};
    for(int i=0; i<2;i++){
        for(int j=0; j<TAILLE;j++){
            plateau[0+i*7][j].rang=liste1[j]; // 0+i*8 : i=0 -> 0 , i=1 -> 7
            if(liste1[j]==TOUR){plateau[0+i*7][j].opt=1;}
            plateau[0+i*7][j].couleur=0+i;
        }
        for(int j=0; j<TAILLE;j++){
            plateau[6-i*5][j].rang=PION; // 0+i*8 : i=0 -> 1 , i=1 -> 6
            plateau[6-i*5][j].opt=1;
            plateau[6-i*5][j].couleur=1-i;
        }
    }
    for(int j=0; j<TAILLE;j++){
        for(int i=2; i<6;i++){
            plateau[i][j].rang=-1; // element nul =-1
            plateau[i][j].couleur=-1;
            plateau[i][j].opt=-1;

        }}
}

