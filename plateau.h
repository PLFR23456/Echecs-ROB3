#include "piece.h"
#ifndef PLATEAU_H
#define PLATEAU_H
#define TAILLE 8
typedef struct deplacement {  // Ajoutez le mot-clé 'struct'
    int x;
    int y;
    int info;
    struct deplacement* next;  // Utilisez 'struct deplacement*' au lieu de 'deplacement*'
} deplacement;  // Ajoutez un typedef

void affiche(piece plateau[TAILLE][TAILLE], deplacement* liste);
void initialise_plateau(piece plateau[TAILLE][TAILLE]);
void addtolist(int x, int y, int info,deplacement* head, piece plateau[TAILLE][TAILLE]);
void promotion(int x, int y, piece plateau[TAILLE][TAILLE]);
#endif