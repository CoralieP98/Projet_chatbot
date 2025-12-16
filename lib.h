typedef struct 
{
    char nom[20];
    player personnage;
}utilisateur;

typedef struct 
{
    int alignement=0; //méchant, gentil, neutre
    char asset[3];
    int fin=0; // 0 si il n'est pas à l'avant dernière étape
    int hist[100]; // historique des étapes enregistrées
    int histIndex=0; //numéro de l'étape en cours
}player;

typedef struct 
{
    char description[1000];
    int id=0; //numéro de l'étape
    int droite=0; //numéro de l'étape vers laquelle rediriger l'utilisteur si il va à droite
    int gauche=0; //numéro de l'étape vers laquelle rediriger l'utilisteur si il va à gauche
    int toutDroit=0; //numéro de l'étape vers laquelle rediriger l'utilisteur si il va tout droit
    int combattre=0; //0 si il fuit 1 pour le rediriger cers l'etape combat
}etape;

typedef struct 
{
    int PV=0;
    char nom[20];
}PNJ;

