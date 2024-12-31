#ifndef PIECE_H
#define PIECE_H
enum rangs{PION, CAVALIER, FOU, TOUR, DAME, ROI};
typedef struct{
    int rang; //valeur du pion
    int couleur; // 0 noir 1 blanc
    int opt; 
    /*
    -1 = rien / pion mort
    0=R
    1=pion ou tour pas encore avancé
    2 = pion mangeable au tour suivant / vient juste detre avancé /!\ achaque debut de tour verifier si un de nos pions esst 2 POUR LEM mettre en 0
    3= pion promu (utile pour inverser le sens de deplacement)
    */
}piece;

#endif
