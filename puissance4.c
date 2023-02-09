/**
 * @file    puissance4.c
 * @brief   Programme "Puissance 4"
 * @author  Justine Verger, 1D2
 * @version 1.0
 * @date    Novembre 2022
 * 
 * Programme qui permet à deux utilisateurs de s'affronter dans une partie de Puissance 4.
 * 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/**
 * \include     permet d'utiliser des pseudo-booléens
*/
#include <stdbool.h>
/**
 * \include     passée en paramètres de srand(), permet de générer un tirage aléatoire
*/
#include <time.h>
/**
 * \include     permet d'utiliser toupper() pour mettre la première lettre des noms des joueurs en majuscule
*/
#include <ctype.h>
/**
 * \include     permet d'utiliser sleep(x), pour afficher un message d'erreur à l'écran pendant x seconde(s)
*/
#include <unistd.h>

/** 
 * \def     nombre de lignes dont dispose la grille du jeu
*/
#define NBLIG 6
/** 
 * \def     nombre de colonnes dont dispose la grille du jeu
*/
#define NBCOL 7
/** 
 * \def     valeur donnée à une cellule, si on y place un jeton jaune
*/
#define PION_J 'J'
/** 
 * \def     valeur donnée à une cellule, si on y place un jeton rouge
*/
#define PION_R 'R'
/** 
 * \def     valeur donnée à une cellule, tant qu'un pion n'y a pas été placé
*/
#define VIDE ' '
/** 
 * \def     valeur donnée à 'vainqueur', tant que la partie n'est pas remportée
*/
#define INCONNU ' '

/** 
 * \typedef char Grille[NBLIG][NBCOL]
*/
typedef char Grille[NBLIG][NBCOL];

/********************************************************************************
*                           DÉCLARATION DES CONSTANTES                          *
*********************************************************************************/

/**
 * \def     int COLONNE_DEBUT = NBCOL / 2
 * \brief   correspond à la colonne du milieu de grille
*/
const int COLONNE_DEBUT = NBCOL / 2;
/**
 * \def     char PIECE_JAUNE[] = "🟡";
 * \brief   affichage de la pièce jaune
*/
const char PIECE_JAUNE[] = "🟡";
/**
 * \def     char PIECE_ROUGE[] = "🔴"
 * \brief   affichage de la pièce rouge
*/
const char PIECE_ROUGE[] = "🔴";
/**
 * \def     char CELLULE_VIDE[] = "  "
 * \brief   affichage du contenu d'une cellule vide
*/
const char CELLULE_VIDE[] = "  ";
/**
 * \def     int LONGUEUR_MAX_NOM = 15 
 * \brief   nombre maximum de caractères acceptés pour les noms des deux joueurs
*/
const int LONGUEUR_MAX_NOM = 15;
/**
 * \def     int LONGUEUR_MAX_COULEUR = 5
 * \brief   nombre maximum de caractères acceptés pour les pièces (les émoticônes des pièces jaune et rouge équivalent à une longueur de 5 caractères)
*/
const int LONGUEUR_MAX_COULEUR = 5;
/**
 * \def     char GAUCHE = 'a'
 * \brief   touche de déplacement vers la gauche (pour un clavier QWERTY)
*/
// const char GAUCHE = 'a';
/**
 * \def     char GAUCHE = 'q'
 * \brief   touche de déplacement vers la gauche (pour un clavier AZERTY)
*/
const char GAUCHE = 'q';
/**
 * \def     char DROITE = 'd' 
 * \brief   touche de déplacement vers la droite
*/
const char DROITE = 'd';
/**
 * \def     char VALIDER = ' '
 * \brief   touche de validation du choix de colonne
*/
const char VALIDER = ' ';

/********************************************************************************
*                   DÉCLARATION DES PROCÉDURES ET FONCTIONS                     *
*********************************************************************************/

/**
 * \fn      void effacerFenetre(void)
 * \brief   Procédure qui vide la fenêtre du terminal
 * \return  void
*/
void effacerFenetre(void);

/**
 * \fn      void msgErrSaisie(void)
 * \brief   Procédure qui affiche un message d'erreur en cas de mauvaise saisie
 * \return  void
*/
void msgErrSaisie(void);

/**
 * \fn      void reglesDuJeu(void)
 * \brief   Procédure qui affiche les règles du jeu
 * \return  void
*/
void reglesDuJeu(void);

/**
 * \fn      void menuPrincipal(void)
 * \brief   Procédure qui affiche le menu d'accueil du jeu 
 * \return  void
*/
void menuPrincipal(void);

/**
 * \fn      bool partie(void)
 * \brief   Fonction qui contient le "corps" d'une partie
 * \return  booléen : 'true' si l'utilisateur relance une partie ; 'false' s'il arrête
*/
bool partie(void);

/**
 * \fn      void nomsJoueurs(char *, char *)
 * \brief   Procédure qui enregistre le nom des deux joueurs et la couleur qui leur est associée
 * @param   joueur_jaune[in, out] : chaîne, représente le nom du joueur jaune
 * @param   joueur_rouge[out], chaîne, représente le nom du joueur rouge
 * \return  void
*/
void nomsJoueurs(char *, char *);

