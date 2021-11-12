#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <qrcode_api.h>

int main(int argc, char* argv[])
{
    char result[256] = {0};
    if(argc != 4) {
        printf("usage: %s file width height\n", argv[0]);
        return 0;
    }
    
    const char* filename = argv[1];
    int width = atoi(argv[2]);
    int height = atoi(argv[3]);
    int size = width * height * 3 / 2;
    
    qrcode_init();
    
    FILE* fp = fopen(filename, "rb");
    if(!fp) {
        printf("fopen %s failed %s\n", filename, strerror(errno));
        return -1;
    }
    
    char* yuvdata = (char*) malloc(size);
    fread(yuvdata, size, 1, fp);
    fclose(fp);
    
    int ret = qrcode_parser(width, height, yuvdata,
                    result, sizeof(result), width, height, 0, 0);
    
    printf("ret %d result %s\n", ret, result);
    
    free(yuvdata);
    return 0;
}