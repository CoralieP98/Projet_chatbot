#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib.h"

void admin(void);
FILE * CreationFichier(FILE *file,char nom[20]);
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

FILE * CreationFichier(FILE *file,char nom[20]){
    
    strcat(nom,".dat");
    printf("%s",nom);
    file=fopen(nom,"a");
    if(file != NULL){
        fclose(file);
    }
    file=fopen(nom,"r+");
    return file;


    
}