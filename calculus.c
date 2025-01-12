#include <stdio.h>
#include <stdlib.h>
#include "piece.h"
#include "plateau.h"
#include "calculus.h"
enum cas{RIEN, PIONTEAM0MANGER, PIONTEAM1MANGER};
enum cas2{RAS, PRISEENPASSANT, ECHEC,ECHECETMATH};




int applymove(piece plateau[TAILLE][TAILLE], deplacement depart, deplacement arrivee){
    //les coups sont donnes de la maniere suivant : depart:xy et arrivee:XY
    // x et X représentent la ligne de depart/arrivée
    // et y Y la colonne
    int val=RIEN;
    int x=depart.x;
    int y=depart.y;
    int X=arrivee.x;
    int Y=arrivee.y;
    if(plateau[X][Y].couleur==1){val=PIONTEAM1MANGER;}
    if(plateau[X][Y].couleur==0){val=PIONTEAM0MANGER;}
    plateau[X][Y].rang=plateau[x][y].rang;
    plateau[X][Y].couleur=plateau[x][y].couleur;
    plateau[X][Y].opt=plateau[x][y].opt;
    plateau[x][y].rang=-1;
    plateau[x][y].couleur=-1;
    plateau[x][y].opt=-1;
    if(plateau[X][Y].opt==1){
        if(plateau[X][Y].rang==PION || plateau[X][Y].rang==TOUR){plateau[X][Y].opt=2;}
        else{plateau[X][Y].opt=0;}
    }
    return val;
}


int estunvraicoup(int coup,deplacement *liste){
    deplacement* adresse = liste;
    if(liste->info==-1){return 0;} //aucun coup dispo pas besoin de cette ligne si on verifie avant quaucun coup est dispo
    while (adresse != NULL) {
        if (adresse->x == coup/10 && coup%10 == adresse->y) {
            if(adresse->info==-2){return 0;}
            return(1);
        }
        adresse = adresse->next;
    }
    return -1; //coup pas dispo
}

void freelist(deplacement *liste){
    if(liste==NULL){return;}
    else{
        freelist(liste->next);
        free(liste);
        return;
    }
}


int tailleliste(deplacement *liste){
    if(liste->next==NULL){return 1;}
    else{
        int j=tailleliste(liste->next);
        return j+1;
    }
}



void addtolist(int x, int y, int info,deplacement* head, piece plateau[TAILLE][TAILLE]) {
    deplacement* noeud = (deplacement*)malloc(sizeof(deplacement));
    if (noeud == NULL) {
        printf("Erreur d'allocation mémoire\n");
        exit(1);
    }
    if(plateau[x][y].rang==ROI){
        noeud->info= ECHEC;
        //ECHEC !!!
    }
    printf("nouveau move en %d %d\n",x,y);
    noeud->x = x;
    noeud->y = y;
    noeud->info = info;
    noeud->next = NULL;
    if(head->info==-1){
        head->x = noeud->x;
        head->y = noeud->y;
        head->info = noeud->info;
        head->next = noeud;
        return;
    }
    deplacement* adresse=head;
    while(adresse->next!=NULL){
        adresse=adresse->next;
    }
    adresse->next=noeud;
    return;
}



