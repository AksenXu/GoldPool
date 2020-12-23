#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    if(argc < 4) {
        printf("usage: %s input output [8/16/32]\n", argv[0]);
        return -1;
    }
    
    char* inputFile = argv[1];
    char* outputFile = argv[2];
    int bit_width = atoi(argv[3]);
    int number = 16, len = 0;
    void* memory = NULL;
    
    FILE* finput = fopen(inputFile, "rb");
    FILE* foutput = fopen(outputFile, "wb");

    if(bit_width == 8) {
        len = sizeof(char) * number;
        memory = malloc(len);
        fprintf(foutput, "char data[] = {\n");
    } else if(bit_width == 16){
        len = sizeof(short) * number;
        memory = malloc(len);
        fprintf(foutput, "short data[] = {\n");
    } else if(bit_width == 32){
        len = sizeof(int) * number;
        memory = malloc(len);
        fprintf(foutput, "int data[] = {\n");
    } else {
        printf("don't support bit_width %d, only support 8/16/32\n", bit_width);
        return -1;
    }

    printf("%s => %s bit_width %d\n", inputFile, outputFile, bit_width);
    while(1) {
        int ret = fread(memory, len, 1, finput);
        if(ret < 1) break;
        if(bit_width == 8) {
            char* data = (char*)memory;
            fprintf(foutput, "%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \n",
                    data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], 
                    data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);
        } else if(bit_width == 16){
            short* data = (short*)memory;
            fprintf(foutput, "%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \n",
                    data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], 
                    data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);
        } else if(bit_width == 32){
            int* data = (int*)memory;
            fprintf(foutput, "%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \n",
                    data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], 
                    data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);
        }       
    }
    fprintf(foutput, "};\n");
    
    fclose(finput);
    fclose(foutput);
    return 0;
}