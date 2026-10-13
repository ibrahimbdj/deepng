#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "png.h"


int main(int argc, char* argv[]){

    if(argc < 3){
        fprintf(stderr, "User: png <command> <file>\n");
        exit(1);
    }

    if(strcmp(argv[1], "ispng") == 0){
        if(is_png(argv[2]) < 0) exit(1);
        else exit(0);
    }
    
    if(strcmp(argv[1], "hexcat") == 0){
        if(hexcat(argv[2]) < 0) exit(1);
        else exit(0);
    }

    return 0;
}