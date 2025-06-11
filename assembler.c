#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"
#include "parser.h"
#include "symbolTable.h"


//data è il *numero* corrispondente alla codifica binaria a 16 bit
void output(FILE *f, unsigned short data) {
    
    char temp[18];
	temp[17] = '\0'; 
	unsigned short tmp;
    for (int i = 15; i >= 0; i--) {
		tmp = data % 2;
		data = data / 2;
        if (tmp == 1)
		{
			temp[i] = '1';
		}
		
        else {
            temp[i] = '0'; 
        }
    }

    temp[16] = '\n';

    printf("in binary: %s", temp);
    fprintf(f, "%s", temp);
	
	//dobbiamo stampare in "binario" (ovvero sequenza di caratteri da scrivere nel file
	//.hack, che è sempre un file di testo) le cifre di data
        
    //essendo un file di testo, ad ogni riga dobbiamo ricordarci di 
    //inserire un carattere di newline sequito dal nul (fine stringa).

    /* TODO */
}


//legge il file, traduce, ed invoca la funzione output
void assemble(FILE *asmfile, FILE *hackfile) {

    char buf[256]; //max 256 chars per riga dal file
    
    //inizializzazione symbol table
    symbolTable st;
    initDefault(&st);

    //prima passata (vedi slides su assemblatore)
    //semplice implementazione. la struttura può essere migliorata. 
    
    while(fgets(buf, 256 , asmfile)!=NULL) {

		rimuoviNewLine(buf);
		
		printf("I read %s\n", buf);
         
        //completamento della symbol table  
		parseSymbols(buf, &st);

    }
 
    //riposiziona il puntatore di lettura del file all'inizio del file
    rewind(asmfile);

    //seconda passata (vedi slides su assemblatore)
    while(fgets(buf, 256 , asmfile)!=NULL) {

	rimuoviNewLine(buf);
    
    //usiamo la symbol table
	replaceSymbols(buf, &st);

	instruction* op;

	//da stringa buf a type instruction (che ha campi valore, dest, comp, jump, 
	//codificati già in binario - parte del progetto da completare: vedi parser.c).
 	op = parseInstruction(buf);


 	//data deve essere la codifica in binario finale di tutta l'instruction

	if(op != NULL) {
	    unsigned short data;

	    if(op->type == A) {
	    	data = op->valore;
                        	
	    }else{

		if(op->type == C){
			data = (0xE << 12) | (op->comp << 6) | (op->dest << 3) | op->jump;

		}
		//invece di prendere le codifiche in binario di dest,comp,jump
		//e concatenarle direttamente come stringhe (facendo attenzione che 
		//ciascuna abbia la lunghezza giusta), usiamo shift per calcolare il 
	    //valore di data. 

	    //poi la conversione in stringa avverrà nella funzione
	    //output (in questo modo separiamo la logica dell'assemblatore da 
	    //quella di scrittura sul file testuale .hack). 

		//formato C-instuctions: 111 a c1-c6 d1-d3 j1-j3
		//quindi 7 (cioè 0x7) shifted di 13 + comp di 6 + ...
		
		/* TODO */
		//data = (0x7 << 13) + 
		 

	   }

	   //scriviamo data sul file di destinazione

	   printf("number: %u\n", data);

	   output(hackfile, data);
	   free(op);

	}
    }

}




