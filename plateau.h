#include "piece.h"
#ifndef PLATEAU_H
#define PLATEAU_H
#define TAILLE 8
typedef struct deplacement {  // Ajoutez le mot-clé 'struct'
    int x;
    int y;
    int info; //-2 = propre pion (non attaquable) -1 = vide, 0 = coup normal, 10 à 25+ -> pion mangé 1X = pion de couleur 0 , 2X pion de couleur 1
    struct deplacement* next;  // Utilisez 'struct deplacement*' au lieu de 'deplacement*'
} deplacement;  // Ajoutez un typedef

void affiche(piece plateau[TAILLE][TAILLE], deplacement* liste);
void initialise_plateau(piece plateau[TAILLE][TAILLE]);
void creer_partie_personnalisee(piece tableau[TAILLE][TAILLE]);
void promotion(int x, int y, piece plateau[TAILLE][TAILLE]);
#endif