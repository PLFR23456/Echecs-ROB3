#include <stdio.h>
#include <stdlib.h>
#include "piece.h"
#include "plateau.h"
#include "menu.h"
#include "calculus.h"
#include "misc.h"
enum cas{RIEN, PIONTEAM0MANGER, PIONTEAM1MANGER};
enum cas2{RAS, PRISEENPASSANT};


int verificationdeplacementmenantalechec(int xarrive, int yarrive, int info, piece plateau[TAILLE][TAILLE], deplacement depart);
void removeimpossiblemove(deplacement* listemove, deplacement depart, piece plateau[TAILLE][TAILLE]);
int estdanslaliste(int dep, deplacement* liste);




int applymove(piece plateau[TAILLE][TAILLE], deplacement depart, deplacement arrivee, int test, int* scorewhite, int* scoreblack, int* mortsnoirs, int* mortsblancs){
    //les coups sont donnes de la maniere suivant : depart:xy et arrivee:XY
    // x et X représentent la ligne de depart/arrivée
    // et y Y la colonne
    int val=RIEN;
    int x=depart.x;
    int y=depart.y;
    int X=arrivee.x;
    int Y=arrivee.y;
    /*
    if(plateau[X][Y].couleur==0){val=10;} //pion team 0 mangé
    if(plateau[X][Y].couleur==1){val=20;} //pion team 1 mangé
    if(plateau[x][y].rang!=-1){val=val+plateau[x][y].rang;} //si le pion de départ n'est pas une case vide, alors on ajoute sa valeur à val
    */
    if(test==0){ // test =0 signifie que le mouvement va être appliqué définitivement (à contrario des appels de cette fonction lors d'une détection d'échec)
        if(plateau[X][Y].couleur==0){(*mortsnoirs)++; (*scorewhite)=(*scorewhite)+valeur_piece(plateau[X][Y].rang);} //pion team 0 mangé
        if(plateau[X][Y].couleur==1){(*mortsblancs)++; (*scoreblack)=(*scoreblack)+valeur_piece(plateau[X][Y].rang);} //pion team 1 mangé
    }
    plateau[X][Y].rang=plateau[x][y].rang;
    plateau[X][Y].couleur=plateau[x][y].couleur;
    plateau[X][Y].opt=plateau[x][y].opt;
    plateau[x][y].rang=-1;
    plateau[x][y].couleur=-1;
    plateau[x][y].opt=-1;
    if(plateau[X][Y].opt==1 && test==0){
        if(plateau[X][Y].rang==PION || plateau[X][Y].rang==TOUR){plateau[X][Y].opt=2;}
        else{plateau[X][Y].opt=0;}
    }
    
    return val;
}
int verifs(partie* p,int couleur,int infoatransmettre){
    
    //infoatransmettre :
    //0 : la totale (promotion)
    if(infoatransmettre==0){
    for(int i=0;i<TAILLE;i++){
        for(int j=0;j<TAILLE;j++){
            if(p->plateau[i][j].opt==2 && p->plateau[i][j].couleur==couleur){p->plateau[i][j].opt=0;}
            if(p->plateau[i][j].rang==PION && p->plateau[i][j].couleur==couleur && i==7*(1-couleur)){
                if(p->niveauIA==0 || couleur==0){
                    deplacement montrer={i,j,-2};
                    affiche(p->plateau,&montrer);
                    promotion(i, j, p->plateau); //promotion !!
                }
                else{
                    int r = rand()%4;
                    p->plateau[i][j].rang=r+1; // 0= pion, 1à4 = CAVALIER,FOU... 5=ROI
                }
            }
        }
    }
    }
    return 0;
}

