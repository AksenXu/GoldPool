#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "tjpgd.h"

struct tjpg_priv
{
    const char* jpegData;
    int jpegOffset;
    int jpegLen;
    char* yuvBuffer;
};

int readFile(const char* filename, char* buffer)
{
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("open %s error\n", filename);
        return -1;
    }
    struct stat s;
    lstat(filename, &s);
    
    int size = s.st_size;
    fread(buffer, size, 1, fp);
    fclose(fp);
    return size;
}

static size_t tjpg_input(JDEC* jd, uint8_t* buffer, size_t size)
{
    struct tjpg_priv* tjpg = (struct tjpg_priv*) jd->device;
    
    printf("size %zd offset %d len %d buffer %p\n", 
            size, tjpg->jpegOffset, tjpg->jpegLen, buffer);

    if(tjpg->jpegOffset + size > tjpg->jpegLen) {
        size = tjpg->jpegLen - tjpg->jpegOffset;
    }
    
    if(buffer) {
        memcpy(buffer, tjpg->jpegData + tjpg->jpegOffset, size);
    }
    
    tjpg->jpegOffset += size;
    
    printf("size %zd offset %d len %d buffer %p done\n", 
            size, tjpg->jpegOffset, tjpg->jpegLen, buffer);
    return size;
}

static int callbackTimes = 0;
static int tjpg_output(JDEC* jd, void* data, JRECT* rect)
{
    callbackTimes ++;

    printf("l %d r %d t %d b %d\n", rect->left, rect->right, rect->top, rect->bottom);
    return 1;
}

int tjpg_decode_buffer(const char *jpgData, int length, int width, int height, void* yuvBuffer)
{
	JDEC jd;		/* Decompression object (70 bytes) */
    struct tjpg_priv tjpg;
    tjpg.jpegData = jpgData;
    tjpg.jpegOffset = 0;
    tjpg.jpegLen = length;
    tjpg.yuvBuffer = yuvBuffer;

    callbackTimes = 0;

    size_t sz_pool = 32 * 1024; //FIXME: enough?
    void* pool = malloc(sz_pool);

    JRESULT rc = jd_prepare(&jd, tjpg_input, pool, sz_pool, &tjpg);
    if(JDR_OK == rc) {
        rc = jd_decomp(&jd, tjpg_output, 0);
        if(JDR_OK != rc) {
            printf("jd_decomp failed rc: %d\n", rc);
        }
    } else {
        printf("jd_prepare failed rc: %d\n", rc);
    }

    free(pool);

    return rc == JDR_OK ? 0 : -1;
}


int main()
{
    char* buffer = (char*) malloc(1024*1024);
    int size = readFile("./test.jpg", buffer);
    
    void* yuvBuffer = malloc(640*480*3/2);
    
    tjpg_decode_buffer(buffer, size, 640, 480, yuvBuffer);

    free(yuvBuffer);
    free(buffer);
    return 0;
}
