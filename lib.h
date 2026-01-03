
#define nbMaxEtape 10
#define NbMaxAssets 3
#define password "password"
#define lenMaxPrompt 1000

typedef struct
{
    char userName[20];
    int index;
    char conv[2*nbMaxEtape][lenMaxPrompt];
}history;

typedef struct
{
    int alignement; // méchant, gentil, neutre
    int asset[NbMaxAssets];
    int fin;              // 0 si il n'est pas à l'avant dernière étape
    int hist[nbMaxEtape]; // historique des étapes enregistrées
    int histIndex;        // numéro de l'étape en cours
    int PV;
} player;

typedef struct
{
    char nom[20];
    player personnage;
    char answer[100];
} utilisateur;

typedef struct
{
    int PV;
    char nom[20];
} PNJ;

typedef struct
{
    char description[lenMaxPrompt];
    int id;          // numéro de l'étape
    int option1;    // numéro de l'étape vers laquelle rediriger l'utilisteur si il choisis l'option 1
    int option2;    // numéro de l'étape vers laquelle rediriger l'utilisteur si il choisis l'option 2
    int option3;    // numéro de l'étape vers laquelle rediriger l'utilisteur si il choisis l'option 3
    int combattre; // 0 si il fuit 1 pour le rediriger cers l'etape combat
    char combatPNJ[20];
} etape;

FILE *CreationFichier(FILE *file, const char *nom)
{

    char nomComplet[30];

    strcpy(nomComplet, nom);
    strcat(nomComplet, ".dat");
    file = fopen(nomComplet, "a");
    fclose(file);
    file = fopen(nomComplet, "r+");
    return file;
}

void afficherFichier(FILE *fichier, FILE *histoire, FILE *conv, FILE *hisinfoUsr)
{
    
        utilisateur user;
        fseek(fichier, 0, SEEK_SET);
        // on va lire des utilisateurs du fichier un par un jusqu'à la fin du fichier
        while (fread(&user, sizeof(utilisateur), 1, fichier) != 0)
        {
            printf("nom utilisateur : %s\n", user.nom);
            printf("alignement : %d\n", user.personnage.alignement);
            printf("assets : %d, %d, %d\n", user.personnage.asset[0], user.personnage.asset[1], user.personnage.asset[2]);
            printf("PV : %d",user.personnage.PV);
            printf("étapes parcourues : \n");
            for (int i = 0; i < nbMaxEtape; i++)
            {
                printf("%d\n", user.personnage.hist[i]);
            }
            printf("étape en cours : %d\n", user.personnage.histIndex);
        }
    
}

void ecrireFichier(FILE *fichier, utilisateur user,history init, FILE *histoire, FILE *conv, FILE *hisinfoUsr)
{
    history parcours;
    if(fichier==hisinfoUsr){
        fseek(fichier, 0, SEEK_END);
        fwrite(&user,sizeof(utilisateur),1, fichier);
    }
    else if(fichier==conv){
        int trouve=0;
        fseek(fichier, 0, SEEK_SET);
         while (fread(&parcours, sizeof(history), 1, fichier) != 0){
            if(strcmp(parcours.userName, init.userName) == 0){
                fseek(conv,-sizeof(history),SEEK_CUR);
                fwrite(&init,sizeof(history),1, fichier);
                trouve=1;

            }
         }
         if(trouve==0){
            fseek(fichier, 0, SEEK_END);
            fwrite(&init,sizeof(history),1, fichier);
         }
    }
    
    
}
