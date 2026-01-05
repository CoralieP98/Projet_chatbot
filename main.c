#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib.h"
#include <time.h>

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
void etapeRunning(utilisateur user, FILE *conv, FILE *histoire, FILE *hisinfoUsr);
etape parcourirHistoire(int id,FILE *histoire);
int traitementReponse(char reponse[50]);
void archiveConv(utilisateur user, FILE *conv, char text[lenMaxPrompt]);
void afficherConv(FILE *conv);
void combat(int etapeId,char nomPNJ[20], FILE *hisinfoUsr,FILE *conv,FILE *histoire, utilisateur user);
int contientMotCle(const char *reponse, const char *mot);
Decision analyserReponseEtape(const char *reponse, etape *e);
void sauvegarderUtilisateur(FILE *hisinfoUsr, utilisateur user);
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

    printf("Saisissez votre pseudo (20 caracteres max):");
    scanf(" %s", user.nom);
    printf("woah ah ouais t'avais pas mieux?");


    // Initialisation du joueur
    user.personnage.alignement = 0;
    user.personnage.fin = 0;
    user.personnage.histIndex = 1;
    user.personnage.PV = 100;

    // for (int i = 0; i < NbMaxAssets; i++) {
    //     user.personnage.asset[i] = 0;
    // }
    for (int i = 0; i < nbMaxEtape; i++) {
        user.personnage.hist[i] = 0;
    }

    // Initialisation de l'historique de conversation pour ce user
    strcpy(init.userName, user.nom);
    init.index = 0;
    for (int i = 0; i < 2 * nbMaxEtape; i++) {
        init.conv[i][0] = '\0';
    }

    printf("\n\n\n-------------------------------------------------------------------\n");
    printf("Instructions de jeu :\n");
    printf("Chacune de vos reponses au ChatBot devra se terminer par un point.\n");
    printf("-------------------------------------------------------------------\n\n\n");

    ecrireFichier(hisinfoUsr, user, init, histoire, conv, hisinfoUsr);

    ecrireFichier(conv, user, init, histoire, conv, hisinfoUsr);

    etapeRunning(user, conv, histoire, hisinfoUsr);
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
    int index = FindIndex(histoire);

    // On nettoie la structure
    memset(&etape0, 0, sizeof(etape));

    // ID de l'étape
    etape0.id = index + 1;

    // --- Description ---
    printf("Saisissez la description de l'histoire (1000 caracteres max):\n");
    getchar();
    fgets(promptedDescription, sizeof(promptedDescription), stdin);
    // promptedDescription[strcspn(promptedDescription, "\n")] = '\0';
    strcpy(etape0.description, promptedDescription);

    printf("Est-ce une etape automatique (sans choix, juste narration) ? (0 = non, 1 = oui) : ");
    scanf("%d", &etape0.autoNext);

    printf("Saisissez le renvoi 1 (id de l'etape suivante, 0 si aucun) :\n");
    scanf("%d", &etape0.option1);

    if (etape0.autoNext == 0) {
        printf("Saisissez le renvoi 2 (0 si aucun) :\n");
        scanf("%d", &etape0.option2);

        printf("Saisissez le renvoi 3 (0 si aucun) :\n");
        scanf("%d", &etape0.option3);
    } else {
        etape0.option2 = 0;
        etape0.option3 = 0;
    }

    printf("Saisissez le nom du PNJ a combattre (\"0\" si pas d'etape de combat a suivre) :\n");
    scanf("%19s", etape0.combatPNJ);

    // Si ce n'est PAS une étape automatique, on demande les mots-clés et alignements
    if (etape0.autoNext == 0) {
        getchar();

        // ---------- Option 1 ----------
        printf("\n--- Configuration des mots-cles pour l'option 1 ---\n");
        printf("Mot-cle 1 (laisser vide si inutile) : ");
        fgets(etape0.key1a, sizeof(etape0.key1a), stdin);
        etape0.key1a[strcspn(etape0.key1a, "\n")] = '\0';

        printf("Mot-cle 2 (laisser vide si inutile) : ");
        fgets(etape0.key1b, sizeof(etape0.key1b), stdin);
        etape0.key1b[strcspn(etape0.key1b, "\n")] = '\0';

        printf("Alignement pour l'option 1 (ex: 1, 0, -1) : ");
        scanf("%d", &etape0.align1);
        getchar(); // vider le buffer

        // ---------- Option 2 ----------
        printf("\n--- Configuration des mots-cles pour l'option 2 ---\n");
        printf("Mot-cle 1 (laisser vide si inutile) : ");
        fgets(etape0.key2a, sizeof(etape0.key2a), stdin);
        etape0.key2a[strcspn(etape0.key2a, "\n")] = '\0';

        printf("Mot-cle 2 (laisser vide si inutile) : ");
        fgets(etape0.key2b, sizeof(etape0.key2b), stdin);
        etape0.key2b[strcspn(etape0.key2b, "\n")] = '\0';

        printf("Alignement pour l'option 2 (ex: 1, 0, -1) : ");
        scanf("%d", &etape0.align2);
        getchar(); // vider le buffer

        // ---------- Option 3 ----------
        printf("\n--- Configuration des mots-cles pour l'option 3 ---\n");
        printf("Mot-cle 1 (laisser vide si inutile) : ");
        fgets(etape0.key3a, sizeof(etape0.key3a), stdin);
        etape0.key3a[strcspn(etape0.key3a, "\n")] = '\0';

        printf("Mot-cle 2 (laisser vide si inutile) : ");
        fgets(etape0.key3b, sizeof(etape0.key3b), stdin);
        etape0.key3b[strcspn(etape0.key3b, "\n")] = '\0';

        printf("Alignement pour l'option 3 (ex: 1, 0, -1) : ");
        scanf("%d", &etape0.align3);
    } else {

        etape0.key1a[0] = '\0';
        etape0.key1b[0] = '\0';
        etape0.align1 = 0;

        etape0.key2a[0] = '\0';
        etape0.key2b[0] = '\0';
        etape0.align2 = 0;

        etape0.key3a[0] = '\0';
        etape0.key3b[0] = '\0';
        etape0.align3 = 0;
    }

    fseek(histoire, 0, SEEK_END);
    fwrite(&etape0, sizeof(etape), 1, histoire);
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

    while (fread(&etape0, sizeof(etape), 1, histoire) != 0) {

        printf("======================================\n");
        printf("id          : %d\n", etape0.id);
        printf("autoNext    : %d\n", etape0.autoNext);
        printf("description : %s\n", etape0.description);
        printf("option 1    : %d\n", etape0.option1);
        printf("option 2    : %d\n", etape0.option2);
        printf("option 3    : %d\n", etape0.option3);
        printf("combatPNJ   : %s\n", etape0.combatPNJ);

        if (etape0.autoNext == 0) {
            printf("--- Option 1 ---\n");
            printf("  key1a   : \"%s\"\n", etape0.key1a);
            printf("  key1b   : \"%s\"\n", etape0.key1b);
            printf("  align1  : %d\n", etape0.align1);

            printf("--- Option 2 ---\n");
            printf("  key2a   : \"%s\"\n", etape0.key2a);
            printf("  key2b   : \"%s\"\n", etape0.key2b);
            printf("  align2  : %d\n", etape0.align2);

            printf("--- Option 3 ---\n");
            printf("  key3a   : \"%s\"\n", etape0.key3a);
            printf("  key3b   : \"%s\"\n", etape0.key3b);
            printf("  align3  : %d\n", etape0.align3);
        } else {
            printf("(Etape automatique : pas de mots-cles, pas de choix)\n");
        }

        printf("\n");
    }
}