/**
 * \fn      void quiCommence(bool, char *, char *, char *, char *, char *, char *, char *, char *)
 * \brief   Procédure qui reçoit le résultat du tirage et attribue les bonnes valeurs au joueur 1 et au joueur 2
 * \param   jaune_commence, booléen, entrée, représente le résultat du tirage : si 'true', c'est le joueur jaune qui commence, sinon c'est le joueur rouge
 * \param   joueur_jaune, chaîne, entrée/sortie, représente le prénom du joueur jaune
 * \param   joueur_rouge, chaîne, entrée/sortie, représente le prénom du joueur rouge
 * \param   j1, chaîne, entrée/sortie, représente le prénom du joueur 1
 * \param   j2, chaîne, entrée/sortie, représente le prénom du joueur 2
 * \param   couleur_j1, chaîne, entrée/sortie, représente la couleur des pièces du joueur 1
 * \param   couleur_j2, chaîne, entrée/sortie, représente la couleur des pièces du joueur 2
 * \param   piece_j1, caractère, entrée/sortie, caractère ('J' ou 'R') correspondant à la couleur du joueur
 * \param   piece_j2, caractère, entrée/sortie, caractère ('J' ou 'R') correspondant à la couleur du joueur
 * \return  void
*/
void quiCommence(bool, char *, char *, char *, char *, char *, char *, char *, char *);

/**
 * \fn      void choixTirage(char *, char *, char *, char *, char *, char *, char *, char *)
 * \brief   Procédure qui détermine qui des deux joueurs commencera la partie (choix ou tirage aléatoire)
 * \param   joueur_jaune, chaîne, entrée/sortie, représente le prénom du joueur jaune
 * \param   joueur_rouge, chaîne, entrée/sortie, représente le prénom du joueur rouge
 * \param   j1, chaîne, entrée/sortie, représente le prénom du joueur 1
 * \param   j2, chaîne, entrée/sortie, représente le prénom du joueur 2
 * \param   couleur_j1, chaîne, entrée/sortie, représente la couleur des pièces du joueur 1
 * \param   couleur_j2, chaîne, entrée/sortie, représente la couleur des pièces du joueur 2
 * \param   piece_j1, caractère, entrée/sortie, caractère ('J' ou 'R') correspondant à la couleur du joueur
 * \param   piece_j2, caractère, entrée/sortie, caractère ('J' ou 'R') correspondant à la couleur du joueur
 * \return  void
*/
void choixTirage(char *, char *, char *, char *, char *, char *, char *, char *);

/** 
 * \fn      void initGrille(Grille)
 * \brief   Procédure qui initialise la grille en affectant la constante VIDE à chacun de ses éléments
 * \param   g, Grille, entrée/sortie, représente la grille du jeu
 * \return  void
*/
void initGrille(Grille);

/**
 * \fn      void afficher(Grille, char, int, char *, char *, int, char)
 * \brief   Procédure qui réalise l'affichage à l'écran du contenu de la grille avec les pièces déjà jouées
 *          Affiche aussi la prochaine pièce à tomber : sera ré-affichée au-dessus de la colonne du milieu à chaque nouveau tour
 * \param   g, Grille, entrée/sortie, représente la grille du jeu
 * \param   pion, caractère, entrée, représente la pièce à afficher au-dessus de la grille
 * \param   colonne, entier, entrée, représente l'indice de la colonne au-dessus de laquelle la pièce doit être affichée
 * \param   jaune, chaîne, entrée/sortie, représente le prénom du joueur qui joue les pièces jaunes
 * \param   rouge, chaîne, entrée/sortie, représente le prénom du joueur qui joue les pièces rouges
 * \param   compteur_tours, entier, entrée, compte le nombre de tours de la partie
 * \param   vainqueur, caractère, entrée, correspond à la pièce gagnante ('J' ou 'R')
 * \return  void
*/
void afficher(Grille, char, int, char *, char *, int, char);

/**
 * \fn      bool grillePleine(Grille)
 * \brief   Fonction qui teste si toutes les cases de la grille sont occupées ou non
 * \param   g, Grille, entrée/sortie, représente la grille du jeu
 * \return  booléen : 'true' si la grille est remplie ; 'false' sinon
*/
bool grillePleine(Grille);

/**
 * \fn      int choisirColonne(Grille, char, int *, char *, char *, int, char)
 * \brief   Fonction qui permet de déplacer la pièce qui s'affiche en tête de la grille vers la gauche ou vers la droite
 * \param   g, Grille, entrée/sortie, représente la grille du jeu
 * \param   pion, caractère, entier, entrée, correspond à la pièce à jouer
 * \param   colonne, entier, entrée/sortie, représente la colonne au-dessus de laquelle se trouve la pièce
 * \param   jaune, chaîne, entrée/sortie, paramètre que l'on envoie à afficher(), représente le prénom du joueur qui joue les pièces jaunes
 * \param   rouge, chaîne, entrée/sortie, paramètre que l'on envoie à afficher(), représente le prénom du joueur qui joue les pièces rouges
 * \param   compteur_tours, entier, entrée, paramètre que l'on envoie à afficher(), compte le nombre de tours de la partie
 * \param   vainqueur, caractère, entrée, correspond à la pièce gagnante ('J' ou 'R')
 * \return  entier : représente l'indice de la colonne dans lequel le pion a été placé ; -1 si la colonne est pleine
*/
int choisirColonne(Grille, char, int *, char *, char *, int, char);

