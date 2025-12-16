#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib.h"

void admin(void);
FILE * CreationFichier(FILE *file, const char *nom);
void init(FILE *fichier,FILE*hisinfoUsr,FILE *histoire,FILE *conv);
void ecrireFichier(FILE *fichier,utilisateur user); // il sera peut-être nécessaire d'ajouter des structures dans les arguments d'entrée
void afficherFichier(FILE *fichier,FILE *histoire,FILE *conv,FILE*hisinfoUsr);

int main(void){
    FILE*histoire=NULL;
    FILE*conv=NULL;
    FILE*hisinfoUsr=NULL;
    histoire=CreationFichier(histoire,"histoire");
    //conv=CreationFichier(conv,"conv");
    //hisinfoUsr=CreationFichier(hisinfoUsr,"histInfoUsr");


    init(hisinfoUsr,hisinfoUsr,histoire,conv);
    afficherFichier(hisinfoUsr,histoire,conv,hisinfoUsr);


}

void init(FILE *fichier,FILE*hisinfoUsr,FILE *histoire,FILE *conv){
    utilisateur user;
    printf("Saisissez votre pseudo (20 caractères max):");
    scanf(" %s",user.nom);
    ecrireFichier(hisinfoUsr,user);
}

void admin(void){

}

void afficherFichier(FILE *fichier,FILE *histoire,FILE *conv,FILE*hisinfoUsr){
    if(fichier==hisinfoUsr){
        utilisateur user;
        fseek(fichier ,0,SEEK_SET);
	// on va lire des CLIENT du fichier un par un jusqua la fin du fichier
	while(fread(&user,sizeof(utilisateur),1,fichier)!=0){
        printf("nom utilisateur : %s\n",user.nom);
        printf("alignement : %d\n",user.personnage.alignement);
        printf("assets : %s, %s, %s\n",user.personnage.asset[0],user.personnage.asset[1],user.personnage.asset[2]);
        printf("étapes parcourues : \n");
        for(int i=0;i<100;i++){
            printf("%d\n",user.personnage.hist[i]);
        }
        printf("étape en cours : %d\n",user.personnage.histIndex);
    }
    }
    
}

void ecrireFichier(FILE *fichier,utilisateur user){
    fseek(fichier,0,SEEK_END);
    fwrite(&user,sizeof(utilisateur),1,fichier);
}

FILE * CreationFichier(FILE *file, const char *nom){
    char filename[25];  // Buffer for filename + ".dat" extension + null terminator
    
    // Safely copy the input name, leaving room for ".dat" extension
    strncpy(filename, nom, 20);     // Copy at most 20 characters
    filename[20] = '\0';             // Ensure null-termination
    strcat(filename, ".dat");        // Concatenate ".dat" to the buffer
    printf("%s", filename);
    file = fopen(filename, "a");
    fclose(file);
    file = fopen(filename, "r+");
    return file;
}