void etapeRunning(utilisateur user, FILE *conv, FILE *histoire, FILE *hisinfoUsr){
    etape etapeActuelle;
    int IdEtapeActuelle = user.personnage.histIndex;

    if (user.personnage.histIndex == 999) {
        utilisateur u;
        int align = user.personnage.alignement;

        fseek(hisinfoUsr, 0, SEEK_SET);
        while (fread(&u, sizeof(utilisateur), 1, hisinfoUsr) != 0) {
            if (strcmp(u.nom, user.nom) == 0) {
                align = u.personnage.alignement;
                break;
            }
        }
        printf("\n========== FIN DU JEU ==========\n");
        printf("Votre alignement final est : %d\n", align);

        if (align < 0) {
            printf("Bienvenue dans l'ordre du Pistou.\n Vous êtes un héros bon, juste, et profondément ignoré.\n Une statue est érigée...Elle est mal sculptés.");
        } else if (align > 0) {
            printf("Bienvenue dans l'ordre de la Farigoule.\n Vous êtes craint.et respecté.\n Et interdit de cuisine à vie.");
        } else {
            printf("Bienvenue dans l'ordre de l'Estragon.\n Vous existez c'est déjà pas mal :/");
        }
        printf("================================\n");

        fclose(hisinfoUsr);
        fclose(histoire);
        fclose(conv);
        exit(0);
    }

    // Charger l'étape courante
    etapeActuelle = parcourirHistoire(IdEtapeActuelle, histoire);

    archiveConv(user, conv, etapeActuelle.description);
    printf("%s\n", etapeActuelle.description);

    // Étape automatique : on saute directement à option1
    if (etapeActuelle.autoNext == 1) {
        if (etapeActuelle.option1 > 0) {
            user.personnage.histIndex = etapeActuelle.option1;
            sauvegarderUtilisateur(hisinfoUsr, user);
            etapeRunning(user, conv, histoire, hisinfoUsr);
            return;
        } else {
            // Pas de prochaine étape définie -> on considère que c'est la fin
            user.personnage.histIndex = 999;
            etapeRunning(user, conv, histoire, hisinfoUsr);
            return;
        }
    }

    // Étape avec choix
    char reponse[50] = "";
    getchar(); // vider le buffer pour éviter que fgets plante
    fgets(reponse, sizeof(reponse), stdin);
    reponse[strcspn(reponse, "\n")] = '\0';

    // Commande spéciale pour revenir au menu
    if (strcmp(reponse, "menu.") == 0) {
        printf("Retour au menu principal...\n");
        menu1(histoire, conv, hisinfoUsr);
        return;
    }

    Decision d = analyserReponseEtape(reponse, &etapeActuelle);

    while (d.option == 0) {
        printf("!! Je ne comprends pas la décision prise, articule le sang !!\n ");
        strcpy(reponse, "");
        fgets(reponse, sizeof(reponse), stdin);
        reponse[strcspn(reponse, "\n")] = '\0';

        if (strcmp(reponse, "menu.") == 0) {
            printf("Retour au menu principal...\n");
            menu1(histoire, conv, hisinfoUsr);
            return;
        }

        d = analyserReponseEtape(reponse, &etapeActuelle);
    }

    archiveConv(user, conv, reponse);

    user.personnage.alignement += d.deltaAlign;
    sauvegarderUtilisateur(hisinfoUsr, user);

    // Gestion des choix
    if (d.option == 1) {
        user.personnage.histIndex = etapeActuelle.option1;
    } else if (d.option == 2) {
        user.personnage.histIndex = etapeActuelle.option2;
    } else if (d.option == 3) {
        user.personnage.histIndex = etapeActuelle.option3;
    } else if (d.option == 4) {
        combat(etapeActuelle.option3, etapeActuelle.combatPNJ, hisinfoUsr, conv, histoire, user);
        return;
    }

    etapeRunning(user, conv, histoire, hisinfoUsr);
}

