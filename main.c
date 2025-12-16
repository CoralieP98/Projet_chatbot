#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib.h"


int main(void){
    FILE*histoire=NULL;
    FILE*conv=NULL;
    FILE*hisinfoUsr=NULL;

printf("coucou c'est un test!")

}

void CreationFichier(FILE *histoire,FILE *conv,FILE*hisinfoUsr){

    histoire=fopen("database.bin","a");
	fclose(histoire);
	histoire=fopen("database.bin","r+");

    conv=fopen("database.bin","a");
	fclose(conv);
	conv=fopen("database.bin","r+");

    hisinfoUsr=fopen("database.bin","a");
	fclose(hisinfoUsr);
	hisinfoUsr=fopen("database.bin","r+");
    
}