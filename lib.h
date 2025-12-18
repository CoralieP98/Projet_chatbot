#define nbMaxEtape 10
#define NbMaxAssets 3
#define password "password"

typedef struct
{
    int alignement; // méchant, gentil, neutre
    int asset[NbMaxAssets];
    int fin;              // 0 si il n'est pas à l'avant dernière étape
    int hist[nbMaxEtape]; // historique des étapes enregistrées
    int histIndex;        // numéro de l'étape en cours
} player;

typedef struct
{
    char nom[20];
    player personnage;
} utilisateur;

typedef struct
{
    char description[1000];
    int id;        // numéro de l'étape
    int droite;    // numéro de l'étape vers laquelle rediriger l'utilisteur si il va à droite
    int gauche;    // numéro de l'étape vers laquelle rediriger l'utilisteur si il va à gauche
    int toutDroit; // numéro de l'étape vers laquelle rediriger l'utilisteur si il va tout droit
    int combattre; // 0 si il fuit 1 pour le rediriger cers l'etape combat
} etape;

typedef struct
{
    int PV;
    char nom[20];
} PNJ;

FILE *CreationFichier(FILE *file, const char *nom)
{

    char nomComplet[30];

    strcpy(nomComplet, nom);
    strcat(nomComplet, ".dat");
    file = fopen(nom, "a");
    fclose(file);
    file = fopen(nom, "r+");
    return file;
}

void afficherFichier(FILE *fichier, FILE *histoire, FILE *conv, FILE *hisinfoUsr)
{
    if (fichier == hisinfoUsr)
    {
        utilisateur user;
        fseek(fichier, 0, SEEK_SET);
        // on va lire des CLIENT du fichier un par un jusqua la fin du fichier
        while (fread(&user, sizeof(utilisateur), 1, fichier) != 0)
        {
            printf("nom utilisateur : %s\n", user.nom);
            printf("alignement : %d\n", user.personnage.alignement);
            printf("assets : %d, %d, %d\n", user.personnage.asset[0], user.personnage.asset[1], user.personnage.asset[2]);
            printf("étapes parcourues : \n");
            for (int i = 0; i < nbMaxEtape; i++)
            {
                printf("%d\n", user.personnage.hist[i]);
            }
            printf("étape en cours : %d\n", user.personnage.histIndex);
        }
    }
}

void ecrireFichier(FILE *fichier, utilisateur user)
{
    fseek(fichier, 0, SEEK_END);
    fwrite(&user, sizeof(utilisateur), 1, fichier);
}
