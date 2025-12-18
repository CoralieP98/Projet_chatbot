#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib.h"

void admin(FILE *hisinfoUsr, FILE *histoire, FILE *conv);
FILE *CreationFichier(FILE *file, const char *nom);
void init(FILE *fichier, FILE *hisinfoUsr, FILE *histoire, FILE *conv);
void ecrireFichier(FILE *fichier, utilisateur user); // il sera peut-être nécessaire d'ajouter des structures dans les arguments d'entrée
void afficherFichier(FILE *fichier, FILE *histoire, FILE *conv, FILE *hisinfoUsr);
void menu1(FILE *histoire, FILE *conv, FILE *hisinfoUsr); // prototype à affiner

int main(void)
{
    FILE *histoire = NULL;
    FILE *conv = NULL;
    FILE *hisinfoUsr = NULL;
    histoire = CreationFichier(histoire, "histoire");
    conv = CreationFichier(conv, "conv");
    hisinfoUsr = CreationFichier(hisinfoUsr, "histInfoUsr");
    menu1(histoire, conv, hisinfoUsr);

    // afficherFichier(hisinfoUsr,histoire,conv,hisinfoUsr); //debug use only
}

void menu1(FILE *histoire, FILE *conv, FILE *hisinfoUsr)
{
    {
        char choix;
        printf("##########################################\n");
        printf("Entrez dans le mode admin..............: A\n");
        printf("Lançer le jeu..........................: L\n");
        printf("Quitter................................: Q\n");
        printf(" votre choix: ");

        scanf(" %c", &choix);

        switch (choix)
        {
        case 'a':
        case 'A':
            admin(histoire, conv, hisinfoUsr);
            break;

        case 'l':
        case 'L':
            init(hisinfoUsr, hisinfoUsr, histoire, conv);
            break;

        case 'q':
        case 'Q':
            printf("sortie du menu\n");
            break;

        default:
            printf("Commande inconnue [%c]\n", choix);
            break;
        }
        printf("\n\n");
    }
}

void init(FILE *fichier, FILE *hisinfoUsr, FILE *histoire, FILE *conv)
{
    utilisateur user;
    printf("Saisissez votre pseudo (20 caractères max):");
    scanf(" %s", user.nom);
    user.personnage.alignement = 0;
    user.personnage.fin = 0;
    user.personnage.histIndex = 0;
    for (int i = 0; i < NbMaxAssets; i++)
    {
        user.personnage.asset[i] = 0;
    }
    for (int i = 0; i < nbMaxEtape; i++)
    {
        user.personnage.hist[i] = 0;
    }
    ecrireFichier(hisinfoUsr, user);
}

void admin(FILE *hisinfoUsr, FILE *histoire, FILE *conv)
{
    char promptedPassword[20];
    printf("Entrez le mot de passe : ");
    scanf(" %s", promptedPassword);
    if (strcmp(password, promptedPassword) == 0)
    {
        printf("***********************************************\n");
        printf("Bienvenue Admin !\n\n");
        char choix;

        do
        {
            printf("Afficher les joueurs enregistrés............: A\n");

            printf("Quitter.....................................: Q\n");
            printf(" votre choix: ");

            scanf(" %c", &choix);

            switch (choix)
            {
            case 'a':
            case 'A':
                afficherFichier(hisinfoUsr, hisinfoUsr, conv, hisinfoUsr);
                break;

            case 'q':
            case 'Q':
                printf("sortie du mode admin\n\n");
                menu1(histoire, conv, hisinfoUsr);
                break;

            default:
                printf("Commande inconnue [%c]\n", choix);
                break;
            }
            printf("\n\n");
        } while (choix != 'q');
    }
    else
    {
        printf("mot de passe incorrect");
    }
}
