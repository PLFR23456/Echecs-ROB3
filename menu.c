#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "jeu.h"
#include "temps.h"
#include "menu.h"
#include "piece.h"
#include "plateau.h"
void enregistrerPartie(partie *p) {
    //Ouvre le fichier save en mode binaire + mode append
    FILE *fichier = fopen("save", "ab");
    
    if(fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier"); //perror sert à donner la raison de lerreur
        return;
    }
    size_t taille=sizeof(partie);
    if(fwrite(p,taille,1,fichier) != 1) { // fwrite(adresseelementaecrire, taille de celui ci, nb delements, fichier)
        perror("Erreur lors de l'écriture dans le fichier");
    } else {
        printf("Partie sauvegardee avec succes.\n");
    }
    
    //Ferme le fichier
    fclose(fichier);
}
void remplacerOuAjouterPartie(partie *p) {
    FILE *fichier = fopen("save", "r+b");
    
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }
    
    size_t taille_partie = sizeof(partie);
    int numero_partie = p->numpartie;
    long position = -1;

    // Parcourir le fichier pour vérifier si la partie existe déjà
    while (1) {
        long current_pos = ftell(fichier); // Sauvegarder la position actuelle
        partie temp_partie;
        
        // Lire une partie à la fois
        size_t read_size = fread(&temp_partie, taille_partie, 1, fichier);
        
        // Si nous avons atteint la fin du fichier, sortir de la boucle
        if (read_size == 0) {
            break;
        }

        // Si l'identifiant de la partie correspond, enregistrer la position
        if (temp_partie.numpartie == numero_partie) {
            position = current_pos;
            break;
        }
    }

    // Si la partie existe déjà, la remplacer
    if (position != -1) {
        fseek(fichier, position, SEEK_SET);
        fwrite(p, taille_partie, 1, fichier);
        printf("Partie %d mise à jour avec succès.\n", numero_partie);
    } else {
        // Si la partie n'existe pas, l'ajouter à la fin du fichier
        enregistrerPartie(p);
    }

    fclose(fichier);
}
void afficherStatistiques() {
    clearecran();
    printf("\n=== Statistiques ===\n");

    // Ouvrir le fichier des parties
    FILE *fichier = fopen("save", "rb");
    if (fichier == NULL) {
        perror("Erreur d'ouverture du fichier");
        attendre(2);
        afficherMenu();
        return;
    }

    size_t taillePartie = sizeof(partie);
    fseek(fichier, 0, SEEK_END);
    long tailleFichier = ftell(fichier);
    int nbParties = tailleFichier / taillePartie;

    if (nbParties == 0) {
        printf("Aucune partie enregistrée.\n");
        fclose(fichier);
        attendre(2);
        afficherMenu();
        return;
    }

    // Allouer de la mémoire pour stocker les parties
    partie *parties = (partie *)malloc(nbParties * taillePartie);
    if (parties == NULL) {
        perror("Erreur d'allocation mémoire");
        fclose(fichier);
        afficherMenu();
        return;
    }

    rewind(fichier);
    fread(parties, taillePartie, nbParties, fichier);
    fclose(fichier);

    // Statistiques cumulées
    int totalCoupsJoues = 0;
    int totalTemps = 0;
    int totalMortsBlancs = 0;
    int totalMortsNoirs = 0;
    int totalScoreWhite = 0;
    int totalScoreBlack = 0;

    // Afficher les statistiques de chaque partie
    for (int i = 0; i < nbParties; i++) {
        printf("\nPartie %d :\n", i + 1);
        printf("Nom : %s\n", parties[i].nom);
        printf("Coups joués : %d\n", parties[i].coupsjoues);
        printf("Morts blancs : %d\n", parties[i].mortsblancs);
        printf("Morts noirs : %d\n", parties[i].mortsnoirs);
        printf("Score blancs : %d\n", parties[i].scorewhite);
        printf("Score noirs : %d\n", parties[i].scoreblack);
        printf("Temps restant pour les noirs : %d\n", TEMPS-parties[i].tempsteam0);
        printf("Temps restant pour les blancs : %d\n", TEMPS-parties[i].tempsteam1);
        if(parties[i].etat == 2){
        printf("Gagnant : %s\n", parties[i].gagnant == 0 ? "Joueur 0" : parties[i].gagnant == 1 ? "Joueur 1" : "Nul/Pat");}

        // Ajouter aux statistiques cumulées
        totalCoupsJoues += parties[i].coupsjoues;
        totalTemps += parties[i].tempsteam0 +parties[i].tempsteam1;
        totalMortsBlancs += parties[i].mortsblancs;
        totalMortsNoirs += parties[i].mortsnoirs;
        totalScoreWhite += parties[i].scorewhite;
        totalScoreBlack += parties[i].scoreblack;
    }

    // Afficher les statistiques cumulées
    printf("\n=== Statistiques cumulées ===\n");
    printf("Total coups joués : %d\n", totalCoupsJoues);
    printf("Total temps écoulé : %ds\n", totalTemps);
    printf("Total morts blancs : %d\n", totalMortsBlancs);
    printf("Total morts noirs : %d\n", totalMortsNoirs);
    printf("Total score blancs : %d\n", totalScoreWhite);
    printf("Total score noirs : %d\n", totalScoreBlack);

    free(parties);

    // Retour au menu principal
    printf("\n1. Retour au menu principal\n");
    printf("Votre choix : ");
    int choix;
    scanf("%d", &choix);
    viderTampon();

    if (choix == 1) {
        afficherMenu();
    } else {
        afficherStatistiques();
    }
}


