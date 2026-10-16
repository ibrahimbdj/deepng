#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include "utilities.h"

struct chunk{
    uint32_t data_len;
    char type[5];
    uint64_t data_offset;
    unsigned char crc[4];
};

//static char chunk_type_nodel[] = {};

static int is_critical(struct chunk* c){
    return isupper((unsigned char)c->type[0]);
}

void chunk_list_title(){
    nprintf("---------------", 4);
    printf("\n");
    printf("|OFFSET       |");
    printf("TYPE          |");
    printf("SIZE          |");
    printf("CLASS         |\n");
    nprintf("---------------", 4);
    printf("\n");
}

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

void print_chunk(struct chunk* c, int cap){
    int prn;

    if(cap > 0) {
        nprintf("---------------", 4);
        printf("\n");
    }
    
    printf("|");
    prn = printf("%lu", (c->data_offset-8));
    nprintf(" ", (13-prn));
    printf("|");
    prn = printf("%s", c->type);
    nprintf(" ", (14-prn));
    printf("|");
    prn = printf("%u", (c->data_len + 12));
    nprintf(" ", (14-prn));
    printf("|");
    if(is_critical(c)) prn = printf("Critical");
    else prn = printf("Ancillary");
    nprintf(" ", (14-prn));
    printf("|");
    printf("\n");
    nprintf("---------------", 4);
    printf("\n");
}

int is_deletable(struct chunk* c){
    if(is_critical(c)) return 0;
    return 1;
}

int chunk_integrity(struct chunk* c){
    return c->data_len;
}