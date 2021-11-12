#include <stdio.h>
#include <string.h>
#include "zbar.h"

static zbar_image_t *image = NULL;
static zbar_image_scanner_t *scanner = NULL;

int qrcode_init(void)
{
	/* create a reader */
    scanner = zbar_image_scanner_create();
	if(!scanner){
		printf("zbar image scanner create failed!\n");
		return -1;
	}

	zbar_image_scanner_set_config(scanner, 0, ZBAR_CFG_ENABLE, 1);
	
	 /* wrap image data */
	image = zbar_image_create();
	if(!image){
		printf("zbar image create failed!\n");
		return -2;
	}

	zbar_image_set_format(image, zbar_fourcc('Y','8','0','0'));
		

	return 0;
}

int qrcode_deinit(void)
{
	 /* clean up */
	if(image) {
    	zbar_image_destroy(image);
        image = NULL;
    }
	
	if(scanner) {
    	zbar_image_scanner_destroy(scanner);
        scanner = NULL;
    }

	return 0;
}

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
int qrcode_parser(int width, int height, void *yuvrawframe, char *result,int result_len,int crop_w, int crop_h, int crop_x, int crop_y)
{
    //zbar_image_scanner_t *scanner = NULL;
    int ret = 0;

    /* create a reader */
    //scanner = zbar_image_scanner_create();
    if(!scanner || !image)
		return -1;
	
    /* configure the reader */
    //zbar_image_scanner_set_config(scanner, 0, ZBAR_CFG_ENABLE, 1);

    /* wrap image data */
    //zbar_image_t *image = zbar_image_create();

    //zbar_image_set_format(image, zbar_fourcc('Y','8','0','0'));

    zbar_image_set_size(image, width, height);
	if(crop_w && crop_h){
    	zbar_image_set_crop(image,crop_x,crop_y,crop_w,crop_h);
    }

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
            //printf("decoded %s symbol \"%s\"\n", zbar_get_symbol_name(typ), data);
            if(strlen(data) > result_len +1){
            	strncpy(result, data, result_len-1);
				result[result_len -1] = '\0';
			}else{
				strncpy(result, data, strlen(data)+1);
			}
        }
    }
    else {
        /* if qrcode decoding failure, return 1 and return empty string */
        strcpy(result, "");
        ret = 1;
    }

    /* clean up */
    //zbar_image_destroy(image);
    //zbar_image_scanner_destroy(scanner);
    
	return (ret);
}