/**
 * \fn      int trouverLigne(Grille, int)
 * \brief   Fonction qui trouve la première cellule non occupée d'une colonne
 * \param   g, Grille, entrée/sortie, représente la grille du jeu
 * \param   c, entier, entrée, indice de la colonne dans laquelle la pièce doit tomber
 * \return  entier : indice de la ligne dans laquelle la pièce a été placée
*/
int trouverLigne(Grille, int);

/**
 * \fn      void jouer(Grille, char, int *, int *, char *, char *, int, char)
 * \brief   Procédure qui détermine la cellule à laquelle la pièce sera placée
 * \param   g, Grille, entrée/sortie, représente la grille du jeu
 * \param   pion, caractère, entier, entrée, correspond à la pièce à jouer
 * \param   ligne, entier, entrée/sortie, correspond à la ligne où est tombée la pièce
 * \param   colonne, entier, entrée/sortie, correspond à la colonne où est tombée la pièce
 * \param   jaune, chaîne, entrée/sortie, correspond au nom du joueur jaune
 * \param   rouge, chaîne, entrée/sortie, correspond au nom du joueur rouge
 * \param   compteur_tours, entier, entrée, paramètre que l'on envoie à colonne(), compte le nombre de tours de la partie
 * \param   vainqueur, caractère, entrée, correspond à la pièce gagnante ('J' ou 'R')
 * \return  void
*/
void jouer(Grille, char, int *, int *, char *, char *, int, char);

/**
 * \fn      bool estVainqueur(Grille, int, int)
 * \brief   Fonction qui 'scanne' la grille à la recherche d'un alignement gagnant
 * \param   g, Grille, entrée/sortie, représente la grille du jeu
 * \param   ligne, entier, entrée, représente la ligne à partir de laquelle rechercher une série de quatre pièces identiques successives
 * \param   colonne, entier, entrée, représente la colonne à partir de laquelle rechercher une série de quatre pièces identiques successives
 * \return  booléen : 'true' si un alignement gagnant a éte trouvé ; 'false' sinon
*/
bool estVainqueur(Grille, int, int);

/**
 * \fn      void finDePartie(char, char *, char *, int *, int *, int *)
 * \brief   Procédure qui affiche un message différent en fonction du résultat de fin de partie
 * \param   vainqueur, caractère, entrée, correspond à la pièce gagnante ('J' ou 'R')
 * \param   jaune, chaîne, entrée/sortie, correspond au nom du joueur jaune
 * \param   rouge, chaîne, entrée/sortie, correspond au nom du joueur rouge
 * \param   score_jaune, entier, entrée/sortie, correspond au score du joueur jaune
 * \param   score_rouge, entier, entrée/sortie, correspond au score du joueur rouge
 * \param   nb_nm, entier, entrée, correspond au nombre de match(es) nul(s)
 * \return  void
 * */
void finDePartie(char, char *, char *, int *, int *, int *);

/**
 * \fn      void afficherScores(char *, char *, int *, int *, int)
 * \brief   Procédure qui permet d'afficher les scores
 *          Se met à jour à chaque manche tant que l'utilisateur n'a pas choisi de réinitialiser les scores et de lancer une nouvelle partie (ie : nouveaux joueurs)
 * \param   jaune, chaîne, entrée/sortie, correspond au nom du joueur jaune
 * \param   rouge, chaîne, entrée/sortie, correspond au nom du joueur rouge
 * \param   score_jaune, entier, entrée/sortie, correspond au score du joueur jaune
 * \param   score_rouge, entier, entrée/sortie, correspond au score du joueur rouge
 * \param   nb_nm, entier, entrée, correspond au nombre de match(es) nul(s)
 * \return  void
*/
void afficherScores(char *, char *, int *, int *, int);

/**
 * \fn      void menuFinDePartie(char *, char *, int *, int *, int *, bool *, bool *)
 * \brief   Procédure qui affiche le menu de fin de partie
 * \param   jaune, chaîne, entrée/sortie, correspond au nom du joueur jaune
 * \param   rouge, chaîne, entrée/sortie, correspond au nom du joueur rouge
 * \param   score_jaune, entier, entrée/sortie, correspond au score du joueur jaune
 * \param   score_rouge, entier, entrée/sortie, correspond au score du joueur rouge
 * \param   nb_nm, entier, entrée/sortie, correspond au nombre de match(es) nul(s)
 * \param   quitter, booléen, entrée/sortie, si 'true' : met fin au programme ; si 'false' : continue à exécuter  
 * \param   nouvelle_partie, booléen, entrée/sortie, si 'true' : lance une partie avec de nouveaux joueurs ; si 'false' : conserve les noms et relance une partie
 * \return  void
*/
void menuFinDePartie(char *, char *, int *, int *, int *, bool *, bool *);

/*********************************************************************************
*                               PROGRAMME PRINCIPAL                              *
**********************************************************************************/

/**
 * \fn      int main(void)
 * \brief   Fonction principale
 * \return  int
*/
int main(void)
{
    srand(time(NULL));

    menuPrincipal();
    printf("\n\t👋 À bientôt !\n\n\n");

    return EXIT_SUCCESS;
}

/********************************************************************************
*                              PROCÉDURES ET FONCTIONS                          *
*********************************************************************************/

void effacerFenetre(void)
{
    system("@cls||clear");          // testé, fonctionne sur Mac ET Linux 
}

