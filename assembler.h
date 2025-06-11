#ifndef ASSEMBLER_H
#define ASSEMBLER_H

void output(FILE *asmfile, unsigned short data);

void assemble(FILE *asmfile, FILE *hackfile);

#endif
