#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "parser.h"
#include "symbolTable.h"

// per semplicità usiamo delle semplici coppia chiave-valore, ma si potrebbero usare 
// strutture dati apposite e performanti come le hash maps (come symbolTable).
// A differenza delle hash maps, queste semplici mappe si scandiscono sequenzialmente.


// I valori sono numerici: in assembler.c (funzione output) tradurremo in "binario"

// La dimensione delle tabelle (cioè XXX qui sotto) è parte della struttura 
// solo per facilitare la scansione riga-per-riga: ovvero con un for da 
// indice 0 a quel numero-1. 
// Si potrebbe evitare di scriverlo hard-coded, ma andrebbe adattato il codice.

tabella compTable = {
    28, {
        {"0",     42},  // 101010
        {"1",     63},  // 111111
        {"-1",    58},  // 111010
        {"D",     12},  // 001100
        {"A",     48},  // 110000
        {"!D",    13},  // 001101
        {"!A",    49},  // 110001
        {"-D",    15},  // 001111
        {"-A",    51},  // 110011
        {"D+1",   31},  // 011111
        {"A+1",   55},  // 110111
        {"D-1",   14},  // 001110
        {"A-1",   50},  // 110010
        {"D+A",    2},  // 000010
        {"D-A",   19},  // 010011
        {"A-D",    7},  // 000111
        {"D&A",    0},  // 000000
        {"D|A",   21},  // 010101

        // M-based equivalents
        {"M",     48},  // 110000
        {"!M",    49},  // 110001
        {"-M",    51},  // 110011
        {"M+1",   55},  // 110111
        {"M-1",   50},  // 110010
        {"D+M",    2},  // 000010
        {"D-M",   19},  // 010011
        {"M-D",    7},  // 000111
        {"D&M",    0},  // 000000
        {"D|M",   21}   // 010101
    }
    
};

tabella jumpTable = {
    7, {  
    /* TODO */
    {"JGT", 1}, 
    {"JEQ", 2}, 
    {"JGE", 3},
    {"JLT", 4}, 
    {"JNE", 5}, 
    {"JLE", 6}, 
    {"JMP", 7}
    }
};

tabella destTable = {
    7, {   
    /* TODO */
    {"M",   1}, 
    {"D",   2}, 
    {"MD",  3},
    {"A",   4},
    {"AM",  5},
    {"AD",  6},
    {"AMD", 7}
    }
};

instruction *parseInstruction(char *riga) {

    riga = pulisciRiga(riga);
    
    if(strlen(riga) == 0) return NULL;

    if(riga[0] == '(') return NULL;

    if(riga[0] == '@') { //è una A-instruction

        if(riga[1] <= '9' && riga[1] >= '0') {
            instruction* A_instruction = (instruction*)malloc(sizeof(instruction));
            if (A_instruction == NULL) {
                return NULL;
            }
            unsigned short address;
            A_instruction->type = A;
            
            if (sscanf(riga, "@%hu", &(address)) != 1) {
                free(A_instruction);
                return NULL;
            }
            A_instruction->valore = address;
            
            return A_instruction;
	    /* TODO */
	    //creare l'istruzione
	    //assegnare il tipo A
	    //prendere l'indirizzo, per esempio con sscanf (vedi main) 
	    //se ok, testare il range (per es. usare shift): è a 15 bit?
	    //assegnare l'indirizzo e ritornare l'istruzione

        }
        
    }else{

	//le 3 parti di una C-instruction (vedi parser.h)
        unsigned char comp, dest, jump;
        parseC(riga, &comp, &dest, &jump);
     
        instruction* C_instruction = (instruction*)malloc(sizeof(instruction));
        

        if (C_instruction == NULL) {
            return NULL;
        }

        C_instruction->type = C;  
        if (jump != MISSING && jump != ERROR)
        {
            C_instruction->jump = jump;  
        }
        if (comp != MISSING && comp != ERROR)
        {
            C_instruction->comp = comp;  
        }
        C_instruction->dest = dest;  

        return C_instruction;
            
    /* TODO */
	//usare parseC: passare le 3 componenti per riferimento: così verranno settate da parseC
    //parseC può settare MISSING o ERROR in ciascuna componente
	//creare l'istruzione e ritornarla

    }

    //dummy, solo per compilare (questa è una funzione non-void)
    abort();

}

