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
            if ((i + j) % 2 == 0) {
                printf("\033[48;5;15m"); // Fond blanc
            } else {
                printf("\033[48;5;235m"); // Fond noir
            }
            if(plateau[i][j].couleur==1){
                printf("\033[31m"); //couleur rouge
                }
            else if (plateau[i][j].couleur==0)
            {
                printf("\033[34m"); //couleur bleu
            }
            if(liste!=0){checkAndPrint(liste,i,j);}
            switch (plateau[i][j].rang)
            {
            case PION:
                printf("♟P");
                break;
            case CAVALIER:
                printf("♞C");
                break;
            case FOU:
                printf("♝F");
                break;
            case TOUR:
                printf("♜T");
                break;
            case DAME:
                printf("♛D");
                break;
            case ROI:
                printf("♚R");
                break;
            default:
                printf(" ");
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
            // Afficher en jaune si la case est dans la liste des déplacements possibles
            printf("\033[48;5;226m"); // Fond jaune
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


void promotion(int x, int y, piece plateau[TAILLE][TAILLE]) {
    char c;
    int valid = 0; // Flag pour vérifier si l'entrée est valide
    while (!valid) {
        printf("Votre pion a atteint la dernière rangée, c'est l'heure de la promotion !\n");
        printf("Tapez la première lettre de la pièce par laquelle vous voulez le remplacer : (CAVALIER ; FOU ; TOUR ; DAME ; ROI)\n");
        scanf(" %c", &c); // L'espace avant %c ignore les espaces blancs résiduels
        viderTampon(); // Vide le tampon après la lecture
        c = toupper(c); // Convertit le caractère en majuscule pour simplifier la comparaison

        if (c == 'C' || c == 'F' || c == 'T' || c == 'D') {
            valid = 1; // Marque l'entrée comme valide
            switch (c) {
                case 'C':
                    plateau[x][y].rang = CAVALIER;
                    break;
                case 'F':
                    plateau[x][y].rang = FOU;
                    break;
                case 'T':
                    plateau[x][y].rang = TOUR;
                    break;
                case 'D':
                    plateau[x][y].rang = DAME;
                    break;
            }
            plateau[x][y].opt = 3;
        } else {
            printf("Entrée invalide. Veuillez recommencer.\n");
        }
    }
}


void creer_partie_personnalisee(piece tableau[TAILLE][TAILLE]) {
    while(1){
    char ligne[TAILLE + 1];
    printf("Entrez la configuration du plateau ligne par ligne (X pour vide, p pour pion bleu, P pour pion rouge, c pour cavalier bleu, C pour cavalier rouge, f pour fou bleu, F pour fou rouge, t pour tour bleu, T pour tour rouge, d pour dame bleu, D pour dame rouge, r pour roi bleu, R pour roi rouge):\n les minuscules/bleus commencent en haut et les majuscules/rouges en bas. Pour éviter les problèmes de logique\n");
    int roiblanc,roinoir;
    roiblanc=0;
    roinoir=0;
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
                    roiblanc=1;
                    break;
                case 'R':
                    tableau[i][j].rang = ROI;
                    tableau[i][j].couleur = 1;
                    roinoir=1;
                    break;
                default:
                    printf("Caractère invalide. Réessayez.\n");
                    i--;
                    break;
            }
        }
    }
    if (roiblanc==1 && roinoir==1){
        break;
    }
    else{
        printf("Il doit y avoir un roi de chaque couleur sur le plateau.\n");
    }
    
    }
}