#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "png.h"


int main(int argc, char* argv[]){

    if(argc < 3){
        fprintf(stderr, "Use: pngfall <command> [args...]\n");
        goto fail;
    }

    FILE* file = fopen(argv[2], "r");
    if(file == NULL){
        fprintf(stderr, "Failed to open file: %s\n", strerror(errno));
        exit(1);
    }

    if(strcmp(argv[1], "ispng") == 0){
        if(is_png(file) < 0){
            printf("Not PNG\n");
            goto fail;
        }
        
        printf("PNG\n");
        goto success;
    }
    
    if(strcmp(argv[1], "cat") == 0){
        if(hexcat(file) < 0) goto fail;
        else goto success;
    }

    if(strcmp(argv[1], "list") == 0){
        if(list(file) < 0) goto fail;
        goto success;
    }

    if(strcmp(argv[1], "info") == 0){
        if(info(file) < 0) goto fail;
        goto success;
    }

    if(strcmp(argv[1], "integrity") == 0){
        if(png_integrity(file) < 0) goto fail;
        goto success;
    }

    if(strcmp(argv[1], "delmd") == 0){
        if(argc < 4){
            printf("Use: pngfall delmd <src> <dst>\n");
            goto fail;
        }
        if(delmd(file, argv[3]) < 0) goto fail;
        goto success;
    }

    if(strcmp(argv[1], "chunk") == 0){
        if(argc < 4){
            printf("Use: pngfall chunk <file> <chunk_offset>\n");
            goto fail;
        }
        if(chunk(file, atol(argv[3])) < 0) goto fail;
        goto success;
    }

    else {
        fprintf(stderr, "Unknown command: pngfall help\n");
        goto fail;
    }

    success:
        fclose(file);
        exit(0);
    
    fail:
        fclose(file);
        exit(1);
}