deplacement *possiblemove(piece plateau[TAILLE][TAILLE], deplacement depart){
    deplacement* head=(deplacement*)malloc(sizeof(deplacement));
    head->info=-1;
    int x=depart.x;
    int y=depart.y;
    int r=plateau[x][y].rang;
    int color=plateau[x][y].couleur;
    int opt=plateau[x][y].opt;
    /*if(opt==3){
        if(color==1){color=0;}
        else{color=1;}//inverser les couleurs
    }*/ //FINALEMENT il n'est pas utile dinverser le sens de déplacement pour un pion promu car les 4 pièces possibles offrent des déplacements dans toutes les directions. 

    switch (r)
    {
    case PION:{
        if(opt==1){if(plateau[x+(2-4*color)][y].couleur==-1){addtolist(x+(2-4*color), y, RAS, head,plateau);}}
        if(x<TAILLE-1 && x>0){if(plateau[x+(1-2*color)][y].couleur==-1){addtolist(x+(1-2*color), y, RAS, head,plateau);}}
        //regarde la case devant et a gauche/droite
        if(y<TAILLE-1){if(plateau[x][y+1].opt==2 && plateau[x][y+1].couleur!=color){addtolist(x, y+1, RAS, head,plateau);}}; //prise en passant
        if(y>0){if(plateau[x][y-1].opt==2 && plateau[x][y-1].couleur!=color){addtolist(x, y-1, RAS, head,plateau);}}; //prise en passant
        if(y<TAILLE-1 && x>0 && x<TAILLE-1){if(plateau[x+(1-2*color)][y+1].couleur!=color && plateau[x+(1-color*2)][y+1].couleur!=-1){addtolist(x+(1-color*2), y+1, RAS, head,plateau);}};
        if(y>0 && x>0 && x<TAILLE-1){if(plateau[x+(1-2*color)][y-1].couleur!=color && plateau[x+(1-color*2)][y-1].couleur!=-1){addtolist(x+(1-color*2), y-1, RAS, head,plateau);}};
        break;}
    case TOUR:{
        for(int i=x+1;i<TAILLE;i++){
            if(plateau[i][y].rang!=-1){if(plateau[i][y].couleur!=color){addtolist(i, y, RAS, head,plateau);break;}else{break;}}
            addtolist(i, y, RAS, head,plateau); }
        for(int i=x-1;i>-1;i--){
            if(plateau[i][y].rang!=-1){if(plateau[i][y].couleur!=color){addtolist(i, y, RAS, head,plateau);break;}else{break;}}
            addtolist(i, y, RAS, head,plateau); }
        for(int i=y+1;i<TAILLE;i++){
            if(plateau[x][i].rang!=-1){if(plateau[x][i].couleur!=color){addtolist(x, i, RAS, head,plateau);break;}else{break;}}
            addtolist(x, i, RAS, head,plateau); }
        for(int i=y-1;i>-1;i--){
            if(plateau[x][i].rang!=-1){if(plateau[x][i].couleur!=color){addtolist(x, i, RAS, head,plateau);break;}else{break;}}
            addtolist(x, i, RAS, head,plateau); }
        break;}
    case FOU:{
        for(int a=0;a<4;a++){
        int i=1,j=1;
        while((int)x+i-(2*i*(int)(a/2))<TAILLE && (int)x+i-(2*(int)(a/2))*i>-1 && (int)y+j-(2*(a%2))*j<TAILLE && (int)y+j-(2*(a%2))*j>-1){
            if(plateau[(int)x+i-(2*(int)(a/2))*i][(int)y+j-(2*(int)(a%2))*j].rang!=-1){
                if(plateau[(int)x+i-(2*(int)(a/2))*i][(int)y+j-(2*(int)(a%2))].couleur!=color){
                    addtolist((int)(x+i-(2*(int)(a/2))*i), (int)(y+j-(2*(int)(a%2))*j), RAS, head,plateau);
                }
                break;}
            else{addtolist((int)(x+i-(2*(int)(a/2))*i), (int)(y+j-(2*(int)(a%2))*j), RAS, head,plateau);}
            j++;
            i++;
        }}
        break;}
    case DAME:{
        for(int a=0;a<4;a++){
        int i=1,j=1;
        while(x+i-(2*i*(int)(a/2))<TAILLE && x+i-(2*(int)(a/2))*i>-1 && y+j-(2*(int)(a%2))*j<TAILLE && y+j-(2*(int)(a%2))*j>-1){
            if(plateau[x+i-(2*(int)(a/2))*i][y+j-(2*(int)(a%2))*j].rang!=-1){
                if(plateau[x+i-(2*(int)(a/2))*i][y+j-(2*(int)(a%2))].couleur!=color){
                    addtolist((int)(x+i-(2*(int)(a/2))*i), (int)(y+j-(2*(int)(a%2))*j), RAS, head,plateau);
                }
                break;}
            else{addtolist((int)(x+i-(2*(int)(a/2))*i), (int)(y+j-(2*(int)(a%2))*j), RAS, head,plateau);}
            j++;
            i++;
        }}
        for(int i=x+1;i<TAILLE;i++){
            if(plateau[i][y].rang!=-1){if(plateau[i][y].couleur!=color){addtolist(i, y, RAS, head,plateau);break;}else{break;}}
            addtolist(i, y, RAS, head,plateau); }
        for(int i=x-1;i>-1;i--){
            if(plateau[i][y].rang!=-1){if(plateau[i][y].couleur!=color){addtolist(i, y, RAS, head,plateau);break;}else{break;}}
            addtolist(i, y, RAS, head,plateau); }
        for(int i=y+1;i<TAILLE;i++){
            if(plateau[x][i].rang!=-1){if(plateau[x][i].couleur!=color){addtolist(x, i, RAS, head,plateau);break;}else{break;}}
            addtolist(x, i, RAS, head,plateau); }
        for(int i=y-1;i>-1;i--){
            if(plateau[x][i].rang!=-1){if(plateau[x][i].couleur!=color){addtolist(x, i, RAS, head,plateau);break;}else{break;}}
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
void listedemespions(piece plateau[TAILLE][TAILLE],int couleur, deplacement *liste, int niveauIA){
    //parcourir le plateau
    int nvmax=0;                                          //CARACTERISTIQUES voir ci dessous   
    for(int i=0;i<TAILLE;i++){
        for(int j=0;j<TAILLE;j++){
            if(plateau[i][j].couleur==couleur){ //          si cest un pion de ma couleur
                deplacement dep={i,j};
                deplacement *listemove= possiblemove(plateau,dep);
                if(listemove->info!=-1){ //                 + si ce pion a 1 ou + déplacement dispos 
                    removeimpossiblemove(listemove,dep,plateau);
                    int ajoutable=0;
                    if(listemove->info!=-1){
                        if(niveauIA>=2){  //            + si le pion a 1 ou + déplacement dispos (après vérif echec et mat)
                            deplacement* adresse = listemove;
                            if(niveauIA>=2){
                                while (adresse != NULL) {
                                    //// si nous sommes dans le cas d'une IA (qui cherche a avoir le meilleur coup possible)////
                                    if(adresse->info>=nvmax){
                                        if(adresse->info>nvmax){
                                            if(niveauIA==2){
                                                ajoutable=1;
                                                if(nvmax!=10){
                                                    nvmax=10;
                                                    freelist(liste->next);
                                                    liste->info=nvmax;
                                                    liste->x=i;
                                                    liste->y=j;
                                                    liste->next=NULL;
                                                }
                                            }
                                            else{
                                            nvmax=adresse->info;
                                            freelist(liste->next);
                                            liste->info=nvmax;
                                            liste->x=i;
                                            liste->y=j;
                                            liste->next=NULL;
                                            }
                                        }
                                        else{ajoutable=1;}
                                    }
                                    adresse = adresse->next;
                                }
                                
                            }
                        }
                        else{ajoutable=1;}
                        
                    }
                    if(ajoutable==1){addtolist(i,j,nvmax,liste,plateau);}
                }
            }
        }
    }
    return;
 }
deplacement* sendattackpossibilites(piece tableau[TAILLE][TAILLE], deplacement* liste, int nvIA) {
    if (!liste) return NULL;

    deplacement* newliste = NULL; // Pointeur pour la nouvelle liste
    deplacement** dernier = &newliste; // Pointeur pour suivre le dernier élément ajouté
    int nvmax = 0;

    deplacement* adresse = liste; // Curseur sur la liste existante
    while (adresse != NULL) {
        if (adresse->info >= 10) { // Si le déplacement implique une attaque
            if (nvIA >= 2) {
                if ((adresse->info > nvmax && nvIA>2) || (nvIA==2 && nvmax<10)) {
                    // On remplace la liste existante par une nouvelle liste pour ce niveau
                    freelist(newliste);
                    newliste = NULL;
                    dernier = &newliste;
                    nvmax = adresse->info;
                    if(nvIA==2){nvmax=10;}
                }
                if (adresse->info == nvmax) {
                    // Ajout d'un nouveau déplacement à la liste
                    *dernier = (deplacement*)malloc(sizeof(deplacement));
                    if (!(*dernier)) {
                        perror("Erreur d'allocation mémoire");
                        freelist(newliste);
                        return NULL;
                    }
                    (*dernier)->x = adresse->x;
                    (*dernier)->y = adresse->y;
                    (*dernier)->info = adresse->info;
                    (*dernier)->next = NULL;
                    dernier = &((*dernier)->next);

                }
            }
        }
        adresse = adresse->next;
    }

    // Si aucune attaque détectée, on retourne la liste d'origine
    if (!newliste) {
        printf("aucune attaque détectée\n");
        return liste;
    }

    // Libération de la liste d'origine
    freelist(liste);
    return newliste;
}

void removeimpossiblemove(deplacement* listemove, deplacement depart, piece plateau[TAILLE][TAILLE]){
    deplacement* adresse = listemove;
    deplacement* precedent=NULL;
    while (adresse != NULL) {
        if (verificationdeplacementmenantalechec(adresse->x,adresse->y,0,plateau,depart)==1) {
            if (adresse->next == NULL && precedent == NULL) {
                // Si c'est le dernier élément et qu'il n'y a pas de précédent (liste avec 1 seul élément)
                adresse->info = -1;
                return;
            } else if (adresse->next == NULL) {
                // Si c'est le dernier élément mais il y a un précédent
                precedent->next = NULL;
                return;
            } else if (precedent == NULL) {
                // Si on supprime le premier élément
                *listemove = *(adresse->next);  // Copie des données du suivant dans la tête    // Libération de l'ancien suivant
                adresse = listemove;
            } else {
                // Si on supprime un élément intermédiaire
                precedent->next = adresse->next;
                adresse = precedent->next;
            }

        }
        else{precedent = adresse;
            adresse = adresse->next;
}
    }
}
int roienechec(piece plateau[TAILLE][TAILLE], deplacement monroi){
    int couleurdemonroi=plateau[monroi.x][monroi.y].couleur;
    for(int i=0;i<TAILLE;i++){
        for(int j=0;j<TAILLE;j++){
            if(plateau[i][j].couleur==1-couleurdemonroi){ //1- couleur de mon roi = couleur complementaire
                //je regarde la liste de ses coups dispo
                deplacement dep={i,j};
                deplacement* liste=possiblemove(plateau,dep);
                deplacement* adresse = liste;
                if(liste->info==-1){continue;} // regarder si cette ligne est enlevable
                while (adresse != NULL) {
                    if (adresse->x == monroi.x && monroi.y == adresse->y) {
                        return(1); //mon roi est dans la liste des coups adverses!
                    }
                    adresse = adresse->next;
                }

            }
        }
    }
    return 0;
}
int verificationdeplacementmenantalechec(int xarrive, int yarrive, int info, piece plateau[TAILLE][TAILLE], deplacement depart) {
    //simulation
    piece temp={plateau[xarrive][yarrive].rang,plateau[xarrive][yarrive].couleur,plateau[xarrive][yarrive].opt};
    int couleur = plateau[depart.x][depart.y].couleur;
    deplacement arrivee={xarrive,yarrive};
    applymove(plateau,depart,arrivee,1,0,0,0,0);
    //
    int roix=0;
    int roiy=0;
    int echec=0;
    while(plateau[roix][roiy].couleur!=couleur || plateau[roix][roiy].rang!=ROI){roix++;if(roix==TAILLE){roiy++;roix=0;}}
    deplacement roi={roix,roiy};
    echec=roienechec(plateau, roi);
    // remise en ordre
    applymove(plateau,arrivee,depart,1,0,0,0,0);
    plateau[xarrive][yarrive].couleur=temp.couleur;
    plateau[xarrive][yarrive].rang=temp.rang;
    plateau[xarrive][yarrive].opt=temp.opt;
    //
    return echec;
}


int estunvraicoup(int coup,deplacement *liste, int* infosurlecoup){
    deplacement* adresse = liste;
    if(liste->info==-1){return 0;} //aucun coup dispo pas besoin de cette ligne si on verifie avant quaucun coup est dispo
    while (adresse != NULL) {
        if (adresse->x == coup/10 && coup%10 == adresse->y) {
            if(adresse->info==-2){return 0;}
            *infosurlecoup=adresse->info;
            return(1);
        }
        adresse = adresse->next;
    }
    return -1; //coup pas dispo
}
int estdanslaliste(int dep, deplacement* liste){
    deplacement* adresse = liste;
    if(adresse->info==-1){return 0;}
    while (adresse != NULL) {
        if (dep/10==adresse->x && dep%10 == adresse->y) {
            return 1; //si le deplacement "dep" est dans la liste de deplacements "liste"
        }
        adresse=adresse->next;
    }return 0;

}



void addtolist(int x, int y, int info,deplacement* head, piece plateau[TAILLE][TAILLE]) {
    
    
    deplacement* noeud = (deplacement*)malloc(sizeof(deplacement));
    if (noeud == NULL) {
        printf("Erreur d'allocation mémoire\n");
        exit(1);
    }
    
    noeud->x = x;
    noeud->y = y;

    //transmission de l'information : quel pièce est mangée :
    if(info!=-2 && plateau[x][y].rang!=-1){ //si ce n'est ni une case vide, ni un appel de cette fonction pour colorier notre propre pion en orange (voir affiche)
        info=10 + plateau[x][y].couleur * 10 + plateau[x][y].rang;}
    //transmission de l'information : quel pièce est mangée

    noeud->info = info;
    noeud->next = NULL;
    if(head->info==-1){
        head->x = noeud->x;
        head->y = noeud->y;
        head->info = noeud->info;
        head->next = NULL;
        free(noeud);
        return;
    }

    deplacement* adresse=head;
    while(adresse->next!=NULL){
        adresse=adresse->next;
    }
    adresse->next=noeud;
    return;
}
void freelist(deplacement *liste){
    if(liste==NULL || liste->info==-1){return;}
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
void lireliste(deplacement* liste){
    deplacement* adresse = liste;
    if(adresse->info==-1){printf("liste nulle");return;}
    while (adresse != NULL) {
        printf("'%d'%d %d %p'",adresse->x,adresse->y,adresse->info,adresse->next);
        adresse=adresse->next;
    }

}















