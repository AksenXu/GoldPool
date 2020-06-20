#ifndef _GOLD_POOL_FILE_UTIL_H_
#define _GOLD_POOL_FILE_UTIL_H_
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

inline int getFileSize(const char* filename)
{
    struct stat sb;
	if(stat(filename, &sb)) {
        printf("getFileSize: stat file %s error %s \n", filename, strerror(errno));
		return 0;
    }
    return sb.st_size;
}

inline bool writeFile(const char* file, const char* content, int len, int append = 0)
{
    if(content == NULL) {
        printf("writeFile: content is NULL\n");
        return false;
    }

    FILE * fp = NULL;
    if(append) {
        fp = fopen(file, "ab+");
    } else {
        fp = fopen(file, "wb");
    }
    
    if(!fp) {
        printf("writeFile: open file %s wb failed: %s\n", file, strerror(errno));
        return false;
    }

    if(len < 0) {
        len = strlen(content);
    }

    int n = fwrite(content, 1, len, fp);
    fclose(fp);

    if(n != len){
        printf("writeFile: write %s return %d != %d\n", file, n, len);
        return false;
    }

    printf("writeFile: write %d to file %s\n", len, file);
    return true;
}

/**
 * Read not too big file to memroy *buffer
 * @param[in] file: absolute file path
 * @param[out] buffer: pointer to store the file content, malloc inside and free outside
 * @return > 0: read OK, -1: read failed
 */
inline int readFile(const char* file, unsigned char** buffer)
{
    int len = getFileSize(file);
    *buffer = NULL;
    if(len == 0) {
        printf("readFile: %s is empty file\n", file);
        return -1;
    }

    *buffer = (unsigned char*) malloc(len);
    if(*buffer == NULL) {
        printf("readFile: malloc %d failed\n", len);
        return -1;
    }

    FILE* fp = fopen(file, "rb");
    if(fp != NULL) {
        int readitem = fread(*buffer, len, 1, fp);
        fclose(fp);
        if(readitem != 1) {
            printf("readFile: fread %s len %d failed %s\n", file, len, strerror(errno));
            return -1;
        }
    } else {
        printf("readFile: fopen %s failed %s\n", file, strerror(errno));
        return -1;
    }

    return len;
}
#endif