#include "plateau.h"
#define TAILLENOMPARTIE 10
#ifndef MENU_H
#define MENU_H
typedef struct partie{
    piece plateau[TAILLE][TAILLE] ;
    int aquiletour; // joueur 0 / joueur 1 / 2= partie finie
    int niveauIA; // plusieurs types de comportements à définir
    /*
    0= pas d'IA on joue en changeant
    */
    int tempsreflexion;
    int aidealavisee;
    char nom[TAILLENOMPARTIE];
    //statistiques
    int temps;
    int coupsjoues;
    int mortsblancs;
    int mortsnoirs;
    int scorewhite;
    int scoreblack;
    //optionnel
    int gagnant; //0
    int etat; //0 = en jeu, 1= pause, 2=fini
    int numpartie;
}partie;
void enregistrerPartie(partie *p);
void remplacerOuAjouterPartie(partie *p);
void viderTampon();
#endif