void msgErrSaisie(void)
{
    printf("\n\t\t🙁 Désolée, je n'ai pas compris ta réponse...\n\n");
    sleep(1);
}

void reglesDuJeu(void)
{
    effacerFenetre();
    int retour = -1;

    while (retour == -1)
    {
        printf("\t\t\t-------------------------------\n");
        printf("\t\t\t##### 📃 RÈGLES DU JEU 📃 #####\n");
        printf("\t\t\t-------------------------------\n\n");

        printf("➤ Nombre de joueurs :\n");
        printf("   Le Puissance 4 se joue à deux : un contre un.\n\n");

        printf("➤ But du jeu :\n");
        printf("   Être le premier à aligner 4 jetons de sa couleur sur la grille (horizontalement, verticalement ou en diagonal).\n\n");

        printf("➤ Déroulement d’une partie :\n");
        printf("   Chacun leur tour, les joueurs placent un jeton dans la colonne de leur choix. La partie se termine quand l’un des deux joueurs parvient à aligner 4 jetons de sa couleur, ou si les deux joueurs ont utilisé tous leurs jetons sans avoir réussi d’alignement gagnant (auquel cas, la partie est déclarée nulle).\n\n");

        printf("\n\n\t\t\t>> Retour au menu précédent (0)\n");
        printf("\t\t\t👉\t");

        scanf("%d", &retour);

        switch (retour)
        {
            case 0:     retour = 1;
                        effacerFenetre();
                        break;
            default:    msgErrSaisie();
                        printf("\t\tAppuie sur la touche '0' pour revenir au menu précédent ↩️\n\n");
                        sleep(2); 
                        effacerFenetre();
                        retour = -1;
                        break;
        }
    }
}

void menuPrincipal(void)
{
    effacerFenetre();

    int choix_menu = -1;
    while (choix_menu == -1)
    {
        printf("\t\t\t-------------------------------------------\n");
        printf("\t\t\t##### 🔥 BIENVENUE SUR PUISSANCE 4 🔥 #####\n");
        printf("\t\t\t-------------------------------------------\n\n");

        printf("\t+-------------------------------------------------------------------------------+\n");
        printf("\t|\t[i] Les chiffres entre parenthèses correspondent à la touche du clavier |\n\t|sur laquelle appuyer pour sélectionner une option affichée à l’écran. \t\t|\n\t|Par exemple : si tu veux consulter les règles du jeu, appuie sur la touche 2 de|\n\t|ton clavier, puis sur la touche “Entrée” pour confirmer.\t\t\t|\n");
        printf("\t+-------------------------------------------------------------------------------+\n\n");

        printf("\t\tQue veux-tu faire ? \n\n");
        printf("\t\t> Commencer une partie (1)\n");
        printf("\t\t> Lire les règles du jeu (2)\n");
        printf("\t\t> Quitter (0)\n\n");
        printf("\t\t👉\t");

        scanf("%d", &choix_menu);

        switch (choix_menu)
        {
            case 1: if (partie())
                    {
                        choix_menu = -1;
                    }
                    break;
            case 2: reglesDuJeu();
                    choix_menu = -1;
                    break;
            case 0: effacerFenetre();
                        while (choix_menu == 0)
                        {
                        printf("\n\t\tOn se quitte déjà...?\n\n");
                        printf("\t\t> ❌ Non, j'ai changé d'avis ! (0)\n");
                        printf("\t\t> ✅ Oui ! (1)\n");
                        printf("\t\t👉 ");
                        scanf("%d", &choix_menu);
                        switch (choix_menu)
                        {
                            case 0:     choix_menu = -1;
                                        effacerFenetre();
                                        break;
                            case 1:     break;
                            default:    msgErrSaisie();
                                        sleep(1); 
                                        effacerFenetre();
                                        choix_menu = 0;
                                        break;
                        }
                    }
                    break;
            default:    msgErrSaisie();
                        effacerFenetre();
                        choix_menu = -1;
                        break;
        }
    }
}

