#ifndef _CODEGEN_H
#define _CODEGEN_H

#include <stdio.h>

void registers_freeall(void);

void cgpreamble(FILE *out_file);
void cgpostamble(FILE *out_file);
int cgload(int value, FILE *out_file);
int cgadd(int r1, int r2, FILE *out_file);
int cgmul(int r1, int r2, FILE *out_file);
int cgsub(int r1, int r2, FILE *out_file);
int cgdiv(int r1, int r2, FILE *out_file);
void cgprintint(int r, FILE *out_file);

#endif // _CODEGEN_H