void viderTampon() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF); // Vide tout jusqu'au prochain saut de ligne
}


void page1();
void page11();
void page12(partie *nv);
void page121(partie *nv);
void page122(partie *nv);
void page123(partie *nv);
void page13();

char nomfichier[TAILLENOMPARTIE]="save";

void afficherMenu() {
    clearecran();
    int choix;
    printf("\n=== Menu Interactif ===\n");
    printf("1. Parties\n");
    printf("2. Voir les statistiques\n");
    printf("3. Quitter\n");
    printf("\nVotre choix : ");
    scanf("%d", &choix);
    viderTampon();
    switch(choix)
    {
    case 1:
        page1();
        return;
    case 2:
        afficherStatistiques();
        return;
    case 3:
    attendre(0.5);
        printf("\t\t\t\t\t\t bye.\n");
        attendre(0.5);
        clearecran();
        return;
    default:
        afficherMenu();
        return;
    }
}

void page1(){
    clearecran();
    printf("\n=== Parties ===\n");
    printf("1. Choisir une partie existante\n");
    printf("2. Nouvelle partie\n");
    printf("3. Supprimer une partie\n");
    printf("4. Quitter\n");
    printf("\nVotre choix : ");
    int choix;
    scanf("%d", &choix);
    viderTampon();
    switch(choix)
    {
    case 1:
        page11();
        return;
    case 2:{
        partie nv={0};
        initialise_plateau(nv.plateau);  
        nv.numpartie=rand()%100;//creation du nouvel échiquier
        //determiner le numero de la partie puis lui attribuer probleme si on supprime la partie tous les numeros décalés
        // lui attribuer un identifiant non attribué
        page12(&nv);
        return;
        }
    case 3:{
        attendre(0.5);
        printf("\t\t\t\t\t\t ATTENTION.\n");
        printf("\033[1m/.\\\033[0m\n");
        attendre(0.5);
        clearecran();
        page13();
        return;
    }
    default:
        afficherMenu();
        return;
    }
}

void page11(){
    clearecran();
    printf("\n=== Liste des parties : selectionnez-en une : ===\n");
    printf("1. Quitter\n");
    FILE *fichier= fopen(nomfichier,"r");
    if (fichier == NULL) {
        perror("Erreur d'ouverture du fichier");
        return;
    }

    size_t taillePartie = sizeof(partie);
    fseek(fichier, 0, SEEK_END);
    long tailleFichier = ftell(fichier);
    int nbParties = tailleFichier / taillePartie;

    partie* parties = (partie *)malloc(nbParties * taillePartie);
    if (parties == NULL) {
        perror("Erreur d'allocation mémoire");
        fclose(fichier);
        free(parties);
        page1();
        return;
    }
    rewind(fichier);
    fread(parties, taillePartie, nbParties, fichier);
    fclose(fichier);
    for(int i=0;i<nbParties;i++){
        printf("%d. %s \t\t time: %ds.\n",i+2,parties[i].nom,parties[i].tempsteam0);
    }
    int index=1;
    scanf("%d",&index);
    viderTampon();
    if(index==1){free(parties);
        page1();
        return;}
    if (index < 2 || index > nbParties+1) {
        printf("Index de partie invalide.\n");
        free(parties);
        attendre(2);
        page1();
        return;
    }

    partie current = parties[index - 2];
    free(parties);
    jeu(&current);
    afficherMenu();


}

void page12(partie *nv){
    clearecran();
    printf("\n=== Creation d'une nouvelle partie ===\n");
    printf("1. Nom  :  %s\n",((nv->nom)));
    printf("2. Niveau IA  :  %d\n",(nv->niveauIA));
    printf("3. Aide à la visée ?  :  %d\n",(nv->aidealavisee));
    printf("4. ?Tableau personnalisé?\n");
    printf("5. LANCER PARTIE\n");
    printf("6. Quitter\n");
    printf("\nVotre choix : ");
    int choix;
    scanf("%d", &choix);
    viderTampon();
    switch(choix)
    {
    case 1:
        page121(nv);
        return;
    case 2:
        page122(nv);
        return;
    case 3:
        page123(nv);
        return;
    case 4:
        creer_partie_personnalisee(nv->plateau);
    case 5:
        enregistrerPartie(nv);
        jeu(nv);
        afficherMenu();
        return;
    default:
        afficherMenu();
        return;
    }
}