bool partie(void)
{
    Grille g;

    char jaune[LONGUEUR_MAX_NOM];               // prénom du joueur jaune   ex : "Justine"
    char rouge[LONGUEUR_MAX_NOM];               // prénom du joueur rouge   ex : "Arthur"
    char joueur1[LONGUEUR_MAX_NOM];             // prénom du joueur 1       ex : "Justine"
    char joueur2[LONGUEUR_MAX_NOM];             // prénom du joueur 2       ex : "Arthur"
    char couleur_j1[LONGUEUR_MAX_COULEUR];      // couleur du joueur 1      "🟡" ou "🔴"
    char couleur_j2[LONGUEUR_MAX_COULEUR];      // couleur du joueur 2      "🟡" ou "🔴"
    char piece_j1;                              //                          'J' ou 'R'
    char piece_j2;                              //                          'J' ou 'R'
    char vainqueur = INCONNU;                   //                          'J' ou 'R'

    int colonne;
    int ligne;
    int compteur_tours;
    int nb_mn = 0;                              // nombre de match(es) nul(s)
    int score_jaune = 0;
    int score_rouge = 0;

    bool resultat_tour = false;                 // si 'true' : un des joueurs a gagné
    bool nouvelle_partie = true;                // si 'true' : initialisation des scores et des noms des joueurs
    bool quitter = false;                       // si 'true' : fin du programme

    while (!quitter)
    {
        initGrille(g);
        colonne = COLONNE_DEBUT;
        ligne = (NBLIG - 1);
        compteur_tours = 0;
        vainqueur = INCONNU;

        if (nouvelle_partie)
        {
            nomsJoueurs(jaune, rouge);
        }

        choixTirage(jaune, rouge, joueur1, joueur2, couleur_j1, couleur_j2, &piece_j1, &piece_j2);
        afficher(g, piece_j1, COLONNE_DEBUT, jaune, rouge, compteur_tours, vainqueur);

        while ((vainqueur == INCONNU) && (!grillePleine(g))) // tant qu'il n'y pas d'alignement gagnant et que la grille n'est pas pleine,
        {            
            jouer(g, piece_j1, &ligne, &colonne, jaune, rouge, compteur_tours, vainqueur);
            compteur_tours++;
            afficher(g, piece_j2, COLONNE_DEBUT, jaune, rouge, compteur_tours, vainqueur);
            resultat_tour = estVainqueur(g, ligne, colonne);

            if (resultat_tour)                  // tour gagnant
            {
                vainqueur = piece_j1;
            }

            else if (!grillePleine(g))          // si pas de gagnant à ce tour,
            {
                jouer(g, piece_j2, &ligne, &colonne, jaune, rouge, compteur_tours, vainqueur);  // le joueur suivant joue
                compteur_tours ++;
                afficher(g, piece_j1, COLONNE_DEBUT, jaune, rouge, compteur_tours, vainqueur);
                resultat_tour = estVainqueur(g, ligne, colonne);

                if (resultat_tour == true)      // si tour gagnant
                {
                    vainqueur = piece_j2;
                }
            }
        }
        finDePartie(vainqueur, jaune, rouge, &score_jaune, &score_rouge, &nb_mn);
        menuFinDePartie(jaune, rouge, &score_jaune, &score_rouge, &nb_mn, &quitter, &nouvelle_partie);
        if ((nouvelle_partie) && (!quitter))
            {
                return true;
            }
    }
    return false;
}

void nomsJoueurs(char *joueur_jaune, char *joueur_rouge)    // COMMENT TRAITER LE CAS D'UN NOM TROP LONG OU VIDE ? No lo sé
{
    effacerFenetre();

    printf("\t\t\t------------------------------------\n");
    printf("\t\t\t##### ⚔️ Qui va s'affronter ? ⚔️ #####\n");
    printf("\t\t\t------------------------------------\n\n");

    printf("\t🟡\t Joueur jaune, écris ton prénom : \n");
    printf("\t👉\t ");
    scanf("%s", joueur_jaune);

    printf("\t🔴\t Joueur rouge, écris ton prénom : \n");
    printf("\t👉\t ");
    scanf("%s", joueur_rouge);

    joueur_jaune[0] = toupper(joueur_jaune[0]);             // Met la première lettre du prénom du joueur jaune en majuscule
    joueur_rouge[0] = toupper(joueur_rouge[0]);             // Met la première lettre du prénom du joueur rouge en majuscule

    printf("\n\t\t Bienvenue à tous les deux, %s et %s ! 🤠\n\n\n", joueur_jaune, joueur_rouge);
    sleep(2);
}

void quiCommence(bool jaune_commence, char *joueur_jaune, char *joueur_rouge, char *j1, char *j2, char *couleur_j1, char *couleur_j2, char *piece_j1, char *piece_j2)
{
    if (jaune_commence)
    {
        strcpy(j1, joueur_jaune);
        strcpy(couleur_j1, PIECE_JAUNE);
        strcpy(j2, joueur_rouge);
        strcpy(couleur_j2, PIECE_ROUGE);
        *piece_j1 = PION_J;
        *piece_j2 = PION_R;
    }
    else if (!jaune_commence)
    {
        strcpy(j1, joueur_rouge);
        strcpy(couleur_j1, PIECE_ROUGE);
        strcpy(j2, joueur_jaune);
        strcpy(couleur_j2, PIECE_JAUNE);
        *piece_j1 = PION_R;
        *piece_j2 = PION_J;
    }
}

void choixTirage(char *joueur_jaune, char *joueur_rouge, char *j1, char *j2, char *couleur_j1, char *couleur_j2, char *piece_j1, char *piece_j2)
{
    effacerFenetre();

    int tirage;
    bool mauvaise_option_choisie = true;
    bool jaune_commence;

    while (mauvaise_option_choisie)
    {
        printf("\t\t\t-----------------------------------\n");
        printf("\t\t\t##### 🏁 Qui va commencer ? 🏁 #####\n");
        printf("\t\t\t-----------------------------------\n\n");

        printf("\t🟡 %s ! (1)\n", joueur_jaune);
        printf("\t🔴 %s ! (2)\n", joueur_rouge);
        printf("\t🎲 Laisser le hasard décider... (0)\n\n");
        printf("\t👉\t ");
        scanf("%d", &tirage);               // COMMENT TRAITER LE CAS D'UNE SAISIE DU MAUVAIS TYPE ? No lo sé

        switch (tirage)
        {
        case 1 :                            // Le joueur jaune commence
            jaune_commence = true;
            quiCommence(jaune_commence, joueur_jaune, joueur_rouge, j1, j2, couleur_j1, couleur_j2, piece_j1, piece_j2);
            mauvaise_option_choisie = false;
            break;
        case 2 :                            // Le joueur rouge commence
            jaune_commence = false;
            quiCommence(jaune_commence, joueur_jaune, joueur_rouge, j1, j2, couleur_j1, couleur_j2, piece_j1, piece_j2);
            mauvaise_option_choisie = false;
            break;
        case 0 :                            // On tire au sort le joueur qui commence
            tirage = rand()%10;
            if (tirage < 5)                // Le joueur jaune commence
            {
                jaune_commence = true;
                quiCommence(jaune_commence, joueur_jaune, joueur_rouge, j1, j2, couleur_j1, couleur_j2, piece_j1, piece_j2);
                mauvaise_option_choisie = false;
            } 
            else if (tirage >= 5)          // Le joueur rouge commence
            {
                jaune_commence = false;
                quiCommence(jaune_commence, joueur_jaune, joueur_rouge, j1, j2, couleur_j1, couleur_j2, piece_j1, piece_j2);
                mauvaise_option_choisie = false;
            }
            break;
        default:    msgErrSaisie();
                    effacerFenetre();
            break;
        }
    }
}

