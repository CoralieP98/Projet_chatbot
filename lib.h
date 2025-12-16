typedef struct 
{
    char nom[20];
    player personnage;
}utilisateur;

typedef struct 
{
    int alignement; //méchant, gentil, neutre
    char asset[3];
    int fin; // 0 si il n'est pas à l'avant dernière étape
    int hist; // historique des étapes enregistrées
    int histIndex; //numéro de l'étape en cours
}player;

typedef struct 
{
    char description[1000];
    int id; //numéro de l'étape
    int droite; //numéro de l'étape vers laquelle rediriger l'utilisteur si il va à droite
    int gauche; //numéro de l'étape vers laquelle rediriger l'utilisteur si il va à gauche
    int toutDroit; //numéro de l'étape vers laquelle rediriger l'utilisteur si il va tout droit
    int combattre; //0 si il fuit 1 pour le rediriger cers l'etape combat
}etape;

typedef struct 
{
    int PV;
    char nom[20];
}PNJ;

