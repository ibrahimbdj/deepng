#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

void nprintf(const char* txt, int n){
    for(int i = 0; i < n; i++){
        printf("%s", txt);
    }
}

uint32_t buf_to_uint32(unsigned char buf[], size_t size){
    uint32_t ret = 0;

    for(size_t i = 0; i < size; i++){
        ret = (ret << 8) | buf[i];
    }
    return ret;
}

int fread_wrapper(void* ptr, size_t size, size_t n, FILE* file){
    if(fread(ptr, size, n, file) < n){
        if(feof(file)){
            fprintf(stderr, "fread failed: fichier tronqué\n");
        }else {
            fprintf(stderr, "fread failed: %s\n", strerror(errno));
        }
        return -1;
    }
    return 0;
}