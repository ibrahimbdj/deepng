#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include "utilities.h"
#include "chunk.h"

//static char chunk_type_nodelete[] = {};

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
    if(c == NULL) return c;
    unsigned char buf[4];

    if(fread_wrapper(buf, 1, sizeof(buf), file) < 0) goto fail;
    c->data_len = buf_to_uint32(buf, sizeof(buf));

    if(fread_wrapper(buf, 1, sizeof(buf), file) < 0) goto fail;
    memcpy(c->type, buf, sizeof(buf));
    c->type[4] = *("\0");

    long pos = ftell(file);
    if(pos < 0){
        fprintf(stderr, "ftell failed: %s\n", strerror(errno));
        goto fail;
    }
    c->data_offset = pos;

    if(fseek(file, c->data_len, SEEK_CUR) < 0){
        fprintf(stderr, "fseek failed: %s\n", strerror(errno));
        goto fail;
    }

    if(fread_wrapper(buf, 1, sizeof(buf), file) < 0) goto fail;
    memcpy(c->crc, buf, sizeof(buf));

    return c;

    fail:
        free(c);
        return NULL;
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

int chunk_integrity(struct chunk* c, FILE* file){
    long cur_pos = ftell(file);
    if(cur_pos < 0){
        fprintf(stderr, "ftell failed: %s\n", strerror(errno));
        return -2;
    }
    if(fseek_wrapper(file, (c->data_offset - 4), SEEK_SET) < 0) return -2;

    uint32_t crc = 0xFFFFFFFF;
    uint32_t polynome = 0xEDB88320;
    unsigned char byte;
    uint32_t nbytes = c->data_len + 4;

    while(nbytes > 0){
        if(fread_wrapper(&byte, 1, 1, file) < 0) return -2;

        for(int i = 0; i < 8; i++){
            
            if((byte & 1) ^ (crc & 1)) crc = (crc >> 1) ^ polynome;
            else crc = crc >> 1;
            byte = byte >> 1;
        }

        nbytes--;
    }

    if(fseek_wrapper(file, cur_pos, SEEK_SET) < 0) return -2;
    
    if((crc ^ 0xFFFFFFFF) == buf_to_uint32(c->crc, 4)) return 0;
    else return -1;
}