void initGrille(Grille g)
{
    int l;
    int c;

    for (l = 0 ; l < NBLIG ; l++)
    {
        for (c = 0 ; c < NBCOL ; c++)
        {
            g[l][c] = VIDE;
        }
    }
}

void afficher(Grille g, char pion, int colonne, char *jaune, char *rouge, int compteur_tours, char vainqueur)
{
    effacerFenetre();

    int l;
    int c;
    int n;

    char pion_a_jouer[10];
    char joueur[LONGUEUR_MAX_NOM];
    char contenu_cellule[10];

    // Affichage du message invitant le joueur à jouer 👇
    if (pion == PION_J)
    {
        strcpy(pion_a_jouer, PIECE_JAUNE);
        strcpy(joueur, jaune);
    } 
    else if (pion == PION_R)
    {
        strcpy(pion_a_jouer, PIECE_ROUGE);
        strcpy(joueur, rouge);
    }

    if (compteur_tours == 0)
    {
        printf("\t\tC'est toi qui commences, %s ! \n\n", joueur);
        printf("\t👇 Choisis la colonne dans laquelle tu souhaites insérer ta pièce 👇 \n");
        printf("\t\t['q' = ← ; 'd' = ➡️ ; 'espace' = ✅]\n\n");
    }
    else if (compteur_tours == 1)
    {
        printf("\n\t\t\tC'est ton tour, %s !\n\n", joueur);
        printf("\t👇 Choisis la colonne dans laquelle tu souhaites insérer ta pièce 👇 \n");
        printf("\t\t['q' = ← ; 'd' = ➡️ ; 'espace' = ✅]\n\n");
    }
    else if (compteur_tours > 1)
    {
        printf("\t\t\tÀ toi, %s ! 👇\n\n", joueur); 
    }

    // Affichage du décalage nécessaire de la pièce à jouer 👇  // À taire si utilisation de la grille numérotée
    printf("\t\t  ");
    for (n = 0 ; n < colonne ; n++)
    {
        printf("     ");
    }
    printf("%s", pion_a_jouer);
    printf("\n");

    // Affichage de la grille de jeu 👇
    // printf("\t\t   1    2    3    4    5    6    7 \n");        // AFFICHAGE MUET : grille numérotée (maquette)
    for (l = 0 ; l < NBLIG ; l++)
    {
        printf("\t\t+----+----+----+----+----+----+----+\n");
        printf("\t\t|");
        for (c = 0 ; c < NBCOL ; c++)
        {  
            if (g[l][c] == PION_J)
            {
                strcpy(contenu_cellule, PIECE_JAUNE);
            } 
            else if (g[l][c] == PION_R)
            {
                strcpy(contenu_cellule, PIECE_ROUGE);
            }
            else if (g[l][c] == VIDE)
            {
                strcpy(contenu_cellule, CELLULE_VIDE);
            }
            printf(" %s ", contenu_cellule);
            printf("|");
        }
        printf("\n");
    }
    printf("\t\t+----+----+----+----+----+----+----+\n");
    // printf("\t\t   1    2    3    4    5    6    7 \n\n");        // AFFICHAGE MUET : grille numérotée (maquette)
}

bool grillePleine(Grille g)
{
    int somme = 0;
    bool etat_grille = false;

    for (int l = 0 ; l < NBLIG ; l++)
    {
        for (int c = 0 ; c < NBCOL ; c++)
        {
            if (g[l][c] != VIDE)    // s'il y a une pièce dans la cellule, alors
            {
                somme = somme + 1;
            }
        }
    }

    if (somme == 42)                // si la grille est pleine
    {
        etat_grille = true;
    } 
    else // if (somme < 42)         // si la grille n'est pas pleine
    {
        etat_grille = false;
    }
    return etat_grille;
}

