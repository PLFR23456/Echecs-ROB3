#include <stdlib.h>
#include <stdio.h>
#include "fonctions.h"


typedef struct pieces {

    char* type;
    //pas besoin de coord ?
    int x;
    int y;
    int couleur; //comme type avec avec int pour verif + rapide
    int etat; // 0 ou 1 joué/pas joué
    struct pieces* suivant;// pas besoin suivant ?

    }pieces;

typedef struct coordonnees {

    int x;
    int y;

    }coordonnees;

pieces** initPieces(); //initialise le tab des pieces et leur position dans un tableau
pieces* affichePieces(pieces* tete);//affiche juste les pieces pr voir si tt ok
int affichePlateau(pieces** tableau); //fonction qui affiche le plateau
pieces** getCoord(pieces** tableau); // recupere les coord du pion qu'on veut jouer
void coupPion(pieces** tableau, int coordx, int coordy); // recoit les coord et effectue le coup souhaité. peut etre pas un void


int main() {

    //         !! NE PAS EFFACER !!
    /*
    // cavaliers
    printf("\u2658\n");
    printf("\033[31m\u265e\033[0m\n");
    //fou
    printf("\u2657\n");
    printf("\033[31m\u265d\033[0m\n");
    //roi
    printf("\u2654\n");
    printf("\033[31m\u265a\033[0m\n");
    //reine
    printf("\u2655\n");
    printf("\033[31m\u265b\033[0m\n");
    //tour
    printf("\u2656\n");
    printf("\033[31m\u265c\033[0m\n");
    //pions
    printf("\u265f\n");
    printf("\033[31m\u2659\033[0m");
    printf("\033[33m"); JAUNE
    */
    pieces** tableauInitial = initPieces(); // initialisation du plateau à l'etat initual. nb : mauvais chois des noms des variables
    affichePlateau(tableauInitial); // affiche le tableau
    getCoord(tableauInitial); // execute un premier coup


    
    return 0;
}

pieces** initPieces() {

    //pieces* tete = (pieces*)malloc(sizeof(pieces));
    //pieces* courant = (pieces*)malloc(sizeof(pieces));
    //courant = tete;
    char* caracterePiecesB[] = {"\u2656", "\u2658","\u2657", "\u2655", "\u2654", "\u2659"};
    char* caracterePiecesR[] = {"\033[31m\u265c\033[0m", "\033[31m\u265e\033[0m","\033[31m\u265d\033[0m", "\033[31m\u265b\033[0m", "\033[31m\u265a\033[0m", "\033[31m\u2659\033[0m"};
    //expliquer le role de ces tableaux
    int tabB[] = {0, 1, 2, 3, 4, 2, 1, 0, 5, 5, 5, 5, 5, 5, 5, 5};
    int tabR[] = {5, 5, 5, 5, 5, 5, 5, 5, 0, 1, 2, 3, 4, 2, 1, 0};
    int k = 0;

    pieces** tableau = (pieces**)malloc(8*sizeof(pieces));
    for(int i = 0; i < 8; i++)
        tableau[i] = (pieces*)malloc(8*sizeof(pieces));
    
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++) 
            {
                tableau[i][j].type = " ";
            }

    // initialise les pieces noires
    for(int i = 0; i < 2; i++)
        {
            for(int j = 0; j < 8; j++) 
                {
                    tableau[i][j].type = caracterePiecesB[tabB[k]];
                    tableau[i][j].etat = 1; // initialise l'etat des pieces noires, 1 =  pas joué
                    k++;
                }
        }
    k = 0;
    // initialise les pieces rouges
    for(int i = 6; i < 8; i++)
        for(int j = 0; j < 8; j++) 
            {
                tableau[i][j].type = caracterePiecesR[tabR[k]];
                tableau[i][j].etat = 1; // initialise l'etat des pieces rouges
                k++;
            }
    
    return tableau;    
}

pieces* affichePieces(pieces* tete) {
    //verifie que la structure a bien été initialisée avce les bonnes valeurs
    // ne sert plus a rien
    pieces* courant = (pieces*)malloc(sizeof(pieces));
    courant = tete;

    while(courant->suivant != NULL)
        {
            printf("x : %d, y : %d\n", courant->x, courant->y);
            printf("type : %s\n", courant->type);
            courant = courant->suivant;
        }


    
    return tete;
}


