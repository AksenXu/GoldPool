#include <stdio.h>
#include <stdlib.h>
#include <zbar.h>
#include <string.h>
#include <testyuvrawframe.h>

/**
 * \brief                  Decode the QR code YUV raw frame
 *
 * \param width            the width of raw YUV frame
 * \param height           the height of raw YUV frame
 * \param yuvrawframe      the raw YUV frame data
 * \param result           the QR decoded text
 * 
 * \return                  0 if successful
 */
int viaqrcode(int width, int height, void *yuvrawframe, char *result)
{
    zbar_image_scanner_t *scanner = NULL;
    int ret = 0;

    /* create a reader */
    scanner = zbar_image_scanner_create();

    /* configure the reader */
    zbar_image_scanner_set_config(scanner, 0, ZBAR_CFG_ENABLE, 1);

    /* wrap image data */
    zbar_image_t *image = zbar_image_create();
    zbar_image_set_format(image, zbar_fourcc('Y','8','0','0'));
    zbar_image_set_size(image, width, height);
    zbar_image_set_data(image, yuvrawframe, width * height, zbar_image_free_data);

    /* scan the image for barcodes */
    int n = zbar_scan_image(scanner, image);

    /* extract results */
    const zbar_symbol_t *symbol = zbar_image_first_symbol(image);
    if (symbol != NULL) {
    	for(; symbol; symbol = zbar_symbol_next(symbol)) {
            /* do something useful with results */
            zbar_symbol_type_t typ = zbar_symbol_get_type(symbol);
            const char *data = zbar_symbol_get_data(symbol);
            //printf("decoded %s symbol \"%s\"\n",
            //   zbar_get_symbol_name(typ), data);
            strcpy(result, data);
        }
    }
    else {
        /* if qrcode decoding failure, return 1 and return empty string */
        strcpy(result, "");
        ret = 1;
    }

    /* clean up */
    zbar_image_destroy(image);
    zbar_image_scanner_destroy(scanner);

    return (ret);
}
#if 0
int main (int argc, char **argv)
{
    int width = 0, height = 0;
    void *raw = NULL;
    FILE *file;
    int yuvframesize, n, count = 0, ret, len;
    char result[256];
    unsigned char *p;

    if(argc > 2) return(1);

    // fixed frame size to 330 x 330
    width = 330;
    height = 330;
    yuvframesize = 330 * 330 * 3 / 2;
    raw = malloc(yuvframesize);

    if (argc == 1) {
        // get frame data from array
        p = raw;
        for (count = 0; count < yuvframesize; count++) {
            if (count < (width * height)) {
                // fill the U and V data from predefined array
   	        *p = TEST_YUV_RAW_FRAME1[count];
            }
            else {
                // fill the Y data to 0x80 (Y800 frame)
                *p = 0x80;
            }
            p++;
        }
    }
    else if (argc == 2) {
	// get frame data from file
        file = fopen(argv[1], "rb");
        if(!file) {
           printf("file open error!\n");
           return(1);
        }
        while (!feof(file)) {
            n = fread(raw, 1, yuvframesize, file);
            count += n;
    	}
    	printf("read %d bytes from file\n", count);

    	fclose(file);
    }
    else {
        return(1);
    }

    ret = viaqrcode(width, height, raw, result);

    if (ret == 0) {
        len = strlen(result);
        printf("viaqrcode decoded symbol len: %d, content: \"%s\"\n", len, result);
    }
    else {
	printf("viaqrcode decoding failure !!\n");
    }

    return(0);
}
#endif
