#include "stdio.h"
#include "errno.h"
#include <string.h>

void DecodeALaw(
        short *out, const unsigned char *in, int inSize) {
    while (inSize-- > 0) {
        int x = *in++;

        int ix = x ^ 0x55;
        ix &= 0x7f;

        int iexp = ix >> 4;
        int mant = ix & 0x0f;

        if (iexp > 0) {
            mant += 16;
        }

        mant = (mant << 4) + 8;

        if (iexp > 1) {
            mant = mant << (iexp - 1);
        }

        *out++ = (x > 127) ? mant : -mant;
    }
}


int main(int argc, char* argv[])
{
    printf("input file %s", argv[1]);
    

	const char* g711file = argv[1];
    const char* pcmfile = (argc > 2) ? argv[2] : "g711topcm.pcm";
	
	printf("g711file = %s pcmfile = %s\n", g711file, pcmfile);
	
    FILE* fr = fopen(g711file, "rb");
	if(fr == NULL) {
		printf("%s fopen failed %s\n", g711file, strerror(errno));
		return -1;
	}
    
	FILE* fw = fopen(pcmfile, "wb");
	if(fw == NULL) {
		printf("%s fopen failed %s\n", pcmfile, strerror(errno));
		return -1;
	}
	
    unsigned char g711data[4096];
    short pcmdata[4096];
    
	while(1) {
		int ret = fread(g711data, sizeof(g711data), 1, fr);	
		if(ret != 1) {
			break;
		}
        DecodeALaw(pcmdata, g711data, sizeof(g711data));
        
        fwrite(pcmdata, sizeof(pcmdata), 1, fw);
    }
    
    fclose(fr);
    fclose(fw);
    return 0;
}