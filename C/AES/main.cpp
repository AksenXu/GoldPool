#include "aes.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


typedef struct _StreamHeaderPrimary {
    short synccode;			//0xD1  0x1D (group separator)
    char type;				// video=0, audio=1,as fllow enum
    char header_len;		//start position of payload
    union{
    	int payload_time;    	//ms
    	int absolute_datetime;  //time_t,s
    } time;
    int payload_len;     	//rawdata length
    char sn[16];         	//unionid to match the apk
}StreamHeaderPrimary;

typedef struct _StreamHeaderExterntion {
    int gps[3];          	//latitudelongtitudemix, only appear when video keyframe to reduce bandwidth
}StreamHeaderExterntion;

int main(int argc, char* argv[])
{
	printf("main start\n");
	const char* filename = "20190418104502_p_MNzQMGdQqR9wJzerEr.media";
    const char* aesKey = "wQZus_Jo-F3VMM3Q";
	
	if(argc > 1) {
		filename = argv[1];
		aesKey = argv[2];
	}
	printf("filename = %s aesKey = %s\n", filename, aesKey);
	
    FILE* f = fopen(filename, "rb");
	
//	const char* aesKey = "vM2cYxqEChIZ9zhh";
//	FILE* f = fopen("20190402161018.v6ts", "rb");
	if(f == NULL) {
		printf("main fopen failed %s\n", strerror(errno));
		return -1;
	}
	FILE* fw_video = fopen("video.h264", "wb");
	if(fw_video == NULL) {
		printf("main fopen failed %s\n", strerror(errno));
		return -1;
	}
	FILE* fw_audio = fopen("audio.aac", "wb");
	if(fw_audio == NULL) {
		printf("main fopen failed %s\n", strerror(errno));
		return -1;
	}	
	StreamHeaderPrimary header;
	StreamHeaderExterntion header1;
	while(1) {
		int ret = fread(&header, sizeof(StreamHeaderPrimary), 1, f);	
		if(ret != 1) {
			break;
		}
		
		if(header.header_len != sizeof(StreamHeaderPrimary)) {
			fread(&header1, sizeof(StreamHeaderExterntion), 1, f);	
		}
		
		printf("synccoode %x type %d header_len %d\n", header.synccode, header.type, header.header_len);

		
		int payloadlen = ((header.payload_len + 15) & ~0xF);
		char* enc_data = (char*)malloc(payloadlen);
		char* dec_data = (char*)malloc(payloadlen);
		
		printf("payloadlen = %d\n", payloadlen);
		
		fread(enc_data, payloadlen, 1, f);
		aes_context ctx;
		aes_setkey_dec( &ctx, (unsigned char *)aesKey, 128);
		
		unsigned char iv[16];
        memcpy(iv, aesKey, sizeof(iv));
		
		aes_crypt_cbc( &ctx, AES_DECRYPT, payloadlen, iv, (const unsigned char*)enc_data, (unsigned char*)dec_data);
	
		if(header.type != 2) {
			fwrite(dec_data, payloadlen, 1, fw_video);
		} else {
            fwrite(dec_data, payloadlen, 1, fw_audio);
        }
		free(enc_data);
		free(dec_data);
	}
	fclose(fw_video);
	fclose(fw_audio);
	fclose(f);
	
	return 0;	
}
