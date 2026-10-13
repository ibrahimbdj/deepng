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

#endif