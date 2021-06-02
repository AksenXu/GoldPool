#include <stdio.h>
#include <stdlib.h>

extern "C" {
    #include "amr/interf_enc.h"  
};

int main(int argc, char* argv[])
{
    if(argc != 3) {
        printf("usage: %s pcmFile amrFile\n", argv[0]);
        return 0;
    }
    
    const char* pcmFile = argv[1];
    const char* amrFile = argv[2];
    
    FILE* pcmFP = fopen(pcmFile, "rb");
    FILE* amrFP = fopen(amrFile, "wb");
    if(pcmFP == NULL || amrFP == NULL) {
        printf("open file faile\n");
        return -1;
    }
    
    void *encoder = Encoder_Interface_init(0);
    enum Mode req_mode = MR122;
    
    unsigned char pcmData[320] = {0};
    unsigned char amrData[32] = {0};
    while(1) {
         if(fread(pcmData, sizeof(pcmData), 1, pcmFP) != 1) {
             break;
         }
         
        int encodedBytes = Encoder_Interface_Encode(encoder, req_mode, (short *)pcmData, amrData, 0);
        if(encodedBytes > 0) {
            fwrite(amrData, encodedBytes, 1, amrFP);
        }
    }
    Encoder_Interface_exit(encoder);

    
    fclose(pcmFP);
    fclose(amrFP);
    
    return 0;
}