int contientMotCle(const char *reponse, const char *mot) {
    if (mot[0] == '\0')
        return 0;

    // Recherche simple, sensible à la casse
    return (strstr(reponse, mot) != NULL);
}

Decision analyserReponseEtape(const char *reponse, etape *e) {
    Decision d;
    d.option = 0;
    d.deltaAlign = 0;

    if (contientMotCle(reponse, e->key1a) || contientMotCle(reponse, e->key1b)) {
        d.option = 1;
        d.deltaAlign = e->align1;
        return d;
    }

    if (contientMotCle(reponse, e->key2a) || contientMotCle(reponse, e->key2b)) {
        d.option = 2;
        d.deltaAlign = e->align2;
        return d;
    }

    if (contientMotCle(reponse, e->key3a) || contientMotCle(reponse, e->key3b)) {
        d.option = 3;
        d.deltaAlign = e->align3;
        return d;
    }

    if (strstr(reponse, "combattre") != NULL &&
        strcmp(e->combatPNJ, "0") != 0 &&
        e->combatPNJ[0] != '\0') {
        d.option = 4;
        d.deltaAlign = 0;
        return d;
        }

    return d; // gné?
}

void sauvegarderUtilisateur(FILE *hisinfoUsr, utilisateur user) {
    utilisateur u;
    fseek(hisinfoUsr, 0, SEEK_SET);

    while (fread(&u, sizeof(utilisateur), 1, hisinfoUsr) != 0) {
        if (strcmp(u.nom, user.nom) == 0) {
            fseek(hisinfoUsr, -(long)sizeof(utilisateur), SEEK_CUR);
            fwrite(&user, sizeof(utilisateur), 1, hisinfoUsr);
            fflush(hisinfoUsr);
            return;
        }
    }

    fseek(hisinfoUsr, 0, SEEK_END);
    fwrite(&user, sizeof(utilisateur), 1, hisinfoUsr);
    fflush(hisinfoUsr);
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
//old one
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
            if (i >= 9) {
                if (strncmp(&reponse[i - 9], "combattre", 9) == 0) {
                    return 4;
                }
            }
        }
    }
    return 0; //erreur
}

