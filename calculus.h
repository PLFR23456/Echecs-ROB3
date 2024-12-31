#include "piece.h"
#include "plateau.h"
#include "menu.h"
#ifndef CALCULUS_H
#define CALCULUS_H
int applymove(piece plateau[TAILLE][TAILLE], deplacement depart, deplacement arrivee, int test);
deplacement *possiblemove(piece plateau[TAILLE][TAILLE], deplacement depart);
int estunvraicoup(int coup,deplacement *liste,int* infosurlecoup);
void freelist(deplacement *liste);
void listedemespions(piece plateau[TAILLE][TAILLE],int couleur, deplacement *liste, int niveauIA);
int tailleliste(deplacement *liste);
int roienechec(piece plateau[TAILLE][TAILLE], deplacement monroi);
void addtolist(int x, int y, int info,deplacement* head, piece plateau[TAILLE][TAILLE]);
int verifs(partie* p,int couleur,int infoatransmettre);
void removeimpossiblemove(deplacement* listemove, deplacement depart, piece plateau[TAILLE][TAILLE]);
deplacement* sendattackpossibilites(piece tableau[TAILLE][TAILLE],deplacement* liste, int nvIA);
int estdanslaliste(int dep, deplacement* liste);
void lireliste(deplacement* liste);
enum cas2;
#endif
