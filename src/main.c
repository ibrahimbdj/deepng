#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "png.h"


int main(int argc, char* argv[]){

    if(argc < 3){
        fprintf(stderr, "User: png <command> <file>\n");
        exit(1);
    }

    FILE* file = fopen(argv[2], "r");
    if(file == NULL){
        fprintf(stderr, "Failed to open file: %s\n", strerror(errno));
        exit(1);
    }

    if(strcmp(argv[1], "ispng") == 0){
        if(is_png(file) < 0){
            printf("Not PNG\n");
            exit(1);
        }
        
        printf("PNG\n");
        exit(0);
    }
    
    if(strcmp(argv[1], "hexcat") == 0){
        if(hexcat(file) < 0) exit(1);
        else exit(0);
    }

    if(strcmp(argv[1], "list") == 0){
        if(list(file) < 0) exit(1);
        exit(0);
    }

    else {
        fprintf(stderr, "Unknown command: pngfall help\n");
        exit(1);
    }

    fclose(file);

    return 0;
}