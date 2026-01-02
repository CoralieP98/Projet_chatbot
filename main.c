#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib.h"

void admin(FILE *histoire, FILE *conv, FILE *hisinfoUsr);
FILE *CreationFichier(FILE *file, const char *nom);
void init( FILE *hisinfoUsr, FILE *histoire, FILE *conv);
void ecrireFichier(FILE *fichier, utilisateur user,history init, FILE *histoire, FILE *conv, FILE *hisinfoUsr); // il sera peut-être nécessaire d'ajouter des structures dans les arguments d'entrée
void afficherFichier(FILE *fichier, FILE *histoire, FILE *conv, FILE *hisinfoUsr);
void menu1(FILE *histoire, FILE *conv, FILE *hisinfoUsr); // prototype à affiner
void histoirefct(FILE *histoire);
void CreationEtape(FILE *histoire);
int FindIndex(FILE *histoire);
void afficherFichierEtape(FILE *histoire);
void etapeRunning(utilisateur user, FILE *conv, FILE *histoire);
etape parcourirHistoire(int id,FILE *histoire);
int traitementReponse(char reponse[50]);
void archiveConv(utilisateur user, FILE *conv, char text[lenMaxPrompt]);
void afficherConv(FILE *conv);

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
    history init;
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
    strcpy(init.userName,user.nom);
    init.index=0;
    ecrireFichier(hisinfoUsr, user,init,histoire,conv,hisinfoUsr);
    ecrireFichier(conv,user,init,histoire,conv,hisinfoUsr);
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
            printf("Afficher le fichier conv....................: T\n");
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
            case 't':
            case 'T':
                afficherConv(conv);
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
    //promptedDescription[strcspn(promptedDescription, "\n")] = '\0'; // Supprimer le '\n' ajouté par fgets 

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
    history archive;
    int IdEtapeActuelle = user.personnage.histIndex;
    char reponse[50]="";
    etapeActuelle=parcourirHistoire(IdEtapeActuelle, histoire);
    archiveConv(user,conv,etapeActuelle.description);
    printf("%s",etapeActuelle.description);
     getchar();// vide le buffer pour éviter que fgets pante
    fgets(reponse, sizeof(reponse), stdin); // on utilise fgets pour pouvoir saisir un texte avec des espaces
    reponse[strcspn(reponse, "\n")] = '\0'; // Supprimer le '\n' ajouté par fgets 
    int decision=traitementReponse(reponse); 
    while(decision==0){printf("!! Je ne comprends pas la décision prise, articule le sang !!\n ");
        strcpy(reponse,"");
        //getchar();// vide le buffer pour éviter que fgets pante
        fgets(reponse, sizeof(reponse), stdin); // on utilise fgets pour pouvoir saisir un texte avec des espaces
        decision=traitementReponse(reponse);
    }   
    //log etape passé
    archiveConv(user,conv,reponse);
    //log conv

    if(decision==1){
        user.personnage.histIndex=etapeActuelle.option1;
        //etapeRunning(user,conv,histoire);
        return;
    }
    else if(decision==2){
        user.personnage.histIndex=etapeActuelle.option2;
        etapeRunning(user,conv,histoire);
        return;
    }
    if(decision==3){
        user.personnage.histIndex=etapeActuelle.option3;
        etapeRunning(user,conv,histoire);
        return;
    }
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

int traitementReponse(char reponse[50]){
    for (int i = 0; reponse[i] != '\0' && i < 50; i++) {
        if (reponse[i] == ' '||reponse[i] == '.') {
            if (i >= 6) {
                if (strncmp(&reponse[i - 6], "droite", 6) == 0) {
                    return 1;
                } else if (strncmp(&reponse[i - 6], "gauche", 6) == 0) {
                    return 2;
                }
            }
            if (i >= 10) {
                if (strncmp(&reponse[i - 10], "tout droit", 10) == 0) {
                    return 3;
                }
            }
        }
    }
    return 0; //erreur 
}


void archiveConv(utilisateur user, FILE *conv, char text[lenMaxPrompt]){
    fseek(conv, 0, SEEK_SET);
    history archive;
    int index=0;
    while(fread(&archive, sizeof(history), 1, conv)!=0){
        if(strcmp(user.nom,archive.userName)==0){
            printf("%s",archive.userName);
            break;
        }
    }
    fseek(conv,-sizeof(history),SEEK_CUR);
    index = archive.index + 1;
    //if(index < 0) index = 0;
    //if(index >= 2*nbMaxEtape) index = 2*nbMaxEtape - 1;
    archive.index=index;
    strncpy(archive.conv[index], text, lenMaxPrompt-1);
    archive.conv[index][lenMaxPrompt - 1] = '\0';
    fwrite(&archive,sizeof(history),1, conv);
}

void afficherConv(FILE *conv){

    history stock;
    fseek(conv, 0, SEEK_SET);
    while(fread(&stock, sizeof(history), 1, conv)!=0){
        printf("nom : %s\n",stock.userName);
        printf("index : %d\n",stock.index);
        for(int i=0; i<2*nbMaxEtape;i++){
            printf("%s\n",stock.conv[i]);
        }
}
}