deplacement *possiblemove(piece plateau[TAILLE][TAILLE], deplacement depart){
    deplacement* head=(deplacement*)malloc(sizeof(deplacement));
    head->info=-1;
    int x=depart.x;
    int y=depart.y;
    int r=plateau[x][y].rang;
    int color=plateau[x][y].couleur;
    int opt=plateau[x][y].opt;

    if(x==color*(TAILLE-1) && r==PION){
        //promotion();
    }  //si le pion d'une couleur est a l'opposee de son cote de depart

    switch (r)
    {
    case PION:{
        if(opt==1){if(plateau[x+(2-4*color)][y].couleur!=color){addtolist(x+(2-4*color), y, RAS, head,plateau);}}
        if(x<TAILLE-1 && x>0){if(plateau[x+(1-2*color)][y].couleur==-1){addtolist(x+(1-2*color), y, RAS, head,plateau);}}
        //regarde la case devant et a gauche/droite
        if(y<TAILLE-1){if(plateau[x][y+1].opt==2 && plateau[x][y+1].couleur!=color){addtolist(x, y+1, PRISEENPASSANT, head,plateau);}};
        if(y>0){if(plateau[x][y-1].opt==2 && plateau[x][y-1].couleur!=color){addtolist(x, y-1, PRISEENPASSANT, head,plateau);}};
        if(y<TAILLE-1 && x>0 && x<TAILLE-1){if(plateau[x+(1-2*color)][y+1].couleur!=color && plateau[x+(1-color*2)][y+1].couleur!=-1){addtolist(x+(1-color*2), y+1, RAS, head,plateau);}};
        if(y>0 && x>0 && x<TAILLE-1){if(plateau[x+(1-2*color)][y-1].couleur!=color && plateau[x+(1-color*2)][y-1].couleur!=-1){addtolist(x+(1-color*2), y-1, RAS, head,plateau);}};
        printf("couleur !!!%d\n",color);
        break;}
    case TOUR:{
        for(int i=x+1;i<TAILLE;i++){
            printf("couleur !!!%d\n",color);
            if(plateau[i][y].rang!=-1){if(plateau[i][y].couleur!=color){addtolist(i, y, RAS, head,plateau);printf("ok8");break;}else{break;}}
            addtolist(i, y, RAS, head,plateau); }
        for(int i=x-1;i>-1;i--){
            if(plateau[i][y].rang!=-1){if(plateau[i][y].couleur!=color){addtolist(i, y, RAS, head,plateau);printf("ok8");break;}else{break;}}
            addtolist(i, y, RAS, head,plateau); }
        for(int i=y+1;i<TAILLE;i++){
            if(plateau[x][i].rang!=-1){if(plateau[x][i].couleur!=color){addtolist(x, i, RAS, head,plateau);printf("ok8");break;}else{break;}}
            addtolist(x, i, RAS, head,plateau); }
        for(int i=y-1;i>-1;i--){
            if(plateau[x][i].rang!=-1){if(plateau[x][i].couleur!=color){addtolist(x, i, RAS, head,plateau);printf("ok8");break;}else{break;}}
            addtolist(x, i, RAS, head,plateau); }
        break;}
    case FOU:{
        for(int a=0;a<4;a++){
        int i=1,j=1;
        while(x+i-(2*i*(a/2))<TAILLE && x+i-(2*(a/2))*i>-1 && y+j-(2*(a%2))*j<TAILLE && y+j-(2*(a%2))*j>-1){
            if(plateau[x+i-(2*(a/2))*i][y+j-(2*(a%2))*j].rang!=-1){
                if(plateau[x+i-(2*(a/2))*i][y+j-(2*(a%2))].couleur!=color){
                    addtolist((int)(x+i-(2*(a/2))*i), (int)(y+j-(2*(a%2))*j), RAS, head,plateau);
                }
                break;}
            else{addtolist((int)(x+i-(2*(a/2))*i), (int)(y+j-(2*(a%2))*j), RAS, head,plateau);}
            j++;
            i++;
        }}
        break;}
    case DAME:{
        for(int a=0;a<4;a++){
        int i=1,j=1;
        while(x+i-(2*i*(a/2))<TAILLE && x+i-(2*(a/2))*i>-1 && y+j-(2*(a%2))*j<TAILLE && y+j-(2*(a%2))*j>-1){
            if(plateau[x+i-(2*(a/2))*i][y+j-(2*(a%2))*j].rang!=-1){
                if(plateau[x+i-(2*(a/2))*i][y+j-(2*(a%2))].couleur!=color){
                    addtolist((int)(x+i-(2*(a/2))*i), (int)(y+j-(2*(a%2))*j), RAS, head,plateau);
                }
                break;}
            else{addtolist((int)(x+i-(2*(a/2))*i), (int)(y+j-(2*(a%2))*j), RAS, head,plateau);}
            j++;
            i++;
        }}
        for(int i=x+1;i<TAILLE;i++){
            if(plateau[i][y].rang!=-1){if(plateau[i][y].couleur!=color){addtolist(i, y, RAS, head,plateau);printf("ok8");break;}else{break;}}
            addtolist(i, y, RAS, head,plateau); }
        for(int i=x-1;i>-1;i--){
            if(plateau[i][y].rang!=-1){if(plateau[i][y].couleur!=color){addtolist(i, y, RAS, head,plateau);printf("ok8");break;}else{break;}}
            addtolist(i, y, RAS, head,plateau); }
        for(int i=y+1;i<TAILLE;i++){
            if(plateau[x][i].rang!=-1){if(plateau[x][i].couleur!=color){addtolist(x, i, RAS, head,plateau);printf("ok8");break;}else{break;}}
            addtolist(x, i, RAS, head,plateau); }
        for(int i=y-1;i>-1;i--){
            if(plateau[x][i].rang!=-1){if(plateau[x][i].couleur!=color){addtolist(x, i, RAS, head,plateau);printf("ok8");break;}else{break;}}
            addtolist(x, i, RAS, head,plateau); }
        break;
    }
    case ROI:{
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                if(x-1+i<TAILLE && x-1+i>-1 && y-1+j<TAILLE && y-1+j>-1){
                    if(plateau[x-1+i][y-1+j].couleur!=color){addtolist(x-1+i, y-1+j, RAS, head,plateau);}}
            }
        }
        break;
    }
    case CAVALIER:{
        int cavvalues[4]={-2,-1,1,2}; // idem
        int cavvalues2[8]={-1,1,2,-2,-2,2,-1,1}; //pas tres optimise !! a revoir avec des operations modulos etc
        for(int i=0;i<8;i++){
            if(x+cavvalues[i/2]<TAILLE && x+cavvalues[i/2]>-1 && y+cavvalues2[i]<TAILLE && y+cavvalues2[i]>-1){
                    if(plateau[x+cavvalues[i/2]][y+cavvalues2[i]].couleur!=color){addtolist(x+cavvalues[i/2], y+cavvalues2[i], RAS, head,plateau);}}
            }
        break;
    }
    default:
        break;
    }
    return head;
}


void listedemespions(piece plateau[TAILLE][TAILLE],int couleur, deplacement *liste){
    for(int i=0;i<TAILLE;i++){
        for(int j=0;j<TAILLE;j++){
            if(plateau[i][j].couleur==couleur){
                deplacement dep={i,j};
                if(possiblemove(plateau,dep)->info!=-1){
                addtolist(i,j,0,liste,plateau);
                }
            }
    }
}
return;}