int choisirColonne(Grille g, char pion, int *colonne, char *jaune, char *rouge, int compteur_tours, char vainqueur)
{
    *colonne = COLONNE_DEBUT;                       // remet le pion au milieu de la grille à chaque tour
    char deplacement = GAUCHE;

    while (deplacement != VALIDER)
    {
        afficher(g, pion, *colonne, jaune, rouge, compteur_tours, vainqueur);
        printf("👉\t");                             // PROBLÈME : S'AFFICHE UNE FOIS DE TROP
        scanf("%c", &deplacement);
        switch (deplacement)
        {
            case GAUCHE:
                if (*colonne > 0)
                {
                    *colonne = (*colonne - 1);      // recule d'une colonne (vers la gauche)
                }
                else if (*colonne <= 0)
                {
                    printf("\n\tOups, il faut rester dans la grille !\n");
                    sleep(1);
                }
                break;
            case DROITE:
                    if (*colonne < (NBCOL - 1))
                    {
                        *colonne = (*colonne + 1);  // avance d'une colonne (vers la droite)
                    }
                    else if (*colonne >= NBCOL - 1)
                    {
                        printf("\n\tOups, il faut rester dans la grille !\n");
                        sleep(1);
                    }
                    break;
            case VALIDER: break;
            default:    //msgErrSaisie();            // PROBLÈME : S'AFFICHE SYSTÉMATIQUEMENT SI DÉ-COMMENTÉ (même quand pas d'erreur)
                        break;
        }
    }
    return *colonne;

/*
    // AFFICHAGE MUET : choix par numéro de colonne (maquette) 👇
    bool mauvaise_colonne = true;

    while (mauvaise_colonne)
    {
        printf("\t👉\t");
        scanf("%d", colonne);
        
        if ((*colonne != 1) && (*colonne != 2) && (*colonne != 3) && (*colonne != 4) && (*colonne != 5) && (*colonne != 6) && (*colonne != 7))
        {
            printf("\n\tBien essayé, mais cette colonne n’existe pas…! 🤡\n\n");
            sleep(1);
        }
        else
        {
            mauvaise_colonne = false;
        }
    }
    *colonne = (*colonne - 1);    
    return *colonne;
*/
}

int trouverLigne(Grille g, int c)
{
    int ligne = (NBLIG - 1);
    int l;

    for (l = (NBLIG - 1) ; l >= 0 ; l--)
    {
        if (g[l][c] != VIDE)            // si la cellule est occupée, alors
        {
            continue;                   // on recommence au début de la boucle
        }
        else if (g[l][c] == VIDE)       // sinon si la cellule est vide, alors
        {
            ligne = l;                  // 'ligne' prend la valeur de la coordonnée de la cellule vide
            break;                      // et on sort de la boucle
        }
    }

    if (g[0][c] != VIDE)                // si la dernière cellule (haut de la grille) de la colonne sélectionnée est occupée (ie. colonne pleine), alors
    {
        ligne = -1;
    }
    return ligne;
}

void jouer(Grille g, char pion, int *ligne, int *colonne, char *jaune, char *rouge, int compteur_tours, char vainqueur)
{
    *colonne = choisirColonne(g, pion, colonne, jaune, rouge, compteur_tours, vainqueur);
    *ligne = trouverLigne(g, *colonne);

    while (*ligne == - 1)
    {
        printf("\n\tImpossible d’ajouter de nouveaux jetons, cette colonne est déjà pleine !\n\n");
        sleep(1);
        *colonne = choisirColonne(g, pion, colonne, jaune, rouge, compteur_tours, vainqueur);
        *ligne = trouverLigne(g, *colonne);
    }
    g[*ligne][*colonne] = pion;
}

bool estVainqueur(Grille g, int ligne, int colonne)
{
    bool resultat_partie = false;
    int l, c;

    //alignement horizontal 
    for (l = (NBLIG - 1) ; l >= 0 ; l--)
    {
        for (c = 0 ; c <= 3 ; c++)
        {
            if (g[l][c] != VIDE)
            {
                if ((g[l][c] == g[l][c + 1]) && (g[l][c + 1] == g[l][c + 2]) && (g[l][c + 2] == g[l][c + 3]))
                {
                    resultat_partie = true;
                }
            }
        }
    }
    // alignement vertical
    for (c = 0 ; c < NBCOL ; c++)
    {
        for (l = (NBLIG - 1) ; l >= 3 ; l--)
        {
            if (g[l][c] != VIDE)
            {
                if ((g[l][c] == g[l - 1][c]) && (g[l - 1][c] == g[l - 2][c]) && (g[l - 2][c] == g[l - 3][c]))
                {
                    resultat_partie = true;
                }
            }
        }
    }
    // alignement diagonal SE
    for (l = (NBLIG - 1) ; l >= 3 ; l--)
    {
        for (c = 0 ; c <= 3 ; c++)
        {
            if (g[l][c] != VIDE)
            {
                if ((g[l][c] == g[l - 1][c + 1]) && (g[l - 1][c + 1] == g[l - 2][c + 2]) && (g[l - 2][c + 2] == g[l - 3][c + 3]))
                {
                    resultat_partie = true;
                }
            }
        }
    }
    // alignement diagonal SO
    for (l = (NBLIG - 1) ; l >= 3 ; l--)
    {
        for (c = 3 ; c < NBCOL ; c++)
        {
            if (g[l][c] != VIDE)
            {
                if ((g[l][c] == g[l - 1][c - 1]) && (g[l - 1][c - 1] == g[l - 2][c - 2]) && (g[l - 2][c - 2] == g[l - 3][c - 3]))
                {
                    resultat_partie = true;
                }
            }
        }
    }
    return resultat_partie;
}

