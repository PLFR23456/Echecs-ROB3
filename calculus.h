#include "piece.h"
#include "plateau.h"
#ifndef CALCULUS_H
#define CALCULUS_H
int applymove(piece plateau[TAILLE][TAILLE], deplacement depart, deplacement arrivee);
deplacement *possiblemove(piece plateau[TAILLE][TAILLE], deplacement depart);
int estunvraicoup(int coup,deplacement *liste);
void freelist(deplacement *liste);
void listedemespions(piece plateau[TAILLE][TAILLE],int couleur, deplacement *liste);
int tailleliste(deplacement *liste);
enum cas2;
#endif