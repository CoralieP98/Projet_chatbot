
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
    // int asset[NbMaxAssets];
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

typedef struct {
    int id;
    char description[1000];

    int option1;
    int option2;
    int option3;

    char combatPNJ[20];

    char key1a[20];
    char key1b[20];
    int  align1;

    char key2a[20];
    char key2b[20];
    int  align2;

    char key3a[20];
    char key3b[20];
    int  align3;

    int autoNext;
} etape;

typedef struct {
    int option;      // 0 = incompris, 1/2/3 = choix, 4 = combat, etc.
    int deltaAlign;  // variation d'alignement
} Decision;

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
            // printf("assets : %d, %d, %d\n", user.personnage.asset[0], user.personnage.asset[1], user.personnage.asset[2]);
            printf("PV : %d\n",user.personnage.PV);
            printf("étapes parcourues : \n");
            for (int i = 0; i < nbMaxEtape; i++)
            {
                printf("%d\n", user.personnage.hist[i]);
            }
            printf("étape en cours : %d\n", user.personnage.histIndex);
        }

}

void ecrireFichier(FILE *fichier, utilisateur user, history init, FILE *histoire, FILE *conv, FILE *hisinfoUsr)
{
    history parcours;

    if (fichier == hisinfoUsr) {
        printf("[DEBUG] ecrireFichier -> hisinfoUsr pour user '%s'\n", user.nom);
        fseek(fichier, 0, SEEK_END);
        fwrite(&user, sizeof(utilisateur), 1, fichier);
    }
    else if (fichier == conv) {
        printf("[DEBUG] ecrireFichier -> conv pour user '%s'\n", user.nom);
        int trouve = 0;
        fseek(fichier, 0, SEEK_SET);

        while (fread(&parcours, sizeof(history), 1, fichier) != 0) {
            if (strcmp(parcours.userName, init.userName) == 0) {
                printf("[DEBUG]   user deja present dans conv.dat, mise a jour\n");
                fseek(fichier, -(long)sizeof(history), SEEK_CUR);
                fwrite(&init, sizeof(history), 1, fichier);
                trouve = 1;
                break;
            }
        }

        if (!trouve) {
            printf("[DEBUG]   user pas trouve, ajout nouvelle entree dans conv.dat\n");
            fseek(fichier, 0, SEEK_END);
            fwrite(&init, sizeof(history), 1, fichier);
        }

        fflush(fichier);
    } else {
        printf("[DEBUG] ecrireFichier appelee avec un fichier inconnu\n");
    }
}