void archiveConv(utilisateur user, FILE *conv, char text[lenMaxPrompt]){
    fseek(conv, 0, SEEK_SET);
    history archive;
    int trouve = 0;

    while (fread(&archive, sizeof(history), 1, conv) != 0) {
        if (strcmp(user.nom, archive.userName) == 0) {
            trouve = 1;
            break;
        }
    }

    if (trouve) {
        fseek(conv, -(long)sizeof(history), SEEK_CUR);
    } else {
        memset(&archive, 0, sizeof(history));
        strncpy(archive.userName, user.nom, sizeof(archive.userName) - 1);
        archive.userName[sizeof(archive.userName) - 1] = '\0';
        archive.index = -1;
    }

    int index = archive.index + 1;
    if (index >= 2 * nbMaxEtape) {
        index = 2 * nbMaxEtape - 1;
    }
    archive.index = index;

    strncpy(archive.conv[index], text, lenMaxPrompt - 1);
    archive.conv[index][lenMaxPrompt - 1] = '\0';

    fwrite(&archive, sizeof(history), 1, conv);
    fflush(conv);

}

void afficherConv(FILE *conv){
    history stock;
    fseek(conv, 0, SEEK_SET);

    printf("=== DEBUG CONV.DAT ===\n");
    int n = 0;

    while (fread(&stock, sizeof(history), 1, conv) != 0) {
        printf("----- ENTREE %d -----\n", n++);
        printf("userName : '%s'\n", stock.userName);
        printf("index    : %d\n", stock.index);

        for (int i = 0; i < 2 * nbMaxEtape; i++) {
            printf("conv[%2d] : '%.40s'\n", i, stock.conv[i]);
        }
        printf("\n");
    }

    if (n == 0) {
        printf("(conv.dat est vide ou aucune entree lue)\n");
    }
}

