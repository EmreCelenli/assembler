#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "assembler.h"

int main(int argc, char *argv[]) {

    if(argc >= 2) 
    {
    	FILE *asmfile;
    	asmfile=fopen(argv[1], "r");

	if( asmfile==NULL ) {
		printf("Errore in apertura del file da leggere: %s\n", argv[1]);
		exit(1);
	}

    FILE *hackfile;
    char filename[64];

	strcpy(filename,argv[1]);

	//ci aspettiamo un .asm quindi rimuoviamo 3 chars. 
	//Potevamo essere più flessibili ed usare funzioni
	//come sscanf per parsare il filename con un format 
	//per separare nome del file ed estensione. Esempio:
	//sscanf(filename, "%s.%s", &filename, &estensione);
	//sscanf ritornare il numero di argomenti assegnati,
	//quindi va sempre testato se il numero sia quelloa atteso.

	filename[strlen(filename)-3] = '\0';
	strcat(filename, "hack"); //concatenazione
        	       	
    hackfile = fopen(filename, "w");

	//scrive direttamente sul file
    assemble(asmfile, hackfile);

    fclose(asmfile);
    fclose(hackfile);

   }else{
        printf("Usage: %s [filename]\n", argv[0]);
   }
}
