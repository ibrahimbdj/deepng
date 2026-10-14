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

static char* color_name(uint8_t color_type){
    switch(color_type){
        case 0:
            return "Greyscale";
        case 2:
            return "Truecolor";
        case 3:
            return "Indexed-color";
        case 4:
            return "Greyscale with alpha";
        case 6:
            return "Truecolor with alpha";
    }

    return "Unknown Color type";
}

static char* compression_name(uint8_t compression_method){
    switch(compression_method){
        case 0:
            return "Deflate";
    }

    return "Unknown";
}

static char* interlace_name(uint8_t interlace_method){
    switch(interlace_method){
        case 0:
            return "No interlace";
        case 1:
            return "Adam7";
    }

    return "Unknown";
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

int info(FILE* file){
    if(is_png(file) < 0) {
        fprintf(stderr, "Wrong file type, please provide a PNG file\n");
        return -1;
    }
    
    uint32_t tmp32;
    uint8_t tmp8;  
    
    struct chunk* ihdr = get_chunk(file);
    if(ihdr == NULL) return -1;
    unsigned char buf[4];
    
    if(fseek(file, ihdr->data_offset, SEEK_SET) < 0){
        fprintf(stderr, "fseek failed: %s\n", strerror(errno));
        return -1;
    }

    if(fread_wrapper(buf, 1, sizeof(unsigned int), file)< 0) return -1;
    tmp32 = buf_to_uint32(buf, sizeof(buf));
    printf("width: %u\n", tmp32);

    if(fread_wrapper(buf, 1, sizeof(unsigned int), file) < 0) return -1;
    tmp32 = buf_to_uint32(buf, sizeof(buf));
    printf("height: %u\n", tmp32);

    if(fread_wrapper(&tmp8, 1, sizeof(uint8_t), file) < 0) return -1;
    printf("Bit depth: %u\n", tmp8);

    if(fread_wrapper(&tmp8, 1, sizeof(uint8_t), file) < 0)  return -1;
    printf("Color type: %s\n", color_name(tmp8));

    if(fread_wrapper(&tmp8, 1, sizeof(uint8_t), file) < 0)  return -1;
    printf("Compression method: %s\n", compression_name(tmp8));

    if(fread_wrapper(&tmp8, 1, sizeof(uint8_t), file) < 0) return -1;
    printf("Filter method: %u\n", tmp8);

    if(fread_wrapper(&tmp8, 1, sizeof(uint8_t), file) < 0) return -1;
    printf("Interlace method: %s\n", interlace_name(tmp8));

    printf("\n");

    return 0;
}