void combat(int etapeId,char nomPNJ[20], FILE *hisinfoUsr,FILE *conv,FILE *histoire, utilisateur user){
    int PvPlayer;
    utilisateur userSto;
    srand(time(NULL));  // initialisation de l'aléatoire
    char reponse[50];
        fseek(hisinfoUsr, 0, SEEK_SET);
        // on va lire des utilisateurs du fichier un par un jusqu'à la fin du fichier
        while (fread(&userSto, sizeof(utilisateur), 1, hisinfoUsr) != 0)
        {
            if(strcmp(user.nom,userSto.nom)==0){
                PvPlayer=userSto.personnage.PV;
                //printf("trouve /n"); /for debug
            }
        }
        int pvPNJ=100;
        
            printf("%s vous attaque ! que voulez-vous faire fuir ou combattre ?\n",nomPNJ);
            getchar();// vide le buffer pour éviter que fgets pante
            fgets(reponse, sizeof(reponse), stdin); // on utilise fgets pour pouvoir saisir un texte avec des espaces
            archiveConv(user,conv,nomPNJ);
            archiveConv(user,conv,"vous attaque ! que voulez-vous faire fuir ou combattre ?\n");
            archiveConv(user,conv,reponse);
            if(traitementReponse(reponse)==4){
                //printf("pvPlayer %d, pvPNJ %d\n",PvPlayer,pvPNJ); for debug use only
                while(PvPlayer>0&& pvPNJ>0){
                    int degats= (rand() % 10) + 1;
                    PvPlayer=PvPlayer-degats;
                    printf("%s vous a infligé %d de dégats, votre PV et de %d que voulez-vous faire ? combattre ou fuir\n",nomPNJ,degats,PvPlayer);
                    char chaine[50];
                    snprintf(chaine,sizeof(chaine),"%s vous a infligé %d de dégats, votre PV et de %d que voulez-vous faire ? combattre ou fuir\n",nomPNJ,degats,PvPlayer);
                    archiveConv(user,conv,chaine);
                    getchar();// vide le buffer pour éviter que fgets pante
                    fgets(reponse, sizeof(reponse), stdin); // on utilise fgets pour pouvoir saisir un texte avec des espaces
                    if(traitementReponse(reponse)==4){
                        int degatsPNJ= (rand() % 10) + 1;
                        pvPNJ=pvPNJ-degatsPNJ;
                    printf("Vous avez infligé %d de dégat à %s sont PV est de %d\n",degatsPNJ,nomPNJ,pvPNJ);
                    char chaine2[50];
                    snprintf(chaine2,sizeof(chaine2),"Vous avez infligé %d de dégat à %s sont PV est de %d\n",degatsPNJ,nomPNJ,pvPNJ);
                    archiveConv(user,conv,chaine2);
                    }

                    else{
                        printf("Tapette Va !\n");
                        archiveConv(user,conv,"Tapette Va !\n");
                        user.personnage.histIndex=etapeId;
                        etapeRunning(user,conv,histoire,hisinfoUsr);
                    }
                }
                if(PvPlayer<=0){
                    printf("Vous êtes mort !! perdu !!\n");
                    archiveConv(user,conv,"Vous êtes mort !! perdu !!\n");
                    fclose(hisinfoUsr);
                    fclose(histoire);
                    fclose(conv);
                    exit(0);
                }
                if(pvPNJ<=0){
                    printf("Vous remportez le duel !!\n");
                    user.personnage.histIndex=etapeId;
                    etapeRunning(user,conv,histoire,hisinfoUsr);
                    archiveConv(user,conv,"Vous remportez le duel !!\n");
                }
            }
            else{
                printf("Tapette Va !\n");
                archiveConv(user,conv,"Tapette Va !\n");
                user.personnage.histIndex=etapeId;
                etapeRunning(user,conv,histoire,hisinfoUsr);
            }
        
}