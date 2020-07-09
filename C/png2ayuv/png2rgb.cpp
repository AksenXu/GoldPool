#include "png2rgb.h"
#include "libpng/png.h"

// https://www.cnblogs.com/mengfanrong/p/3801583.html

struct ImageSource
{
    unsigned char* pngData;
    int pngLen;
    int offset;    
};

static void
pngReadCallback(png_structp png_ptr, png_bytep data, png_size_t length)
{
    struct ImageSource* source = (struct ImageSource*)png_get_io_ptr(png_ptr);
    if (source != NULL)
    {
        memcpy(data, source->pngData + source->offset, length);
        source->offset += length;
 
        if (source->offset > source->pngLen)
        {
            png_error(png_ptr, "Read Error!");
        }
    }
}

int png_decode_to_argb(unsigned char* pngData, int pngLen, struct ARGB& argb)
{
    int ret = -1;
    png_byte        header[PNGSIGSIZE]   = {0}; 
    png_structp     png_ptr     =   0;
    png_infop       info_ptr    = 0;
    AYUV_LOG("png_decode_to_ayuv: pngLen %d\n", pngLen);

    do {
        // png header len is 8 bytes
        CC_BREAK_IF(pngLen < PNGSIGSIZE);

        // check the data is png or not
        memcpy(header, pngData, PNGSIGSIZE);
        CC_BREAK_IF(png_sig_cmp(header, 0, PNGSIGSIZE));

        // init png_struct
        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
        CC_BREAK_IF(!png_ptr);

        // init png_info
        info_ptr = png_create_info_struct(png_ptr);
        CC_BREAK_IF(!info_ptr);

#if (CC_TARGET_PLATFORM != CC_PLATFORM_BADA && CC_TARGET_PLATFORM != CC_PLATFORM_NACL)
        //设置异常处理
        CC_BREAK_IF(setjmp(png_jmpbuf(png_ptr)));
#endif

        // set the read call back function
        ImageSource imageSource;
        imageSource.pngData    = (unsigned char*)pngData;
        imageSource.pngLen    = pngLen;
        imageSource.offset  = 0;
        png_set_read_fn(png_ptr, &imageSource, pngReadCallback);

        // read png file info
        png_read_info(png_ptr, info_ptr);

        //查询图像信息
        argb.width = png_get_image_width(png_ptr, info_ptr);
        argb.height = png_get_image_height(png_ptr, info_ptr);
        png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
        png_uint_32 color_type = png_get_color_type(png_ptr, info_ptr);

        AYUV_LOG("png_decode_to_argb: png res %dx%d\n", argb.width, argb.height);

        //调色板格式的png图片，转化为RGB888的像素格式
        // force palette images to be expanded to 24-bit RGB
        // it may include alpha channel
        if (color_type == PNG_COLOR_TYPE_PALETTE)
        {
            png_set_palette_to_rgb(png_ptr);
        }
        //像素格式少于1字节长度的灰度图，将其转为每像素占1字节的像素格式
        // low-bit-depth grayscale images are to be expanded to 8 bits
        if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        {
            bit_depth = 8;
            png_set_expand_gray_1_2_4_to_8(png_ptr);
        }
        //将tRNS块数据信息扩展为完整的ALPHA通道信息 
        // expand any tRNS chunk pngData into a full alpha channel
        if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
        {
            png_set_tRNS_to_alpha(png_ptr);
        }  
        //将16位输入降为8位
        // reduce images with 16-bit samples to 8 bits
        if (bit_depth == 16)
        {
            png_set_strip_16(png_ptr);            
        } 

        // Expanded earlier for grayscale, now take care of palette and rgb
        if (bit_depth < 8)
        {
            png_set_packing(png_ptr);
        }
        //更新png数据的具体信息
        // update info
        png_read_update_info(png_ptr, info_ptr);
        bit_depth = png_get_bit_depth(png_ptr, info_ptr);
        color_type = png_get_color_type(png_ptr, info_ptr);

        switch (color_type)
        {
        case PNG_COLOR_TYPE_GRAY:
            break;
        case PNG_COLOR_TYPE_GRAY_ALPHA:
            break;
        case PNG_COLOR_TYPE_RGB:
            break;
        case PNG_COLOR_TYPE_RGB_ALPHA:
            break;
        default:
            break;
        }
        AYUV_LOG("png_decode_to_ayuv: color_type %lu PNG_COLOR_TYPE_RGB_ALPHA %d\n", color_type, PNG_COLOR_TYPE_RGB_ALPHA);

        //按行读取png信息，
        // read png pngData
        png_size_t rowbytes;
        png_bytep* row_pointers = (png_bytep*)malloc( sizeof(png_bytep) * argb.height );

        //获取每一行像素的字节数量
        rowbytes = png_get_rowbytes(png_ptr, info_ptr);

        //申请内存地址
        argb.size = rowbytes * argb.height;
        AYUV_LOG("png_decode_to_ayuv: rowbytes %lu ayuv.height %d ayuv.size %d\n", rowbytes, argb.height, argb.size);

        argb.argb_addr = static_cast<unsigned char*>(malloc(argb.size * sizeof(unsigned char)));
        if (!argb.argb_addr)
        {
            if (row_pointers != NULL)
            {
                free(row_pointers);
            }
            break;
        }

        for (unsigned short i = 0; i < argb.height; ++i)
        {
            row_pointers[i] = argb.argb_addr + i*rowbytes;
        }
        //读取png数据
        png_read_image(png_ptr, row_pointers);
        //结束读取数据
        png_read_end(png_ptr, NULL);

        // premultiplied alpha for RGBA8888
        if (color_type == PNG_COLOR_TYPE_RGB_ALPHA)
        {
            //预乘Alpha。使用渐变Alpha
            //premultipliedAlpha();
        }
        else
        {
            //_hasPremultipliedAlpha = false;
        }

        if (row_pointers != NULL)
        {
            //释放图片数据的内存
            free(row_pointers);
        }

        ret = 0;
    } while (0);

    if (png_ptr)
    {
        //释放png_ptr
        png_destroy_read_struct(&png_ptr, (info_ptr) ? &info_ptr : 0, 0);
    }
    return ret;
}