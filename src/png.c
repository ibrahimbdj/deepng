#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/param.h>
#include "chunk.h"
#include "utilities.h"

const unsigned char png_signature[] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};


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

    struct chunk* c = get_chunk(file);
    if(c == NULL) return -1;

    chunk_list_title();

    while(strcmp(c->type, "IEND") != 0){
        print_chunk(c, 0);
        free(c);
        c = get_chunk(file);
        if(c == NULL) return -1;
    }
    print_chunk(c, 0);
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

    free(ihdr);
    return 0;
}

int delmd(FILE* file, char* new_file_name){
    if(is_png(file) < 0) {
        fprintf(stderr, "Wrong file type, please provide a PNG file\n");
        return -1;
    }

    if(fseek_wrapper(file, 0, SEEK_SET) < 0) return -1;

    char* cwd = getcwd(NULL, 0);
    if(cwd == NULL) {
        fprintf(stderr, "getcwd failed: %s\n", strerror(errno));
        return -1;
    }

    char new_file_path[strlen(cwd) + strlen(new_file_name) + 2];

    strcpy(new_file_path, cwd);
    strcat(strcat(new_file_path, "/"), new_file_name);
    free(cwd);

    FILE* new_file = fopen(new_file_name, "w+");
    if(new_file == NULL) return -1;

    char buf[1024];
    int read_bytes;
    int chunk_size;

    read_bytes = fread_wrapper(buf, 1, 8, file);
    if(read_bytes < 0)  return -1;
    if(fwrite_wrapper(buf, 1, read_bytes, new_file) < 0) return -1;

    struct chunk* c = get_chunk(file);
    if(c == NULL) goto fail;

    while(strcmp(c->type, "IEND") != 0){

        if(is_deletable(c) == 0){
            chunk_size = (c->data_len + 12);
            if(fseek(file, (c->data_offset - 8), SEEK_SET) < 0) goto fail;

            while(chunk_size != 0){
                read_bytes = fread_wrapper(buf, 1, MIN(sizeof(buf), chunk_size), file);
                if(read_bytes < 0) goto fail;
                if(fwrite_wrapper(buf, 1, read_bytes, new_file) < 0) goto fail;
                chunk_size = chunk_size - read_bytes;
            }

        }

        free(c);
        c = get_chunk(file);
        if(c == NULL) goto fail;
    }

    if(fseek_wrapper(file, (c->data_offset - 8), SEEK_SET) < 0) goto fail;
    read_bytes = fread_wrapper(buf, 1, (c->data_len + 12), file);
    if(read_bytes < 0)  goto fail;
    if(fwrite_wrapper(buf, 1, read_bytes, new_file) < 0) goto fail;

    free(c);
    if(fclose(new_file) < 0){
        fprintf(stderr, "Failed to close new file: %s\n", strerror(errno));
        return -1;
    }
    return 0;

    fail:
        free(c);
        if(fclose(new_file) < 0){
            fprintf(stderr, "Failed to close new file: %s\n", strerror(errno));
        }
        return -1;
}

int chunk(FILE* file, long offset){
    if(fseek_wrapper(file, offset, SEEK_SET) < 0) return -1;

    struct chunk* c = get_chunk(file);
    if(c == NULL) return -1;

    chunk_list_title();
    print_chunk(c, 0);
    free(c);
    
    return 0;
}

int png_integrity(FILE* file){
    if(is_png(file) < 0) {
        fprintf(stderr, "PNG signature corrupted\n");
        return -1;
    }

    int ret = 0;
    int tmp_ret = 0;
    struct chunk* c = get_chunk(file);
    if(c == NULL) return -1;

    while(strcmp(c->type, "IEND") != 0){
        tmp_ret = chunk_integrity(c, file);

        if(tmp_ret < 0){
            if(tmp_ret == -2) goto fail;
            printf("%lu\n", (c->data_offset - 8));
            ret = -1;
        }

        free(c);
        c = get_chunk(file);
        if(c == NULL) return -1;
    }

    if(chunk_integrity(c, file) < 0){
            printf("%lu\n", (c->data_offset - 8));
            ret = -1;
    }
    free(c);
    return ret;

    fail:
        free(c);
        return -2;
}