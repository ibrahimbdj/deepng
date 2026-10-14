#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include "chunk.h"
#include "utilities.h"

const unsigned char png_signature[] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};

static void list_title(){
    nprintf("---------------", 4);
    printf("\n");
    printf("|   OFFSET    |");
    printf("    TYPE      |");
    printf("    SIZE      |");
    printf("    CLASS     |\n");
    nprintf("---------------", 4);
    printf("\n");
}

static void list_line(struct chunk* c){
    int prn;

    printf("|");
    prn = printf("%ld", (c->data_offset-8));
    nprintf(" ", (13-prn));
    printf("|");
    prn = printf("%s", c->type);
    nprintf(" ", (14-prn));
    printf("|");
    prn = printf("%u", (c->data_len + 12));
    nprintf(" ", (14-prn));
    printf("|");
    if(isupper((unsigned char)c->type[0]) == 0) prn = printf("Ancillary");
    else prn = printf("Critical");
    nprintf(" ", (14-prn));
    printf("|");
    printf("\n");
    nprintf("---------------", 4);
    printf("\n");
}

int is_png(FILE* file){
    unsigned char bytes[sizeof(png_signature)]; 

    if(fread(bytes, 1, sizeof(bytes), file) != sizeof(bytes)) return -1;
    if(memcmp(png_signature, bytes, sizeof(png_signature)) != 0) return -1;

    return 0;
}

int hexcat(FILE* file){
    unsigned char bytes[32];
    int rbytes = 0;

    do{
        rbytes = fread(bytes, 1, sizeof(bytes), file);

        for(int i = 0; i < rbytes; i++){
            printf("%02X ", bytes[i]);
        }
        printf("\n");        
    }while(rbytes == sizeof(bytes));

    if(ferror(file)){
        fprintf(stderr, "\nFailed to read entirely: %s\n", strerror(errno));
    }

    return 0;
}

int list(FILE* file){
    if(is_png(file) < 0) {
        fprintf(stderr, "Wrong file type, please provide a PNG file\n");
        return -1;
    }
    
    list_title();

    struct chunk* c = get_chunk(file);
    if(c == NULL) return -1;

    while(strcmp(c->type, "IEND") != 0){
        list_line(c);
        free(c);
        c = get_chunk(file);
        if(c == NULL) return -1;
    }
    list_line(c);
    free(c);
    
    return 0;
}