#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "symbolTable.h"

const int numDefaultSymbols = 23;

const symbol defaultSymbols[] = {
      {0, "SP"},  {1, "LCL"},  {2, "ARG"},
      {3, "THIS"}, {4, "THAT"},   {0, "R0"},
      {1, "R1"},   {2, "R2"},   {3, "R3"},
      {4, "R4"},   {5, "R5"},   {6, "R6"},
      {7, "R7"},   {8, "R8"},   {9, "R9"},
    {10, "R10"}, {11, "R11"}, {12, "R12"},
    {13, "R13"}, {14, "R14"}, {15, "R15"},
    {0x4000, "SCREEN"},   {0x6000, "KBD"}
};

void parseSymbols(char *line, symbolTable *st) {
    static unsigned short labelAddr = 0;
    
    line = pulisciRiga(line);
    
    // Skip empty lines and comments
    if(line == NULL || line[0] == '\0' || line[0] == '/') {
        return;
    }
    
    if(isInstruction(line)) {
        labelAddr++;
        return;
    }

    if(line[0] == '(') {
        char *end = strchr(line, ')');
        if(end != NULL) {
            *end = '\0';
            line++;  // Skip '('        
            addSymbol(line, labelAddr, st);
            return;
        }
    }

    fprintf(stderr, "Syntax error in line (not a valid instruction or label): %s\n", line);
    exit(1);
}


/* Sostituisce un simbolo in istruzioni assembly con l'indirizzo in tabella */

void replaceSymbols(char *line, symbolTable *st) {
    /* TODO */
    // I simboli non cominciano con numeri! Vedi slides.
    // Se è un simbolo (non un indirizzo), cercalo in tabella altrimenti aggiungilo.
    // In entrambi i casi, applica la sostituzione con sprintf
    /*
    static unsigned short varaddr = 16;

    line = pulisciRiga(line);
    if (line[0] == '@') {
        if (line[1] < '0' || line[1] > '9') {
            char symbol;
            sscanf(line, "@%s", &symbol);
            unsigned short address = findBySymbol(&symbol, st);
            
            if (address != MISSING)
            {
                sprintf(line, "@%hu", address);
            }else{
                addSymbol(&symbol, varaddr, st);
                sprintf(line, "@%hu", varaddr);
                varaddr = varaddr + 1;
            }
            
        }
    }
    */
  
    /* TODO */
    // I simboli non cominciano con numeri! Vedi slides.
    // Se è un simbolo (non un indirizzo), cercalo in tabella altrimenti aggiungilo.
    // In entrambi i casi, applica la sostituzione con sprintf

    static unsigned short varaddr = 16;

    line = pulisciRiga(line);
    if (line[0] == '@') {
        if (line[1] < '0' || line[1] > '9') {
            char symbol[256];
            if(sscanf(line, "@%s", symbol) != 1) {
                fprintf(stderr, "Syntax error: %s\n", line);
                exit(1);
            }
            unsigned short address = findBySymbol(symbol, st);
            
            if (address != MISSING) {
                sprintf(line, "@%hu", address);
            } else {
                addSymbol(symbol, varaddr, st);
                sprintf(line, "@%hu", varaddr);
                varaddr++;
            }
        }
    }
}





/* Cerca un simbolo in tabella, tramite hashvalue */

unsigned short findBySymbol(char *name, symbolTable *st) {
    int h = hash(name, st->tableLen);
    while(strlen(st->symbols[h].name) != 0 && strcmp(st->symbols[h].name, name))
        h = h + 1 % st->tableLen;

    if(strlen(st->symbols[h].name) == 0) {
        return MISSING;
    }

    return st->symbols[h].address;
}

/* Aggiunge un simbolo in tabella */

void addSymbol(const char *name, unsigned short address, symbolTable *st) {
    int h = hash(name, st->tableLen);
    while(strlen(st->symbols[h].name) != 0)
        h = (h + 1) % st->tableLen;
    
    symbol sym;
    sym.address = address;
    strcpy(sym.name, name);
    
    st->symbols[h] = sym;

    if(st->size++ >= st->tableLen/2) expand(st);
}


/* per espansione tabella e hashing */



void initDefault(symbolTable *st) {
    st->symbols = calloc(INITIAL_SIZE, sizeof(symbol));
    st->tableLen = INITIAL_SIZE;
    st->size = 0;

    st->symbols = calloc(INITIAL_SIZE, sizeof(symbol));

    copySymbols(st, defaultSymbols, numDefaultSymbols);
}

void copySymbols(symbolTable *dest, const symbol *src, const size_t srclen) {
    int i;
    for(i = 0; i < srclen; i++) {
        if(strlen(src[i].name) != 0) {
            addSymbol(src[i].name, src[i].address, dest);
        }
    }
}

void expand(symbolTable *st) {
    symbol *tableVals = st->symbols;
    size_t oldLen = st->tableLen;

    st->tableLen = (st->tableLen * 2) + 1;
    st->symbols = calloc(st->tableLen, sizeof(symbol));
    st->size = 0;

    copySymbols(st, tableVals, oldLen);

    free(tableVals);
}

size_t hash(const char *key, size_t M) { //genera hash values

    int h, a = 31415, b = 27183;

    for(h = 0; *key != 0; key++)
        h = (a*h + *key) % M;
    
    return (h < 0) ? (h + M) : h;
}