void page13() {
    clearecran();
    printf("\n=== Liste des parties : sélectionnez-en une à supprimer ===\n");
    printf("1. Retour\n");

    // Ouvrir le fichier des parties
    FILE *fichier = fopen(nomfichier, "r");
    if (fichier == NULL) {
        perror("Erreur d'ouverture du fichier");
        attendre(2);
        page1();
        return;
    }

    size_t taillePartie = sizeof(partie);
    fseek(fichier, 0, SEEK_END);
    long tailleFichier = ftell(fichier);
    int nbParties = tailleFichier / taillePartie;

    if (nbParties == 0) {
        printf("Aucune partie à supprimer.\n");
        fclose(fichier);
        attendre(2);
        page1();  // Retourner à la page d'accueil
        return;
    }

    // Allouer de la mémoire pour stocker les parties
    partie *parties = (partie *)malloc(nbParties * taillePartie);
    if (parties == NULL) {
        perror("Erreur d'allocation mémoire");
        fclose(fichier);
        page1();  // Retourner à la page d'accueil en cas d'erreur
        return;
    }

    rewind(fichier);
    fread(parties, taillePartie, nbParties, fichier);
    fclose(fichier);

    // Afficher la liste des parties avec leurs indices
    for (int i = 0; i < nbParties; i++) {
        printf("%d. %s \t\t time: %ds.\n", i + 2, parties[i].nom, parties[i].tempsteam0);
    }

    int index;
    printf("\nEntrez le numéro de la partie à supprimer (ou 1 pour retourner) : ");
    scanf("%d", &index);
    viderTampon();  // Vider le tampon d'entrée

    // Si l'utilisateur choisit de revenir en arrière
    if (index == 1) {
        free(parties);
        page1();  // Retourner à la page 1
        return;
    }

    // Vérifier si l'index est valide
    if (index < 2 || index > nbParties + 1) {
        printf("Index de partie invalide.\n");
        free(parties);
        attendre(2);
        page13();  // Retourner à la page13 pour réessayer
        return;
    }

    // Supprimer la partie choisie directement dans le fichier
    FILE *tempFichier = fopen("temp_save", "wb");
    if (!tempFichier) {
        perror("Erreur lors de la création d'un fichier temporaire");
        free(parties);
        return;
    }

    // Réécrire toutes les parties sauf celle à supprimer
    for (int i = 0; i < nbParties; i++) {
        if (i != index - 2) {  // index de l'utilisateur commence à 2, donc on décale
            fwrite(&parties[i], taillePartie, 1, tempFichier);
        }
    }

    // Fermer les fichiers
    fclose(tempFichier);
    free(parties);

    // Remplacer le fichier original par le fichier temporaire
    remove(nomfichier);
    rename("temp_save", nomfichier);
    printf("Partie supprimée avec succès.\n");

    attendre(2);  // Attendre avant de revenir à la page d'accueil
    page1();  // Retourner à la page 1
}

void page121(partie *nv){
    clearecran();
    char nom[TAILLENOMPARTIE]="";
    printf("Tapez le nom puis pressez entrée :\n\n");
    scanf("%s",nom);
    viderTampon();
    int i=0;
    while (nom[i] != '\0' && i < TAILLENOMPARTIE - 1) {
        nv->nom[i] = nom[i];
        i++;}
    nv->nom[i]='\0';
    clearecran();
    page12(nv); //retourne au menu en actualisant la nouvelle partie (essentiel pour laffichage)
    return;
}

void page122(partie *nv){
    clearecran();
    int n=0;
    printf("Détails :\n\tNv 0 = vous jouez contre vous-même (pas d'IA)\n\tNv 1 = IA fait des mouvements randoms mais valables\n\tNv 2 = si l'IA peut attaquer, elle attaquera\n\tNv 3 = si l'IA peut attaquer plusieurs pions, elle attaquera le meilleur\n\n");
    printf("Tapez le niveau de l'IA :\n\n");
    scanf("%d",&n);
    viderTampon();
    nv->niveauIA=n;
    clearecran();
    page12(nv); //retourne au menu en actualisant la nouvelle partie (essentiel pour laffichage)
    return;
}
void page123(partie *nv){
    clearecran();
    char n='z';
    printf("Voulez vous l'aide à la visee ? Oui [O] ou Non [N] (voir aide pour + de détails) :\n\n");
    scanf("%c",&n);
    viderTampon();
    if(n=='O' || n == 'o'){
        nv->aidealavisee=1;
    }
    else{nv->aidealavisee=0;}
    clearecran();
    page12(nv); //retourne au menu en actualisant la nouvelle partie (essentiel pour laffichage)
    return;
}



int main(){
    srand(time(NULL));
    afficherMenu();
    return 0;
}

//A


