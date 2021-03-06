#include <string.h>
#include <stdio.h>

typedef struct vcap300_osd_char {
    unsigned short font;         ///< font index
    unsigned char  fbitmap[36];  ///< font bitmap, 12 bits per row, total 18 rows.
} VCAP300_OSD_CHAR_T;

static VCAP300_OSD_CHAR_T VCAP300_OSD_Default_Char[] = {
    {
        0x20,    //Char:[ ]
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x21,    //Char:[!]
        {0x00,0x00,0x00,0x00,0x0c,0x00,0x1e,0x00,0x1e,0x00,0x1e,0x00,0x1e,0x00,0x1e,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x00,0x00,0x0c,0x00,0x0c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x22,    //Char:["]
        {0x00,0x00,0x00,0x00,0x33,0x00,0x33,0x00,0x33,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x23,    //Char:[#]
        {0x00,0x00,0x00,0x00,0x33,0x00,0x33,0x00,0x33,0x00,0x7f,0x80,0x7f,0x80,0x33,0x00,0x33,0x00,0x7f,0x80,0x7f,0x80,0x33,0x00,0x33,0x00,0x33,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x24,    //Char:[$]
        {0x0c,0x00,0x0c,0x00,0x1e,0x00,0x3f,0x00,0x73,0x80,0x70,0x00,0x38,0x00,0x1c,0x00,0x06,0x00,0x03,0x00,0x03,0x80,0x73,0x80,0x3f,0x00,0x1e,0x00,0x0c,0x00,0x0c,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x25,    //Char:[%]
        {0x00,0x00,0x70,0x00,0xdb,0x00,0xdb,0x00,0x76,0x00,0x06,0x00,0x0c,0x00,0x0c,0x00,0x18,0x00,0x18,0x00,0x33,0x80,0x36,0xc0,0x66,0xc0,0x63,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x26,    //Char:[&]
        {0x00,0x00,0x00,0x00,0x0e,0x00,0x1b,0x00,0x31,0x80,0x1b,0x00,0x0e,0x00,0x18,0x00,0x33,0xc0,0x61,0x80,0x61,0x00,0x63,0x00,0x3f,0x80,0x1e,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x27,    //Char:[']
        {0x00,0x00,0x00,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x28,    //Char:[(]
        {0x00,0x00,0x06,0x00,0x0c,0x00,0x18,0x00,0x10,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x10,0x00,0x18,0x00,0x0c,0x00,0x06,0x00,0x00,0x00,},
    },
    {
        0x29,    //Char:[)]
        {0x00,0x00,0x30,0x00,0x18,0x00,0x0c,0x00,0x04,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x04,0x00,0x0c,0x00,0x18,0x00,0x30,0x00,0x00,0x00,},
    },
    {
        0x2a,    //Char:[*]
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x61,0x80,0x33,0x00,0x1e,0x00,0x7f,0x80,0x1e,0x00,0x33,0x00,0x61,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x2b,    //Char:[+]
        {0x00,0x00,0x00,0x00,0x00,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x7f,0x80,0x7f,0x80,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x2c,    //Char:[,]
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0c,0x00,0x0c,0x00,0x04,0x00,0x08,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x2d,    //Char:[-]
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0x00,0x3f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x2e,    //Char:[.]
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0c,0x00,0x0c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x2f,    //Char:[/]
        {0x00,0x00,0x00,0x00,0x01,0x80,0x01,0x80,0x03,0x00,0x03,0x00,0x06,0x00,0x06,0x00,0x0c,0x00,0x0c,0x00,0x18,0x00,0x18,0x00,0x30,0x00,0x30,0x00,0x60,0x00,0x60,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x30,    //Char:[0]
        {0x00,0x00,0x00,0x00,0x1e,0x00,0x3f,0x00,0x73,0x80,0x61,0x80,0x61,0x80,0x6d,0x80,0x6d,0x80,0x61,0x80,0x61,0x80,0x73,0x80,0x3f,0x00,0x1e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x31,    //Char:[1]
        {0x00,0x00,0x00,0x00,0x0c,0x00,0x3c,0x00,0x3c,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x32,    //Char:[2]
        {0x00,0x00,0x00,0x00,0x1e,0x00,0x3f,0x00,0x73,0x80,0x01,0x80,0x03,0x80,0x07,0x00,0x0e,0x00,0x1c,0x00,0x38,0x00,0x70,0x00,0x7f,0x80,0x7f,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x33,    //Char:[3]
        {0x00,0x00,0x00,0x00,0x1e,0x00,0x3f,0x00,0x73,0x80,0x01,0x80,0x03,0x00,0x0e,0x00,0x0f,0x00,0x03,0x80,0x01,0x80,0x73,0x80,0x3f,0x00,0x1e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x34,    //Char:[4]
        {0x00,0x00,0x00,0x00,0x30,0x00,0x33,0x00,0x33,0x00,0x33,0x00,0x33,0x00,0x63,0x00,0x7f,0x80,0x7f,0x80,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x35,    //Char:[5]
        {0x00,0x00,0x00,0x00,0x7f,0x80,0x7f,0x80,0x60,0x00,0x60,0x00,0x7e,0x00,0x7f,0x00,0x03,0x80,0x01,0x80,0x01,0x80,0x73,0x80,0x3f,0x00,0x1e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x36,    //Char:[6]
        {0x00,0x00,0x00,0x00,0x1e,0x00,0x3f,0x00,0x73,0x80,0x60,0x00,0x60,0x00,0x6e,0x00,0x7f,0x00,0x73,0x80,0x61,0x80,0x73,0x80,0x3f,0x00,0x1e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x37,    //Char:[7]
        {0x00,0x00,0x00,0x00,0x3f,0x80,0x3f,0x80,0x01,0x80,0x03,0x00,0x06,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x38,    //Char:[8]
        {0x00,0x00,0x00,0x00,0x1e,0x00,0x3f,0x00,0x73,0x80,0x61,0x80,0x33,0x00,0x1e,0x00,0x3f,0x00,0x73,0x80,0x61,0x80,0x73,0x80,0x3f,0x00,0x1e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x39,    //Char:[9]
        {0x00,0x00,0x00,0x00,0x1e,0x00,0x3f,0x00,0x73,0x80,0x61,0x80,0x73,0x80,0x3f,0x80,0x1d,0x80,0x01,0x80,0x01,0x80,0x73,0x80,0x3f,0x00,0x1e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x3a,    //Char:[:]
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0c,0x00,0x0c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0c,0x00,0x0c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x3b,    //Char:[;]
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0c,0x00,0x0c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0c,0x00,0x0c,0x00,0x04,0x00,0x08,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x3c,    //Char:[<]
        {0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x06,0x00,0x0c,0x00,0x18,0x00,0x30,0x00,0x60,0x00,0x30,0x00,0x18,0x00,0x0c,0x00,0x06,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x3d,    //Char:[=]
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3e,0x00,0x3e,0x00,0x00,0x00,0x00,0x00,0x3e,0x00,0x3e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x3e,    //Char:[>]
        {0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x18,0x00,0x0c,0x00,0x06,0x00,0x03,0x00,0x01,0x80,0x03,0x00,0x06,0x00,0x0c,0x00,0x18,0x00,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x3f,    //Char:[?]
        {0x00,0x00,0x00,0x00,0x1e,0x00,0x3f,0x00,0x73,0x80,0x61,0x80,0x03,0x80,0x07,0x00,0x0e,0x00,0x0c,0x00,0x0c,0x00,0x00,0x00,0x0c,0x00,0x0c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x40,    //Char:[@]
        {0x00,0x00,0x00,0x00,0x1e,0x00,0x3f,0x00,0x61,0x80,0x61,0x80,0x67,0x80,0x6f,0x80,0x69,0x80,0x69,0x80,0x6f,0x80,0x67,0x80,0x60,0x00,0x3f,0x80,0x1f,0x80,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x41,    //Char:[A]
        {0x00,0x00,0x00,0x00,0x0c,0x00,0x1e,0x00,0x33,0x00,0x61,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x7f,0x80,0x7f,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x42,    //Char:[B]
        {0x00,0x00,0x00,0x00,0x7e,0x00,0x7f,0x00,0x61,0x80,0x61,0x80,0x61,0x80,0x7f,0x00,0x7f,0x00,0x61,0x80,0x61,0x80,0x61,0x80,0x7f,0x00,0x7e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x43,    //Char:[C]
        {0x00,0x00,0x00,0x00,0x1e,0x00,0x3f,0x00,0x73,0x80,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x73,0x80,0x3f,0x00,0x1e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x44,    //Char:[D]
        {0x00,0x00,0x00,0x00,0x7e,0x00,0x7f,0x00,0x63,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x63,0x80,0x7f,0x00,0x7e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x45,    //Char:[E]
        {0x00,0x00,0x00,0x00,0x7f,0x80,0x7f,0x80,0x60,0x00,0x60,0x00,0x60,0x00,0x7e,0x00,0x7e,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x7f,0x80,0x7f,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x46,    //Char:[F]
        {0x00,0x00,0x00,0x00,0x7f,0x80,0x7f,0x80,0x60,0x00,0x60,0x00,0x60,0x00,0x7e,0x00,0x7e,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x47,    //Char:[G]
        {0x00,0x00,0x00,0x00,0x1e,0x00,0x3f,0x00,0x73,0x80,0x60,0x00,0x60,0x00,0x60,0x00,0x67,0x80,0x67,0x80,0x61,0x80,0x73,0x80,0x3f,0x80,0x1d,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x48,    //Char:[H]
        {0x00,0x00,0x00,0x00,0x61,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x7f,0x80,0x7f,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x49,    //Char:[I]
        {0x00,0x00,0x00,0x00,0x3f,0x00,0x3f,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x3f,0x00,0x3f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x4a,    //Char:[J]
        {0x00,0x00,0x00,0x00,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x73,0x80,0x3f,0x00,0x1e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x4b,    //Char:[K]
        {0x00,0x00,0x00,0x00,0x63,0x00,0x67,0x00,0x6e,0x00,0x7c,0x00,0x78,0x00,0x70,0x00,0x78,0x00,0x7c,0x00,0x6e,0x00,0x67,0x00,0x63,0x80,0x61,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x4c,    //Char:[L]
        {0x00,0x00,0x00,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x7f,0x80,0x7f,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x4d,    //Char:[M]
        {0x00,0x00,0x00,0x00,0x61,0x80,0x61,0x80,0x73,0x80,0x73,0x80,0x7f,0x80,0x7f,0x80,0x6d,0x80,0x6d,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x4e,    //Char:[N]
        {0x00,0x00,0x00,0x00,0x61,0x80,0x71,0x80,0x71,0x80,0x79,0x80,0x79,0x80,0x6d,0x80,0x6d,0x80,0x67,0x80,0x67,0x80,0x63,0x80,0x63,0x80,0x61,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x4f,    //Char:[O]
        {0x00,0x00,0x00,0x00,0x1e,0x00,0x3f,0x00,0x73,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x73,0x80,0x3f,0x00,0x1e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x50,    //Char:[P]
        {0x00,0x00,0x00,0x00,0x7e,0x00,0x7f,0x00,0x63,0x80,0x61,0x80,0x61,0x80,0x63,0x80,0x7f,0x00,0x7e,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x51,    //Char:[Q]
        {0x00,0x00,0x00,0x00,0x1e,0x00,0x3f,0x00,0x73,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x67,0x80,0x77,0x80,0x3f,0x80,0x1d,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x52,    //Char:[R]
        {0x00,0x00,0x00,0x00,0x7e,0x00,0x7f,0x00,0x63,0x80,0x61,0x80,0x61,0x80,0x63,0x80,0x7f,0x00,0x7e,0x00,0x63,0x00,0x61,0x80,0x61,0x80,0x61,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x53,    //Char:[S]
        {0x00,0x00,0x00,0x00,0x1e,0x00,0x3f,0x00,0x73,0x80,0x70,0x00,0x38,0x00,0x1c,0x00,0x06,0x00,0x03,0x00,0x03,0x80,0x73,0x80,0x3f,0x00,0x1e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x54,    //Char:[T]
        {0x00,0x00,0x00,0x00,0x7f,0x80,0x7f,0x80,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x55,    //Char:[U]
        {0x00,0x00,0x00,0x00,0x61,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x73,0x80,0x3f,0x00,0x1e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x56,    //Char:[V]
        {0x00,0x00,0x00,0x00,0x61,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x33,0x00,0x1e,0x00,0x0c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x57,    //Char:[W]
        {0x00,0x00,0x00,0x00,0x61,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x6d,0x80,0x6d,0x80,0x6d,0x80,0x6d,0x80,0x6d,0x80,0x6d,0x80,0x7f,0x80,0x33,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x58,    //Char:[X]
        {0x00,0x00,0x00,0x00,0x61,0x80,0x61,0x80,0x33,0x00,0x33,0x00,0x1e,0x00,0x0c,0x00,0x0c,0x00,0x1e,0x00,0x33,0x00,0x33,0x00,0x61,0x80,0x61,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x59,    //Char:[Y]
        {0x00,0x00,0x00,0x00,0x61,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x33,0x00,0x1e,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x5a,    //Char:[Z]
        {0x00,0x00,0x00,0x00,0x7f,0x80,0x7f,0x80,0x01,0x80,0x03,0x80,0x07,0x00,0x0e,0x00,0x1c,0x00,0x38,0x00,0x70,0x00,0x60,0x00,0x7f,0x80,0x7f,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x5b,    //Char:[[]
        {0x00,0x00,0x00,0x00,0x1e,0x00,0x1e,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x1e,0x00,0x1e,0x00,0x00,0x00,},
    },
    {
        0x5c,    //Char:[\]
        {0x00,0x00,0x00,0x00,0x60,0x00,0x60,0x00,0x30,0x00,0x30,0x00,0x18,0x00,0x18,0x00,0x0c,0x00,0x0c,0x00,0x06,0x00,0x06,0x00,0x03,0x00,0x03,0x00,0x01,0x80,0x01,0x80,0x00,0x00,0x00,0x00,},
    },
    {
        0x5d,    //Char:[]]
        {0x00,0x00,0x00,0x00,0x1e,0x00,0x1e,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x1e,0x00,0x1e,0x00,0x00,0x00,},
    },
    {
        0x5e,    //Char:[^]
        {0x1e,0x00,0x33,0x00,0x40,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x5f,    //Char:[_]
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7f,0x80,0x7f,0x80,0x00,0x00,},
    },
    {
        0x60,    //Char:[`]
        {0x0c,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x61,    //Char:[a]
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3e,0x00,0x3f,0x00,0x01,0x80,0x3f,0x80,0x7f,0x80,0x61,0x80,0x61,0x80,0x7f,0x80,0x3f,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x62,    //Char:[b]
        {0x00,0x00,0x00,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x6e,0x00,0x7f,0x00,0x73,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x73,0x80,0x7f,0x00,0x6e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x63,    //Char:[c]
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1e,0x00,0x3f,0x00,0x73,0x80,0x60,0x00,0x60,0x00,0x60,0x00,0x73,0x80,0x3f,0x00,0x1e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x64,    //Char:[d]
        {0x00,0x00,0x00,0x00,0x01,0x80,0x01,0x80,0x01,0x80,0x1d,0x80,0x3f,0x80,0x73,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x73,0x80,0x3f,0x80,0x1d,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x65,    //Char:[e]
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1e,0x00,0x3f,0x00,0x73,0x80,0x61,0x80,0x7f,0x80,0x7f,0x80,0x70,0x00,0x3f,0x00,0x1e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x66,    //Char:[f]
        {0x00,0x00,0x00,0x00,0x0f,0x80,0x1f,0x80,0x18,0x00,0x18,0x00,0x7f,0x80,0x7f,0x80,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x67,    //Char:[g]
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1e,0x00,0x3f,0x00,0x73,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x73,0x80,0x3f,0x80,0x1d,0x80,0x01,0x80,0x03,0x80,0x3f,0x00,0x3e,0x00,},
    },
    {
        0x68,    //Char:[h]
        {0x00,0x00,0x00,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x6e,0x00,0x7f,0x00,0x73,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x69,    //Char:[i]
        {0x00,0x00,0x0c,0x00,0x0c,0x00,0x00,0x00,0x00,0x00,0x3c,0x00,0x3c,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x3f,0x00,0x3f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x6a,    //Char:[j]
        {0x00,0x00,0x06,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x1e,0x00,0x1e,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x0e,0x00,0x3c,0x00,0x38,0x00,},
    },
    {
        0x6b,    //Char:[k]
        {0x00,0x00,0x00,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x63,0x00,0x66,0x00,0x6c,0x00,0x78,0x00,0x70,0x00,0x7c,0x00,0x66,0x00,0x63,0x00,0x61,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x6c,    //Char:[l]
        {0x00,0x00,0x00,0x00,0x3c,0x00,0x3c,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x3f,0x00,0x3f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x6d,    //Char:[m]
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x6d,0x00,0x7f,0x80,0x6d,0x80,0x6d,0x80,0x6d,0x80,0x6d,0x80,0x6d,0x80,0x6d,0x80,0x6d,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x6e,    //Char:[n]
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x6e,0x00,0x7f,0x00,0x73,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x6f,    //Char:[o]
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1e,0x00,0x3f,0x00,0x73,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x73,0x80,0x3f,0x00,0x1e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x70,    //Char:[p]
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x6e,0x00,0x7f,0x00,0x73,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x73,0x80,0x7f,0x00,0x6e,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,},
    },
    {
        0x71,    //Char:[q]
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1d,0x80,0x3f,0x80,0x73,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x73,0x80,0x3f,0x80,0x1d,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,},
    },
    {
        0x72,    //Char:[r]
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x6f,0x80,0x7f,0x80,0x78,0x00,0x70,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x73,    //Char:[s]
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1e,0x00,0x3f,0x00,0x61,0x80,0x70,0x00,0x3f,0x00,0x03,0x80,0x61,0x80,0x3f,0x00,0x1e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x74,    //Char:[t]
        {0x00,0x00,0x00,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x3f,0x00,0x3f,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x1f,0x00,0x0f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x75,    //Char:[u]
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x61,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x73,0x80,0x3f,0x80,0x1d,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x76,    //Char:[v]
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x61,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x33,0x00,0x1e,0x00,0x0c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x77,    //Char:[w]
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x61,0x80,0x61,0x80,0x61,0x80,0x6d,0x80,0x6d,0x80,0x7f,0x80,0x7f,0x80,0x73,0x80,0x21,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x78,    //Char:[x]
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x61,0x80,0x73,0x80,0x3f,0x00,0x1e,0x00,0x0c,0x00,0x1e,0x00,0x3f,0x00,0x73,0x80,0x61,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x79,    //Char:[y]
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x61,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x61,0x80,0x73,0x80,0x3f,0x80,0x1d,0x80,0x01,0x80,0x01,0x80,0x3f,0x00,0x3e,0x00,},
    },
    {
        0x7a,    //Char:[z]
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7f,0x80,0x7f,0x80,0x03,0x00,0x06,0x00,0x0c,0x00,0x18,0x00,0x30,0x00,0x7f,0x80,0x7f,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x7b,    //Char:[{]
        {0x00,0x00,0x00,0x00,0x07,0x00,0x0c,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x30,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x0c,0x00,0x07,0x00,0x00,0x00,},
    },
    {
        0x7c,    //Char:[|]
        {0x00,0x00,0x00,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
    {
        0x7d,    //Char:[}]
        {0x00,0x00,0x00,0x00,0x38,0x00,0x0c,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x03,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x0c,0x00,0x38,0x00,0x00,0x00,},
    },
    {
        0x7e,    //Char:[~]
        {0x00,0x00,0x38,0x80,0x6d,0x80,0x47,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
    },
};
#define NO_OTHER_PRINTF

void print12x18Array(unsigned char fbitmap[36])
{
    for(int i = 0; i < 18; i ++) {
        unsigned short data = (unsigned short)fbitmap[i * 2] << 4 | fbitmap[i * 2 + 1] >> 4;
        for(int j = 0; j < 12; j++) {
            if(data & (1 << (11 - j))) {
                printf("+");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

void print18x12Array(unsigned char fbitmap[36])
{
    for(int i = 0; i < 12; i ++) {
        unsigned int data = (unsigned int)fbitmap[i * 3] << 10 | (unsigned int)fbitmap[i * 3 + 1] << 2 | fbitmap[i * 3 + 2] >> 6;
        for(int j = 2; j < 14; j++) {
            if(data & (1 << (17 - j))) {
                printf("+");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

void rotateBitmap(unsigned char fbitmap[36], 
            unsigned char rotateBitmap[36], int angle)
{
    unsigned short orgBitmap[18] = {0};
    for(int i = 0; i < 18; i++) {
        orgBitmap[i] = (unsigned short)fbitmap[2 * i] << 4 | (unsigned short)fbitmap[2 * i + 1] >> 4; //only use 12bit
    }
    
    unsigned int dstBitmap[12] = {0};
    for(int i = 0; i < 12; i++) {
        for(int j = 0; j < 18; j++) {
            if(90 == angle) {
                dstBitmap[i] |= ((orgBitmap[j] >> (11 - i)) & 1) << j;
            } else if( 270 == angle) {
                dstBitmap[i] |= ((orgBitmap[j] >> i) & 1) << (17 - j);
            }
        }
        
        rotateBitmap[3 * i] = dstBitmap[i] >> 10;
        rotateBitmap[3 * i + 1] = dstBitmap[i] >> 2;
        rotateBitmap[3 * i + 2] = (dstBitmap[i] & 0x3) << 6;
    }
}

void printfoutArray(unsigned char rotatebitmap[36])
{
#ifndef NO_OTHER_PRINTF
    printf("{");
    for(int i = 0; i < 36; i++) {
        printf("0x%x,", rotatebitmap[i]);
    }
    printf("},\n");
#endif

    //convert 18 * 12bit array to 12 * 12bit array, choose the 17 - 0 bit 13 - 2 bit
    printf("{");
    for(int i = 0; i < 12; i++) {
        unsigned int data = (unsigned int) rotatebitmap[3*i] << 16 | (unsigned int) rotatebitmap[3*i + 1] << 8 | rotatebitmap[3*i + 2];
        unsigned char data1 = (data >> 20) & 0xFF;
        unsigned char data2 = (data >> 12) & 0xFF;
        printf("0x%x,0x%x,", data1, data2);
    }
    printf("0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},\n");
    
}

void rotateFont(unsigned char fbitmap[36], int angle)
{
    //incoming bitmap is 36byte, 18short, one short is one line pixel.
    //but we only need width:12, so choose the high 12bits, for example: 0x12 0x34, 
    //we choose the 0x123 12bit for one line
 
    //incoming font is 12 * 18, we want to rotate to 18 * 12, it means 3bytes one line, and 12 line, it need 3byte*12 = 36bytes
    //the same size with incoming data

#ifndef NO_OTHER_PRINTF    
    printf("Original: ------------------------\n");
    print12x18Array(fbitmap);
#endif    
    unsigned char rotatebitmap[36];
    
    rotateBitmap(fbitmap, rotatebitmap, angle);
    
#ifndef NO_OTHER_PRINTF    
    printf("Rotage:   +++++++++++++++++++++++++\n");
    print18x12Array(rotatebitmap);
#endif    
    memcpy(fbitmap, rotatebitmap, sizeof(rotatebitmap));
    
    printfoutArray(rotatebitmap);
    return;
}

int main(int argc, char* argv[])
{
    printf("static VCAP300_OSD_CHAR_T VCAP300_OSD_Default_Char[] = {\n");
    for(int i = 0; i < sizeof(VCAP300_OSD_Default_Char) / sizeof(VCAP300_OSD_CHAR_T); i++) {
        printf("    {\n");
        printf("        0x%x,\n", VCAP300_OSD_Default_Char[i].font);
        printf("        ");
        rotateFont(VCAP300_OSD_Default_Char[i].fbitmap, 270);
        printf("    },\n");
    }
    printf("};\n");
    
    return 0;
}