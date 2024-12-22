#include "piece.h"
#include "misc.h"

int valeur_piece(piece x){
    switch (x.rang)
            {
            case PION:
                return 1;
            case CAVALIER:
                return 3;
            case FOU:
                return 3;
            case TOUR:
                return 5;
            case DAME:
                return 10;
            default:
                return 0;
            }
}