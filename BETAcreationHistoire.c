#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct
{
    char description[1000];
    int id;          // numéro de l'étape
    int option1;    // numéro de l'étape vers laquelle rediriger l'utilisteur si il choisis l'option 1
    int option2;    // numéro de l'étape vers laquelle rediriger l'utilisteur si il choisis l'option 2
    int option3;    // numéro de l'étape vers laquelle rediriger l'utilisteur si il choisis l'option 3
    int combattre; // 0 si il fuit 1 pour le rediriger cers l'etape combat
} etape;

void CreationEtape(FILE *histoire);
int FindIndex(FILE *histoire);
void afficherFichier(FILE *histoire);

int main(void){

FILE *histoire = NULL;
histoire = fopen("histoire.h", "a");
fclose(histoire);
histoire = fopen("histoire.h", "r+");
etape etapes[50];
fseek(histoire,0,SEEK_SET);
fread(etapes,sizeof(etapes),1,histoire);

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
                afficherFichier(histoire);
            case 'q':
            case 'Q':
                break;

            default:
                printf("Commande inconnue [%c]\n", choix);
                break;
            }
            printf("\n\n");
        } while (choix != 'q');
        fclose(histoire);
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
    int index;
    fseek(histoire, 0, SEEK_SET);
    while(fread(&etape0, sizeof(etape), 1, histoire)!=0){
    
       index=etape0.id;

}
    return index;

}

void afficherFichier(FILE *histoire){
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