#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib.h"

void admin(FILE *histoire, FILE *conv, FILE *hisinfoUsr);
FILE *CreationFichier(FILE *file, const char *nom);
void init( FILE *hisinfoUsr, FILE *histoire, FILE *conv);
void ecrireFichier(FILE *fichier, utilisateur user); // il sera peut-être nécessaire d'ajouter des structures dans les arguments d'entrée
void afficherFichier(FILE *fichier, FILE *histoire, FILE *conv, FILE *hisinfoUsr);
void menu1(FILE *histoire, FILE *conv, FILE *hisinfoUsr); // prototype à affiner
void histoirefct(FILE *histoire);
void CreationEtape(FILE *histoire);
int FindIndex(FILE *histoire);
void afficherFichierEtape(FILE *histoire);
void etapeRunning(utilisateur user, FILE *conv, FILE *histoire);
etape parcourirHistoire(int id,FILE *histoire);

int main(void)
{
    FILE *histoire = NULL;
    FILE *conv = NULL;
    FILE *hisinfoUsr = NULL;
    histoire = CreationFichier(histoire, "histoire");
    conv = CreationFichier(conv, "conv");
    hisinfoUsr = CreationFichier(hisinfoUsr, "histInfoUsr");
    menu1(histoire, conv, hisinfoUsr);
    fclose(hisinfoUsr);
    fclose(histoire);
    fclose(conv);
    return 0;
}

void menu1(FILE *histoire, FILE *conv, FILE *hisinfoUsr)
{
    {
        char choix;
        printf("##########################################\n");
        printf("Entrez dans le mode admin..............: A\n");
        printf("Commencer une nouvelle partie..........: L\n");
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
            init(hisinfoUsr, histoire, conv);
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

void init(FILE *hisinfoUsr, FILE *histoire, FILE *conv)
{
    utilisateur user;
    printf("Saisissez votre pseudo (20 caractères max):");
    scanf(" %s", user.nom);
    user.personnage.alignement = 0;
    user.personnage.fin = 0;
    user.personnage.histIndex = 1;
    for (int i = 0; i < NbMaxAssets; i++)
    {
        user.personnage.asset[i] = 0;
    }
    for (int i = 0; i < nbMaxEtape; i++)
    {
        user.personnage.hist[i] = 0;
    }
    ecrireFichier(hisinfoUsr, user);
    etapeRunning(user,conv,histoire);
    
}

void admin(FILE *histoire, FILE *conv, FILE *hisinfoUsr)
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
            printf("Modifier l'histoire.........................: H\n");
            printf("Quitter.....................................: Q\n");
            printf(" votre choix: ");

            scanf(" %c", &choix);

            switch (choix)
            {
            case 'a':
            case 'A':
                afficherFichier(hisinfoUsr, histoire, conv, hisinfoUsr);
                break;
            case 'h':
            case 'H':
                histoirefct(histoire);
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
        } while (choix != 'q' && choix != 'Q');
    }
    else
    {
        printf("mot de passe incorrect");
    }
}

void histoirefct(FILE *histoire){

fseek(histoire,0,SEEK_SET);
printf("***********************************************\n");
        printf("Que voulez-vous faire\n\n");
        char choix;

        do
        {
            printf("Créer une nouvelle étape....................: C\n");
            printf("Affichier fichier histoire..................: A\n");
            printf("Quitter.....................................: Q\n");
            printf(" votre choix: ");

            scanf(" %c", &choix);

            switch (choix)
            {
            case 'C':
            case 'c':
                CreationEtape(histoire);
                break;
            case 'A':
            case 'a':
                afficherFichierEtape(histoire);
            case 'q':
            case 'Q':
                break;

            default:
                printf("Commande inconnue [%c]\n", choix);
                break;
            }
            printf("\n\n");
        } while (choix != 'q' && choix != 'Q');
        // Ne pas fermer le fichier ici : il est fermé dans main()
}

void CreationEtape(FILE *histoire){
    char promptedDescription[1000];
    etape etape0;
    printf("Saisissez la description de l'histoire (1000 caractères max):");
    getchar();// vide le buffer pour éviter que fgets pante
    fgets(promptedDescription, sizeof(promptedDescription), stdin); // on utilise fgets pour pouvoir saisir un texte avec des espaces
    promptedDescription[strcspn(promptedDescription, "\n")] = '\0'; // Supprimer le '\n' ajouté par fgets 

    printf("Saisissez le renvoi 1 :\n");
    scanf("%d",&etape0.option1);
    printf("Saisissez le renvoi 2 :\n");
    scanf("%d",&etape0.option2);
    printf("Saisissez le renvoi 3 :\n");
    scanf("%d",&etape0.option3);

    int index =FindIndex(histoire); 
    fseek(histoire,0,SEEK_END);
    
    etape0.id=index+1;
    strcpy(etape0.description,promptedDescription);
    fwrite(&etape0,sizeof(etape),1,histoire);
    
}

int FindIndex(FILE *histoire){
    etape etape0;
    int index = 0;
    fseek(histoire, 0, SEEK_SET);
    while(fread(&etape0, sizeof(etape), 1, histoire)!=0){
    
       index=etape0.id;

}
    return index;

}

void afficherFichierEtape(FILE *histoire){
    fseek(histoire, 0, SEEK_SET);
    etape etape0;
    while(fread(&etape0, sizeof(etape), 1, histoire)!=0){
    
        printf("id : %d\n",etape0.id);
        printf("description : %s\n",etape0.description);
        printf("option 1 : %d\n",etape0.option1);
        printf("option 2 : %d\n",etape0.option2);
        printf("option 3 : %d\n",etape0.option3);

}
    
}




void etapeRunning(utilisateur user, FILE *conv, FILE *histoire){
    etape etapeActuelle;
    int IdEtapeActuelle = user.personnage.histIndex;
    etapeActuelle=parcourirHistoire(IdEtapeActuelle, histoire);
    printf("%s",etapeActuelle.description);
    

}

etape parcourirHistoire(int id,FILE *histoire){
     etape etape0;
    fseek(histoire, 0, SEEK_SET);
     while(fread(&etape0, sizeof(etape), 1, histoire)!=0){
    
       if(etape0.id==id){
            break;
       }

    }
    return etape0;

}