#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

const unsigned char png_signature[] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};

int is_png(const char* file){
    FILE* ofile = fopen(file, "r");

    if(ofile == NULL){
        fprintf(stderr, "fopen file failed: %s\n", strerror(errno));
        return -1;
    }

    unsigned char bytes[sizeof(png_signature)]; 

    if(fread(bytes, 1, sizeof(bytes), ofile) != sizeof(bytes)){
        fprintf(stderr, "read signature failed\n");
        return -1;
    }

    fclose(ofile);
    
    if(memcmp(png_signature, bytes, sizeof(png_signature)) != 0) {
        printf("not png\n");
        return -1;
    }

    printf("png\n");
    return 0;
}

int hexcat(const char* file){
    FILE* ofile = fopen(file, "r");

    if(ofile == NULL){
        fprintf(stderr, "fopen file failed: %s\n", strerror(errno));
        return -1;
    }

    unsigned char bytes[32];
    int rbytes = 0;

    do{
        rbytes = fread(bytes, 1, sizeof(bytes), ofile);

        for(int i = 0; i < rbytes; i++){
            printf("%02X ", bytes[i]);
        }
        printf("\n");        
    }while(rbytes == sizeof(bytes));

    if(ferror(ofile)){
        fprintf(stderr, "\nread failed: %s\n", strerror(errno));
    }

    return 0;
}