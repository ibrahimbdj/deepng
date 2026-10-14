#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void nprintf(const char* txt, int n);
int fread_wrapper(void* ptr, size_t size, size_t n, FILE* file);
uint32_t buf_to_uint32(unsigned char buf[], size_t size);

#endif