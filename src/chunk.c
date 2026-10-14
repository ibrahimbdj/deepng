#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include "utilities.h"

struct chunk{
    uint32_t data_len;
    char type[5];
    uint64_t data_offset;
    unsigned char crc[4];
};

struct chunk* get_chunk(FILE* file){
    struct chunk* c = malloc(sizeof(struct chunk));
    unsigned char buf[4];

    if(fread_wrapper(buf, 1, sizeof(buf), file) < 0) return NULL;
    c->data_len = buf_to_uint32(buf, sizeof(buf));

    if(fread_wrapper(buf, 1, sizeof(buf), file) < 0) return NULL;
    memcpy(c->type, buf, sizeof(buf));
    c->type[5] = *("\0");

    long pos = ftell(file);
    if(pos < 0){
        fprintf(stderr, "ftell failed: %s\n", strerror(errno));
        return NULL;
    }
    c->data_offset = pos;

    if(fseek(file, c->data_len, SEEK_CUR) < 0){
        fprintf(stderr, "fseek failed: %s\n", strerror(errno));
        return NULL;
    }

    if(fread_wrapper(buf, 1, sizeof(buf), file) < 0) return NULL;
    memcpy(c->crc, buf, sizeof(buf));

    return c;
}