#include "SkStream.h"
#include "SkJpegUtility.h"
#include <setjmp.h>
extern "C" {
#include "jpeglib.h"
};
#include "carutils/VideoUtil.h"
#include "carutils/WAssert.h"
#include "JpegDecoder.h"

#include "carutils/WLogger.h"
static WLogger * myLog = wlogGet("@jpegdecoder");

struct my_error_mgr {
    struct jpeg_error_mgr pub;  /* "public" fields */
    jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

void my_error_exit (j_common_ptr cinfo)
{
    /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
    my_error_ptr myerr = (my_error_ptr) cinfo->err;
    char buffer[JMSG_LENGTH_MAX];
    /* Create the message */
    (*cinfo->err->format_message) (cinfo, buffer);

    /* Send it to stderr, adding a newline */
    WLOGE(myLog, "JpegDecode Error: %s\n", buffer);

    /* Return control to the setjmp point */
    longjmp(myerr->setjmp_buffer, 1);
}


static uint8_t s_default_dht[] = { 
  
         0xff,0xc4, /* DHT (Define Huffman Table) identifier */ 
         0x01,0xa2, /* section size: 0x01a2, from this two bytes to the end, inclusive */   
  
         0x00,                                                                                                                                  /* DC Luminance */ 
         0x00,0x01,0x05,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /* BITS         */ 
         0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,                                   /* HUFFVALS     */ 
  
         0x10,                                                                                                                                  /* AC Luminance */ 
         0x00,0x02,0x01,0x03,0x03,0x02,0x04,0x03,0x05,0x05,0x04,0x04,0x00,0x00,0x01,0x7d, /* BITS         */ 
         0x01,0x02,0x03,0x00,0x04,0x11,0x05,0x12,0x21,0x31,0x41,0x06,0x13,0x51,0x61,0x07, /* HUFFVALS     */ 
         0x22,0x71,0x14,0x32,0x81,0x91,0xa1,0x08,0x23,0x42,0xb1,0xc1,0x15,0x52,0xd1,0xf0, 
         0x24,0x33,0x62,0x72,0x82,0x09,0x0a,0x16,0x17,0x18,0x19,0x1a,0x25,0x26,0x27,0x28, 
         0x29,0x2a,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x43,0x44,0x45,0x46,0x47,0x48,0x49, 
         0x4a,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,0x63,0x64,0x65,0x66,0x67,0x68,0x69, 
         0x6a,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7a,0x83,0x84,0x85,0x86,0x87,0x88,0x89, 
         0x8a,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9a,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7, 
         0xa8,0xa9,0xaa,0xb2,0xb3,0xb4,0xb5,0xb6,0xb7,0xb8,0xb9,0xba,0xc2,0xc3,0xc4,0xc5, 
         0xc6,0xc7,0xc8,0xc9,0xca,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,0xd8,0xd9,0xda,0xe1,0xe2, 
         0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xea,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,
         0xf9,0xfa, 
  
         0x01,                                                                                                                                  /* DC Chrominance */ 
         0x00,0x03,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00, /* BITS           */ 
         0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,                                   /* HUFFVALS       */ 
  
         0x11,                                                                                                                                  /* AC Chrominance */ 
         0x00,0x02,0x01,0x02,0x04,0x04,0x03,0x04,0x07,0x05,0x04,0x04,0x00,0x01,0x02,0x77, /* BITS           */ 
         0x00,0x01,0x02,0x03,0x11,0x04,0x05,0x21,0x31,0x06,0x12,0x41,0x51,0x07,0x61,0x71, /* HUFFVALS       */ 
         0x13,0x22,0x32,0x81,0x08,0x14,0x42,0x91,0xa1,0xb1,0xc1,0x09,0x23,0x33,0x52,0xf0, 
         0x15,0x62,0x72,0xd1,0x0a,0x16,0x24,0x34,0xe1,0x25,0xf1,0x17,0x18,0x19,0x1a,0x26, 
         0x27,0x28,0x29,0x2a,0x35,0x36,0x37,0x38,0x39,0x3a,0x43,0x44,0x45,0x46,0x47,0x48, 
         0x49,0x4a,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,0x63,0x64,0x65,0x66,0x67,0x68, 
         0x69,0x6a,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7a,0x82,0x83,0x84,0x85,0x86,0x87, 
         0x88,0x89,0x8a,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9a,0xa2,0xa3,0xa4,0xa5, 
         0xa6,0xa7,0xa8,0xa9,0xaa,0xb2,0xb3,0xb4,0xb5,0xb6,0xb7,0xb8,0xb9,0xba,0xc2,0xc3, 
         0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,0xd8,0xd9,0xda, 
         0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xea,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8, 
         0xf9,0xfa 
}; 


static int isHuffmanExist(uint8_t *buf, uint32_t len)
{
    uint32_t i = 0;
    uint8_t *pbuf = buf;
    while (((pbuf[0] << 8) | pbuf[1]) != 0xffda) {
        if (i++ > len)
            return 0;
        
        if (((pbuf[0] << 8) | pbuf[1]) == 0xffc4) {
            return 1;
        }

        pbuf++;
    }

    return 0;
}

static int findTagPos(uint8_t *buf, uint32_t len, uint16_t target)
{
    uint32_t pos = 0;
    uint8_t *pbuf = buf;

    while (((pbuf[0] << 8) | pbuf[1]) != 0xffda) {
	    pos = pbuf - buf;
        if (((pbuf[0] << 8) | pbuf[1]) == target) {
			WLOGD(myLog, "target %x Found, Pos %d\n", target, pbuf - buf);
            return pos;
        }

        pbuf++;

        if(pos > len)
            return -1;
    }

    return pos;
}

int swjpegDecodeMemory(uint8_t *data, uint32_t size, uint8_t* out, int out_format)
{
    struct jpeg_decompress_struct cinfo;
    struct my_error_mgr jerr;
    uint32_t h_samp_factor = 1, v_samp_factor = 1;
	uint32_t chroma_width, chroma_height;
	uint32_t i = 0, j = 0;
    uint32_t luma_width, luma_height;

    uint8_t * YUV420 = (unsigned char *)out;
    uint8_t * yDstPtr = YUV420;
    uint8_t * uDstPtr = NULL; //depend on the color format
	uint8_t * vDstPtr = NULL;
    uint8_t * YUV420_align_16 = NULL;

    uint8_t * uPtr = NULL, *vPtr = NULL;

    uint8_t * YUV422 = NULL;
    uint8_t * YUV444 = NULL;

    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;
	
	// All objects need to be instantiated before this setjmp call so that
    // they will be cleaned up properly if an error occurs.
	/* Establish the setjmp return context for my_error_exit to use. */
    if (setjmp(jerr.setjmp_buffer)) {
        /* If we get here, the JPEG code has signaled an error.
        * We need to clean up the JPEG object, close the input file, and return.
        */
        jpeg_destroy_decompress(&cinfo);
        WLOGE(myLog, "JpegDecode Error Happen, Clean jpeg environment");
		if(YUV422) free(YUV422);
		if(YUV444) free(YUV444);
        return 0;
    }	
    
    jpeg_create_decompress(&cinfo);

	SkMemoryStream stream(data, size);
    skjpeg_source_mgr  sk_stream(&stream, NULL);
    cinfo.src = &sk_stream;

    
    //jpeg_mem_src(&cinfo, (unsigned char*)data, size);
    jpeg_read_header(&cinfo, TRUE);
    cinfo.dct_method = JDCT_IFAST;

	h_samp_factor = cinfo.comp_info[0].h_samp_factor;
	v_samp_factor = cinfo.comp_info[0].v_samp_factor;
	
    /*set parameters for decompression */
    cinfo.out_color_space = JCS_YCbCr;
	cinfo.raw_data_out = TRUE;
    cinfo.do_fancy_upsampling = FALSE;
	
	jpeg_start_decompress(&cinfo);

    luma_width  = carutils::VIDEO_ALIGN(cinfo.output_width, 16);
    luma_height = carutils::VIDEO_ALIGN(cinfo.output_height, 16);
    if(luma_height != cinfo.output_height) {
        YUV420_align_16 = (uint8_t*) malloc(luma_width * luma_height * 3 / 2);
        yDstPtr = YUV420_align_16;
    }

	chroma_width = luma_width / h_samp_factor;
	chroma_height = luma_height / v_samp_factor;

	JSAMPARRAY dst_buf[3];
    JSAMPROW   y_row[luma_height];
    JSAMPROW   u_row[luma_height];
    JSAMPROW   v_row[luma_height];
     
    dst_buf[0] = y_row;
    for(i = 0; i < luma_height; i++) {
        dst_buf[0][i] = yDstPtr + i * luma_width;
    }

	vDstPtr = yDstPtr + luma_width * luma_height;
    uDstPtr = vDstPtr + luma_width * luma_height / 4;
	
	if((v_samp_factor == 2) && (h_samp_factor == 2)) { 
        //YUV420, use the out buffer directly
        uPtr = uDstPtr;
        vPtr = vDstPtr;
	} else if((v_samp_factor == 1) && (h_samp_factor == 2)){ 
	    //YUV422, malloc temp UV buffer
		YUV422 = (unsigned char *) malloc(luma_width * luma_height);
		uPtr = YUV422;
	    vPtr = uPtr + chroma_width * chroma_height;
	} else if((v_samp_factor == 1) && (h_samp_factor == 1)) {
	    //YUV444, malloc temp UV buffer
		YUV444 = (unsigned char *) malloc(luma_width * luma_height * 2);
		uPtr = YUV444;
	    vPtr = uPtr + chroma_width * chroma_height;        
    } else 
		WASSERT(1 == 0)(v_samp_factor)(h_samp_factor);

	dst_buf[1] = u_row;
	for(i = 0; i < chroma_height;i++) {
		dst_buf[1][i] = uPtr + i * chroma_width;
	}
 
	dst_buf[2] = v_row;
	for(i = 0; i < chroma_height ;i++) {
		dst_buf[2][i] = vPtr + i * chroma_width;
	}

	int read_line = cinfo.max_v_samp_factor * cinfo.min_DCT_scaled_size;
	while(cinfo.output_scanline < cinfo.output_height){
        uint32_t rows = jpeg_read_raw_data(&cinfo, dst_buf, read_line);

        if(rows == 0) {
            WLOGE(myLog, "jpeg_read_raw_data Failed");
            break;
        }
		if(YUV422) {
            if(out_format == JPEG_YUV420_PLANAR) {
                //YUV422 output, and convert to YUV420 for display
    			for(i = 0; i < rows; i+=2) {
    				memcpy(uDstPtr, dst_buf[1][i], chroma_width);
    				uDstPtr += chroma_width;

    				memcpy(vDstPtr, dst_buf[2][i], chroma_width);
    				vDstPtr += chroma_width;
    			}
            } else if (out_format == JPEG_YCbCr420_SEMIPLANAR) {
    			for(i = 0; i < rows; i += 2) {
                    for(j = 0; j < chroma_width; j++) {
                        *vDstPtr ++ = dst_buf[1][i][j];
                        *vDstPtr ++ = dst_buf[2][i][j];
                    }
    			}
            } else if (out_format == JPEG_YCrCb420_SEMIPLANAR) {
    			for(i = 0; i < rows; i += 2) {
                    for(j = 0; j < chroma_width; j++) {
                        *vDstPtr ++ = dst_buf[2][i][j];
                        *vDstPtr ++ = dst_buf[1][i][j];
                    }
    			}
            }
		} else if(YUV444) {
            if(out_format == JPEG_YUV420_PLANAR) {
                //YUV444 output, and convert to YUV420 for display
    			for(i = 0; i < rows; i += 2) {
                    for(j = 0; j < chroma_width; j += 2) {
                        //FIXME: only for qrcode 8bit picture now, it's gray image
                        *uDstPtr ++ = 128;//dst_buf[1][i][j];
                        *vDstPtr ++ = 128;//dst_buf[2][i][j];
                    }
    			}
            } else if (out_format == JPEG_YCbCr420_SEMIPLANAR) {
    			for(i = 0; i < rows; i += 2) {
                    for(j = 0; j < chroma_width; j += 2) {
                        *vDstPtr ++ = dst_buf[1][i][j];
                        *vDstPtr ++ = dst_buf[2][i][j];
                    }
    			}
            } else if (out_format == JPEG_YCrCb420_SEMIPLANAR) {
    			for(i = 0; i < rows; i += 2) {
                    for(j = 0; j < chroma_width; j += 2) {
                        *vDstPtr ++ = dst_buf[2][i][j];
                        *vDstPtr ++ = dst_buf[1][i][j];
                    }
    			}
            }            
        }

        dst_buf[0] += rows;
        dst_buf[1] += rows / v_samp_factor;
        dst_buf[2] += rows / v_samp_factor;
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
	
	if(YUV422) free(YUV422);
    if(YUV444) free(YUV444);

    if(YUV420_align_16) {
        int y_size = cinfo.output_width*cinfo.output_height;
        int align_y_size = luma_width * luma_height;
        for(unsigned int i = 0; i < cinfo.output_height; i ++) {
            memcpy(out + i * cinfo.output_width, YUV420_align_16 + i * luma_width, cinfo.output_width);
        }
        
        unsigned char* outU = out + y_size;
        unsigned char* srcU = YUV420_align_16 + align_y_size;
        for(unsigned int i = 0; i < cinfo.output_height / 2; i ++) {
            memcpy(outU + i * cinfo.output_width / 2, srcU + i * luma_width / 2, cinfo.output_width / 2);
        }        

        unsigned char* outV = out + y_size + y_size / 4;
        unsigned char* srcV = YUV420_align_16 + align_y_size + align_y_size / 4;
        for(unsigned int i = 0; i < cinfo.output_height / 2; i ++) {
            memcpy(outV + i * cinfo.output_width / 2, srcV + i * luma_width / 2, cinfo.output_width / 2);
        }       
        
        free(YUV420_align_16);
    }
    return 0;
}


void jpegDecode(uint8_t* data, uint32_t length, uint32_t dataSize, uint8_t* out, int out_format)
{
    uint8_t * jpeg = NULL;
    if(!isHuffmanExist(data, dataSize)) {
        int pos = findTagPos(data, dataSize, 0xFFC0);
        if(pos != -1) {
            jpeg = (uint8_t *) malloc(dataSize + sizeof(s_default_dht));
			memcpy(jpeg, data, pos);
			memcpy(jpeg + pos, s_default_dht, sizeof(s_default_dht));
			memcpy(jpeg + pos + sizeof(s_default_dht), 
						data + pos, dataSize - pos);

            dataSize += sizeof(s_default_dht);
        }
        WLOGE(myLog, "Insert Huffman Table");
    } else {
        jpeg = data;
    }

    swjpegDecodeMemory(data, length, out, out_format);

    if(jpeg != data) {
        free(jpeg);
    }
    //MY_LOGD("jpegDecode Finished");
	return;
}
