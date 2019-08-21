#include "stdlib.h"
#include "stdio.h"

inline static unsigned short be16(const unsigned char* data) {
    unsigned short swap = ((unsigned short)data[0] << 8) | data[1];
    return swap;
}

inline static unsigned int be32(const unsigned char* data) {
    unsigned int swap = ((unsigned int)data[0] << 24) | ((unsigned int)data[1] << 16)
                            | ((unsigned int)data[2] << 8) | data[3];
    return swap;
}

inline static int64_t be64(const unsigned char* data) {
    unsigned int swap1 = be32(data);
    unsigned int swap2 = be32(data + 4);
    int64_t swap = ((int64_t)swap1 << 32) | swap2;
    return swap;
}

int main()
{
    long long data = 0x1234567890abcdef;
    
    long long swap = be64((unsigned char*)&data);
    
    printf("data = 0x%llx swap = 0x%llx\n", data, swap);
    return 0;
}