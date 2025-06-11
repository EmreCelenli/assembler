#ifndef PARSER_H
#define PARSER_H

//definisco due costanti che uso per capire se parti delle 
//C-instructions sono assenti o malformate. Scelgo valori a caso che
//non possono essere confusi con valori possibili di dest/comp/jump 
#define MISSING 0xff 
#define ERROR 0xef

typedef enum {A, C} optype;

typedef struct {
    optype type;	//il tipo
    
    //potremmo usare int, ma uns short = 16 bit, e uns char = 8 bit
    unsigned short valore;	//15 bit indirizzo (per A-instructions)
    unsigned char comp;		//6+1 bit per comp
    unsigned char dest;		//3 bit per dest
    unsigned char jump;		//3 bit per jump
} instruction;

typedef struct {
    char *chiave;
    unsigned short valore; //invece di int
} coppia_nome_valore;

typedef struct {
    int lunghezza;
    coppia_nome_valore righe[];
} tabella;



instruction* parseInstruction(char *riga);

void parseC(char *line, unsigned char *comp, unsigned char *dest, unsigned char *jump);

static unsigned char traduciInBinario(char *chiave, tabella *tabella);

int isInstruction(char *line);

char *pulisciRiga(char *line);
char *rimuoviSpazi(char *str);
void rimuoviNewLine(char *str);

#endif