void finDePartie(char vainqueur, char *jaune, char *rouge, int *score_jaune, int *score_rouge, int *nb_mn)
{
    if (vainqueur != INCONNU)
    {
        if (vainqueur == PION_J)
        {
            printf("\n\t%s remporte la partie ! GG ! 🥳\n", jaune);
            printf("\tSans rancune, %s ! Puis, la deuxième place, c'est pas si mal...\n\n\n", rouge);
            *score_jaune = (*score_jaune + 1);
        }
        else if (vainqueur == PION_R)
        {
            printf("\n\t%s remporte la partie ! GG ! 🥳\n", rouge);
            printf("\tSans rancune, %s ! Puis, la deuxième place, c'est pas si mal...\n\n\n", jaune);
            *score_rouge = (*score_rouge + 1);
        }
    }
    else
    {
        printf("\n\n\tMatch nul ! Félicitations à tous les deux ! 🤝\n\n\n");
        *nb_mn = (*nb_mn + 1);
    }
}

void afficherScores(char *jaune, char *rouge, int *score_jaune, int *score_rouge, int nb_mn)
{   
    int retour = -1;
    while (retour == -1)
    {
        printf("\t\t\t----------------------------------------\n");
        printf("\t\t\t##### 🏆\t SCORES \t🏆 #####\n");
        printf("\t\t\t----------------------------------------\n\n");

        printf("+-----------------------+-----------------------+\n");
        printf("| Match(es) nul(s)\t| %d\t\t\t|\n", nb_mn);
        printf("+-----------------------+-----------------------+\n\n");

        printf("\t\t\t+-----------------------+-----------------------+\n");
        printf("\t\t\t| 🟡 %s\t\t| 🔴 %s\t\t|\n", jaune, rouge);
        printf("+-----------------------+-----------------------+-----------------------+\n");
        printf("| Partie(s) gagnée(s)\t| %d\t\t\t| %d\t\t\t|\n", *score_jaune, *score_rouge);
        printf("+-----------------------+-----------------------+-----------------------+\n\n\n");

        printf("\n\n\t\t\t>> Retour au menu précédent (0)\n");
        printf("\t\t👉\t");

        scanf("%d", &retour);

        switch (retour)
        {
            case 0:     break;
            default:    msgErrSaisie();
                        printf("\t\tAppuie sur la touche '0' pour revenir au menu précédent ↩️\n\n");
                        sleep(2); 
                        retour = -1;
                        effacerFenetre();
                        break;
        }
    }
}

void menuFinDePartie(char *jaune, char *rouge, int *score_jaune, int *score_rouge, int *nb_mn, bool *quitter, bool *nouvelle_partie)
{
    int choix_fin = -1 ; 

    while (choix_fin == -1)
    {
        printf("\n\t\tQue veux-tu faire ?\n\n");

        printf("\t\t> Rejouer (1)\n");
        printf("\t\t> Afficher les scores (2)\n");
        printf("\t\t> Réinitialiser les scores et changer de joueurs (3)\n");
        printf("\t\t> Lire les règles du jeu (4)\n");
        printf("\t\t> Quitter (0)\n\n");

        printf("\t\t👉\t");
        scanf("%d", &choix_fin);

        switch (choix_fin)
        {
            case 1:     *quitter = false;
                        *nouvelle_partie = false;
                        break;
            case 2:     effacerFenetre();
                        afficherScores(jaune, rouge, score_jaune, score_rouge, *nb_mn);
                        choix_fin = -1;
                        break;
            case 3:     effacerFenetre();
                        while (choix_fin == 3)
                        {
                            printf("\n\t\t⚠️\tEs-tu vraiment sûr(e) de vouloir réinitialiser les scores ?\n\n");
                            printf("\t\t> 😬 Euh... Finalement, j'ai changé d'avis ! (0)\n");
                            printf("\t\t> 👍 Oui, ma décision est prise et est irrévocable. (1)\n\n");
                            printf("\t\t👉\t");
                            scanf("%d", &choix_fin);
                            switch (choix_fin)
                            {
                                case 0:     choix_fin = -1;
                                            effacerFenetre();
                                            break;
                                case 1:     *quitter = false;
                                            *nouvelle_partie = true;
                                            *nb_mn = 0;
                                            *score_jaune = 0;
                                            *score_rouge = 0;
                                            effacerFenetre();
                                            break;
                                default:    msgErrSaisie();
                                            effacerFenetre();
                                            choix_fin = 3;
                                            break;
                            }             
                        }
                        break;
            case 4:     effacerFenetre();
                        reglesDuJeu();
                        choix_fin = -1;
                        break;
            case 0:     effacerFenetre();
                        while (choix_fin == 0)
                        {
                            printf("\n\t\tOn se quitte déjà...?\n\n");
                            printf("\t\t> ❌ Non, j'ai changé d'avis ! (0)\n");
                            printf("\t\t> ✅ Oui ! (1)\n");
                            printf("\t\t👉\t");
                            scanf("%d", &choix_fin);
                            switch (choix_fin)
                            {
                                case 0:     choix_fin = -1;
                                            effacerFenetre();
                                            break;
                                case 1:     *quitter = true;
                                            break;
                                default:    msgErrSaisie();
                                            effacerFenetre();
                                            choix_fin = 0;
                                            break;
                            }
                        }
                        break;
            default:    msgErrSaisie();
                        effacerFenetre();
                        choix_fin = -1;
                        break;
        }
    }
}