//fa il parsing della C-instruction ed assegna i valori alle 3 componenti
void parseC(char *riga, unsigned char *comp, unsigned char *dest, unsigned char *jump) {
    *dest = 0;
    *jump = 0;
    *comp = 0;

    // Handle jump part
    char *jumpPart = strchr(riga, ';');
    if(jumpPart != NULL) {
        *jumpPart = '\0';
        jumpPart++;
        jumpPart = rimuoviSpazi(jumpPart);
        *jump = traduciInBinario(jumpPart, &jumpTable);
        if(*jump == MISSING) {
            fprintf(stderr, "Invalid jump: %s\n", jumpPart);
            exit(1);
        }
    }

    // Handle dest and comp
    char *equals = strchr(riga, '=');
    if(equals != NULL) {
        *equals = '\0';
        char *destPart = rimuoviSpazi(riga);
        *dest = traduciInBinario(destPart, &destTable);
        if(*dest == MISSING) {
            fprintf(stderr, "Invalid dest: %s\n", destPart);
            exit(1);
        }

        char *compPart = rimuoviSpazi(equals + 1);
        int a_bit = 0;
        char temp[256];
        strncpy(temp, compPart, 255);
        char *p = temp;
        while(*p) {
            if(*p == 'M') {
                a_bit = 1;
                *p = 'A';
            }
            p++;
        }
        unsigned char comp_val = traduciInBinario(temp, &compTable);
        if(comp_val == MISSING) {
            fprintf(stderr, "Invalid comp: %s\n", compPart);
            exit(1);
        }
        *comp = (a_bit << 6) | comp_val;
    } else {
        char *compPart = rimuoviSpazi(riga);
        int a_bit = 0;
        char temp[256];
        strncpy(temp, compPart, 255);
        char *p = temp;
        while(*p) {
            if(*p == 'M') {
                a_bit = 1;
                *p = 'A';
            }
            p++;
        }
        unsigned char comp_val = traduciInBinario(temp, &compTable);
        if(comp_val == MISSING) {
            fprintf(stderr, "Invalid comp: %s\n", compPart);
            exit(1);
        }
        *comp = (a_bit << 6) | comp_val;
    }
}



int isInstruction(char *riga) {
    riga = pulisciRiga(riga);
    if(!riga || !*riga || riga[0] == '/') return 0;
    
    return (riga[0] == '@' || strchr(riga, '=') || strchr(riga, ';'));
}



//traduce, semplicemente cercando nelle tabelle
unsigned char traduciInBinario(char *chiave, tabella *tabella) {
    
    int i;
    for(i = 0; i < tabella->lunghezza; i++) {
        if(strcmp(chiave, tabella->righe[i].chiave) == 0) {
            return tabella->righe[i].valore;
        }
    }

    return MISSING;
}


char *pulisciRiga(char *riga) {

    riga = rimuoviSpazi(riga);
    char *commento = strstr(riga, "//");
    if(commento != NULL) {
        *commento = '\0'; //terminiamo la stringa
        riga = rimuoviSpazi(riga); //in caso ci fossero spazi prima del commento
    }

    return riga;
}


//rimuovere spazi all'inizio ed alla fine della stringa.
//in un paio di esercizi in classe abbiamo visto sia come eliminarli solo 
//all'nizio della stringa che come farlo anche all'interno. qui serve solo
//il trimming ad inizio e fine. per farlo alla fine segue un ragionamento
//simile a come farlo all'inizio della stringa.
char *rimuoviSpazi(char *str) { 
	/* TODO */
	char *start;
    start = str;
    while(*start == ' ')
    {
        start++;
    }
    //rimuovere spazi in testa
    char *end;
    end = str + strlen(str)-1;
    while (end>str && *end == ' ')
    {
        end--;
    }
    end++;
    *end = '\0';
    return start;
    //rimuovere spazi in coda
	//char *end;
	//end = str + strlen(str) - 1;
	//while(end > str && ... ) ...
	//...
}


//se l'ultimo carattere è un newline lo togliamo (terminando la stringa)
void rimuoviNewLine(char *str){
    size_t len = strlen(str);
    if(len > 0 && str[len-1] == '\n') {
        str[len-1] = '\0';
    }
}