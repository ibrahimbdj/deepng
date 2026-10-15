#ifndef PNG_H
#define PNG_H

#include <stdio.h>

int is_png(FILE* file);
int hexcat(FILE* file);
int list(FILE* file);
int info(FILE* file);
int png_integrity(FILE* file);
int delmd(FILE* file, char* new_file);

#endif
