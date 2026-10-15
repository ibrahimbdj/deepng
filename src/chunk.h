#ifndef CHUNK_H
#define CHUNK_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct chunk{
    uint32_t data_len;
    char type[5];
    uint64_t data_offset;
    unsigned char crc[4];
};

struct chunk* get_chunk(FILE* file);
void print_chunk(struct chunk* c, int cap);
int chunk_integrity(struct chunk* c);
int is_deletable(struct chunk* c);
void chunk_list_title();

#endif