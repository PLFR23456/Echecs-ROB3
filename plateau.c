#include <stdlib.h>
#include <stdio.h>
#include "plateau.h"
#include <wchar.h>
#include "piece.h"
#include <ctype.h>
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
                #ifndef _WIN32
                printf(" "); //affichage different avec linux! donc rajouter un espace
                #endif
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


void promotion(int x, int y, piece plateau[TAILLE][TAILLE]){
    char c;
    do{
    printf("Votre pion a atteint la dernière rangée, c'est l'heure de la promotion !\n  Tapez la première lettre de la pièce par laquelle vous voulez le remplacer : (CAVALIER ; FOU ; TOUR ; DAME ; ROI)\n");
    scanf("%c",&c);
    c=toupper(c);
    if(c=='C' || c=='F' || c=='T' || c=='D'){break;}
    }while(1);
    switch (c)
    {
    case 'C':
        plateau[x][y].rang=CAVALIER;
        break;
    case 'F':
        plateau[x][y].rang=FOU;
        break;
    case 'T':
        plateau[x][y].rang=TOUR;
        break;
    case 'D':
        plateau[x][y].rang=DAME;
        break;
    }
    plateau[x][y].opt=3;
    return;
}


void creer_partie_personnalisee(piece tableau[TAILLE][TAILLE]) {
    char ligne[TAILLE + 1];
    printf("Entrez la configuration du plateau ligne par ligne (X pour vide, p pour pion bleu, P pour pion rouge, c pour cavalier bleu, C pour cavalier rouge, f pour fou bleu, F pour fou rouge, t pour tour bleu, T pour tour rouge, d pour dame bleu, D pour dame rouge, r pour roi bleu, R pour roi rouge):\n");
    
    for (int i = 0; i < TAILLE; i++) {
        printf("Ligne %d: ", i + 1);
        scanf("%s", ligne);
        for (int j = 0; j < TAILLE; j++) {
            switch (ligne[j]) {
                case 'X':
                    tableau[i][j].rang = -1;
                    tableau[i][j].couleur = -1;
                    break;
                case 'p':
                    tableau[i][j].rang = PION;
                    tableau[i][j].couleur = 0;
                    break;
                case 'P':
                    tableau[i][j].rang = PION;
                    tableau[i][j].couleur = 1;
                    break;
                case 'c':
                    tableau[i][j].rang = CAVALIER;
                    tableau[i][j].couleur = 0;
                    break;
                case 'C':
                    tableau[i][j].rang = CAVALIER;
                    tableau[i][j].couleur = 1;
                    break;
                case 'f':
                    tableau[i][j].rang = FOU;
                    tableau[i][j].couleur = 0;
                    break;
                case 'F':
                    tableau[i][j].rang = FOU;
                    tableau[i][j].couleur = 1;
                    break;
                case 't':
                    tableau[i][j].rang = TOUR;
                    tableau[i][j].couleur = 0;
                    break;
                case 'T':
                    tableau[i][j].rang = TOUR;
                    tableau[i][j].couleur = 1;
                    break;
                case 'd':
                    tableau[i][j].rang = DAME;
                    tableau[i][j].couleur = 0;
                    break;
                case 'D':
                    tableau[i][j].rang = DAME;
                    tableau[i][j].couleur = 1;
                    break;
                case 'r':
                    tableau[i][j].rang = ROI;
                    tableau[i][j].couleur = 0;
                    break;
                case 'R':
                    tableau[i][j].rang = ROI;
                    tableau[i][j].couleur = 1;
                    break;
                default:
                    printf("Caractère invalide. Réessayez.\n");
                    j--;
                    break;
            }
        }
    }
}
