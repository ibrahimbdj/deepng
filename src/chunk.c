#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

struct chunk{
    uint32_t data_len;
    char type[5];
    uint64_t data_offset;
    unsigned char crc[4];
};

uint32_t buf_to_uint32(/*unsigned char buf[]*/){
    uint32_t ret = 0;
    return ret;
}

struct chunk* get_chunk(FILE* file){
    struct chunk* c = malloc(sizeof(struct chunk));
    unsigned char buf[4];

    fread(buf, 1, sizeof(buf), file);
    c->data_len = buf_to_uint32(/*buf*/);

    fread(buf, 1, sizeof(buf), file);
    memcpy(c->type, buf, sizeof(buf));
    c->type[5] = *("\0");

    long pos = ftell(file);
    if(pos < 0){
        fprintf(stderr, "ftell failed: %s\n", strerror(errno));
        return NULL;
    }
    c->data_offset = pos;

    fread(buf, 1, sizeof(buf), file);
    memcpy(c->crc, buf, sizeof(buf));

    return c;
}