int affichePlateau(pieces** tableau) {

    
            for(int j = 0; j < 8; j++)
                printf("+--");
            printf("+\n");

            for(int j = 0; j < 8; j++)
                {
                    for(int k = 0; k < 8; k++)
                        {
                            printf("|%s ", tableau[j][k].type);
                            //nerf de la guerre 1
                        }
                    printf("| %d\n", j);
                    
                    for(int i = 0; i < 8; i++)
                        printf("+--");
                    
                    printf("+\n");
                }

    printf("| 0| 1| 2| 3| 4| 5| 6| 7|\n\n");
    
    return 0;
    
}

pieces** getCoord(pieces** tableau) {

    int coordx;
    int coordy;
    int ok = 0;// vrai ou faux, vaut 1 si les coordonées données sont justes.

    do
        {
            printf("entrez la coordonée en x puis en y de la pièce : \n");
            scanf("%d\n", &coordx);
            scanf("%d", &coordy); //bug si autre que int
            if(coordx > 7 || coordy > 7 || coordx < 0 || coordy < 0 ||                          tableau[coordy][coordx].type == " ")
                printf("Vous avez choisi une case vide ou non valide. Veuillez choisir une case comprenant une pièce\n");
            else    
                {
                    printf("pièce sélectionnée : %s\n", tableau[coordy]                           [coordx].type);
                    ok = 1;
                }
                    
        } while(ok == 0);
    // depuis le début de la fonction, on récupère les coord de la pièce que le joueur veut jouer
    // une fois les coord récupérées, on appelle une fonction effectuant le coup de la piece coresp.

    // verifie que le coup joué soit un pion
    if(tableau[coordy][coordx].type == "\033[31m\u2659\033[0m")
        coupPion(tableau, coordx, coordy);
    else
        printf("ce coup n'est pas encore défini\n");

    return NULL;// a modifier pour renvoyer le tab final
}

void coupPion(pieces** tableau, int coordx, int coordy) {

    // nb : coord déjà ok
    // nb : 25 coups possibles au max (reine)
    //on considère que le tour est au rouge pr commencer
    
    pieces** tableau_identique = (pieces**)malloc(8*sizeof(pieces));

    // initialisation d'un tab identique pour lui faire afficher les coups possibles
    for(int i = 0; i < 8; i++)
        tableau_identique[i] = (pieces*)malloc(8*sizeof(pieces));

    for(int x = 0; x < 8; x++)
        for(int y = 0; y < 8; y++) 
            {
                tableau_identique[y][x].type = tableau[y][x].type;
            }
    
    coordonnees case1, case2, case3, case4;
    // cases à vérifer pour un pion rouge
    case1.x = coordx; 
    case1.y = coordy - 1;
    
    case2.x = coordx; 
    case2.y = coordy - 2;
    // condition : pour avancer de 2 cases, il faut que etat = 1
    if(tableau[coordy][coordx].etat == 0)
        case2.y = coordy - 1;
    
    case3.x = coordx + 1; 
    case3.y = coordy - 1;
    
    case4.x = coordx - 1; 
    case4.y = coordy - 1;
    
    coordonnees cases_bouge[] = {case1, case2};
    coordonnees cases_mange[] = {case3, case4};
    // boucle qui remplace les cases de déplacement par des X
    for(int i = 0; i < 2; i++) 
        {
            if(tableau[cases_bouge[i].y][cases_bouge[i].x].type == " ") 
                {
                    tableau_identique[cases_bouge[i].y][cases_bouge[i].x].type = "X";
                }
        }
    // boucle qui remplace les pions mangés par une couleur differente
    for(int i = 0; i < 2; i++) 
        {
            if(tableau[cases_mange[i].y][cases_mange[i].x].type != " ") 
                {
                    tableau_identique[cases_mange[i].y][cases_mange[i].x].type = "X";
                    // modifier le "X" pour uniquement modifier la couleur du type en jaune
                }
        }

    affichePlateau(tableau_identique);
    //free(tableau_identique); pas oublier le free mais pas maintenant

    int xf; // coord du coup final
    int yf;
    int ok = 0;

    do {
            printf("Entrez les coordonnées du coup que vous voulez jouer : \n");
            scanf("%d\n", &xf);
            scanf("%d", &yf);
            if(tableau_identique[yf][xf].type != "X" || tableau_identique[yf][xf].type == " ")
        } while(ok == 0)
        
    pieces temp = tableau[coordy][coordx];
    tableau[coordy][coordx].type = " ";
    tableau[yf][xf] = temp;

    affichePlateau(tableau);

}



