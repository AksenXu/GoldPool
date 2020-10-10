
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include<malloc.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_STROKER_H
#include FT_IMAGE_H
#define WIDTH   80
#define HEIGHT  60
 
 
#define uint8 unsigned char
#define uint16 unsigned short
/* origin is the upper left corner */
unsigned char image[HEIGHT][WIDTH];
 
struct Pixel32
{
	uint8 a;
	uint8 r;
	uint8 g;
	uint8 b;
};
 
struct Span {
	int x;
	int y;
	int width;
	int coverage;
};
 
typedef struct Lspan {
	struct Span node;
	struct Lspan *next;
}Node, *PNode;
 
struct Rect {
	float xmin;
	float xmax;
	float ymin;
	float ymax;
};
 
struct vec2 {
	float x;
	float y;
};
 
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
void Include(const struct vec2 *r, struct Rect *rect)
{
	rect->xmin = MIN(rect->xmin, r->x);
	rect->ymin = MIN(rect->ymin, r->y);
	rect->xmax = MAX(rect->xmax, r->x);
	rect->ymax = MAX(rect->ymax, r->y);
}
 
void RasterCallback(const int y,
		const int count,
		const FT_Span * const spans,
		void * const user)
{
	PNode sptr = (PNode)user;
	while (sptr->next != NULL)
		sptr = sptr->next;
	int i;
	for (i = 0; i < count; ++i) {
		PNode newNode = (PNode) calloc(sizeof(Node), 1);
		if (!newNode) {
			printf("failed to alloc new node\n");
			break;
		}
		newNode->next = NULL;
		newNode->node.x = spans[i].x;
		newNode->node.y = y;
		newNode->node.width = spans[i].len;
		newNode->node.coverage = spans[i].coverage;
		sptr->next = newNode;
		sptr = sptr->next;
	}
}
 
void RenderSpans(FT_Library *library,
		FT_Outline *const outline,
		Node *spans)
{
	FT_Raster_Params params;
	memset(&params, 0, sizeof(params));
	params.flags = FT_RASTER_FLAG_AA | FT_RASTER_FLAG_DIRECT;
	params.gray_spans = RasterCallback;
	params.user = spans;
	FT_Outline_Render(*library, outline, &params);
}
 
#pragma pack(push)
#pragma pack(1)
struct TGAHeader
{
  uint8   idLength,           // Length of optional identification sequence.
          paletteType,        // Is a palette present? (1=yes)
          imageType;          // Image data type (0=none, 1=indexed, 2=rgb,
                              // 3=grey, +8=rle packed).
  uint16  firstPaletteEntry,  // First palette index, if present.
          numPaletteEntries;  // Number of palette entries, if present.
  uint8   paletteBits;        // Number of bits per palette entry.
  uint16  x,                  // Horiz. pixel coord. of lower left of image.
          y,                  // Vert. pixel coord. of lower left of image.
          width,              // Image width in pixels.
          height;             // Image height in pixels.
  uint8   depth,              // Image color depth (bits per pixel).
          descriptor;         // Image attribute flags.
};
#pragma pack(pop)
 
int WriteTGA(const char *filename,
	const struct Pixel32 *pxl,
	uint16 width,
	uint16 height)
{
	int ret = 0;
	FILE *fp = fopen(filename, "wb+");
	if (fp) {
		struct TGAHeader header;
		memset(&header, 0, sizeof(struct TGAHeader));
		header.imageType  = 2;
		header.width = width;
		header.height = height;
		header.depth = 32;
		header.descriptor = 0x20;
 
		fwrite(&header, sizeof(struct TGAHeader), 1, fp);
		/* printf("sizeof TGAHeader: %ld\n", sizeof(struct TGAHeader)); */
		fwrite(pxl, sizeof(struct Pixel32), width * height, fp);
	} else {
		printf("open file error\n");
		ret = -1;
	}
 
	if (fp)
		fclose(fp);
	return ret;
}
 
void gen_tga(FT_Face *face, FT_Library *library, char ch) {
	FT_UInt gindex;
	FT_GlyphSlot slot;
	FT_Stroker stroker;
	FT_Error error;
	PNode sp = NULL;
	PNode olsp = NULL;
	PNode sph = NULL;
	PNode olsph = NULL;
 
	/* use 50pt at 100dpi */
	error = FT_Set_Char_Size(*face, 100<<6, 100<<6, 90, 90);  /* set character size */
	/* error handling omitted */
	if (error) {
		printf("FT_Set_Char_Size error\n");
		goto exit;
	}
 
	slot = (*face)->glyph;
	gindex = FT_Get_Char_Index(*face, ch);
 
	error = FT_Load_Glyph(*face, gindex, FT_LOAD_NO_BITMAP);
	if (error) {
		printf("FT_Load_Glyph error\n");
		goto exit;
	}
 
	if (slot->format == FT_GLYPH_FORMAT_OUTLINE) {
		// Render the basic glyph to a span list.
 
		PNode sp = (PNode)malloc(sizeof(Node));
		if (!sp) {
			printf("failed to malloc Node\n");
			goto exit;
		}
		memset(&(sp->node), 0, sizeof(struct Span));
		sp->next = NULL;
		RenderSpans(library, &(slot->outline), sp);
 
		// Next we need the spans for the outline.
		PNode olsp = (PNode)malloc(sizeof(Node));
		if (!olsp) {
			printf("failed to malloc olsp\n");
			goto exit;
		}
		memset(&(olsp->node), 0, sizeof(struct Span));
		olsp->next = NULL;
 
		error = FT_Stroker_New((*library), &stroker);
		FT_Stroker_Set(stroker,
				3 * 64,
				FT_STROKER_LINECAP_ROUND,
				FT_STROKER_LINEJOIN_ROUND,
				0);
 
		FT_Glyph glyph;
		if (FT_Get_Glyph(slot, &glyph) == 0) {
			error = FT_Glyph_StrokeBorder(&glyph, stroker, 0, 1);
			/* printf("error = %d\n", error); */
			// Again, this needs to be an outline to work.
			if (glyph->format == FT_GLYPH_FORMAT_OUTLINE) {
				// Render the outline spans to the span list
				FT_Outline *o = &(((FT_OutlineGlyph)glyph)->outline);
				RenderSpans(library, o, olsp);
			}
			// Clean up afterwards.
			FT_Stroker_Done(stroker);
			FT_Done_Glyph(glyph);
 
			// Now we need to put it all together.
 
 
			do {
				// Figure out what the bounding rect is for both the span lists.
				struct Rect rect;
				memset(&rect, 0, sizeof(struct Rect));
				PNode ptr = NULL;
 
				sph = sp->next;
				olsph = olsp->next;
				if (!sph || !olsph)
					break;
 
				rect.xmin = sph->node.x;
				rect.xmax = sph->node.y;
				rect.ymin = sph->node.x;
				rect.ymax = sph->node.y;
 
				for(ptr = sph; ptr != NULL; ptr = ptr->next) {
					struct vec2 vc1, vc2;
					vc1.x = ptr->node.x;
					vc1.y = ptr->node.y;
					vc2.x = ptr->node.x + ptr->node.width - 1;
					vc2.y = ptr->node.y;
 
					Include(&vc1, &rect);
					Include(&vc2, &rect);
				}
 
				for (ptr = olsph; ptr != NULL; ptr = ptr->next) {
					struct vec2 vc1, vc2;
					vc1.x = ptr->node.x;
					vc1.y = ptr->node.y;
					vc2.x = ptr->node.x + ptr->node.width - 1;
					vc2.y = ptr->node.y;
 
					Include(&vc1, &rect);
					Include(&vc2, &rect);
				}
 
				int imgWidth = rect.xmax - rect.xmin + 1;
				int imgHeight = rect.ymax - rect.ymin + 1;
				int imgSize = imgWidth * imgHeight;
				printf("imgHeight=%d\n", imgHeight);
				printf("imgWidth=%d\n", imgWidth);
				printf("imgSize=%d\n", imgSize);
 
				// Allocate data for our image and clear it out to transparent.
				/* struct Pixel32 pxl[imgSize]; */
				struct Pixel32 *pxl;
				pxl = (struct Pixel32 *)malloc(sizeof(struct Pixel32) * imgSize);
				memset(pxl, 0, sizeof(struct Pixel32) * imgSize);
 
				// Loop over the outline spans and just draw them into the image.
				int w;
				for (ptr = olsph; ptr != NULL; ptr = ptr->next)
					for (w = 0; w < ptr->node.width; ++w) {
						int m = (int)((imgHeight - 1 - (ptr->node.y - rect.ymin)) * imgWidth +
								ptr->node.x - rect.xmin + w);
						pxl[m].r = 255;
						pxl[m].g = 255;
						pxl[m].b = 255;
						pxl[m].a = ptr->node.coverage;
					}
 
				for (ptr = sph; ptr != NULL; ptr = ptr->next)
					for (w = 0; w < ptr->node.width; ++w) {
						struct Pixel32 src;
						int m = (int)((imgHeight - 1 - (ptr->node.y - rect.ymin)) * imgWidth +
								ptr->node.x - rect.xmin + w);
						src.r = 255;
						src.g = 90;
						src.b = 30;
						src.a = ptr->node.coverage;
						pxl[m].r = (int)(pxl[m].r + ((src.r - pxl[m].r) * src.a) / 255.0f);
						pxl[m].g = (int)(pxl[m].g + ((src.g - pxl[m].g) * src.a) / 255.0f);
						pxl[m].b = (int)(pxl[m].b + ((src.b - pxl[m].b) * src.a) / 255.0f);
						pxl[m].a = MIN(255, pxl[m].a + src.a);
					}
 
				// Dump the image to disk.
				char fn[50];
				memset(fn, 0, 30);
				sprintf(fn, "/mnt/sd/freetype/out%c.tga", ch);
				printf("outfile: %s\n", fn);
				WriteTGA(fn, pxl, imgWidth, imgHeight);
 
				free(pxl);
				pxl = NULL;
			} while(0);
		}
	}
 
exit:
	while(sp) {
		PNode pre = sp;
		sp = sp->next;
		free(pre);
		pre = NULL;
	}
 
	while(olsp) {
		PNode pre = olsp;
		olsp = olsp->next;
		free(pre);
		pre = NULL;
	}
}
 
 
int main(int argc, char** argv)
{
	FT_Library library;
	FT_Face face;
	FT_Error error;
 
	char *filename;
	char *text;
	int n, num_chars;
 
	if (argc != 3) {
		fprintf(stderr, "usage: %s font sample-text\n", argv[0]);
		exit(1);
	}
 
	filename = argv[1];   /* first argument     */
	text = argv[2];   /* second argument    */
	num_chars = strlen(text);
 
	error = FT_Init_FreeType(&library);     /* initialize library */
	if (error) {
		printf("FT_Init_FreeType failed\n");
		return -1;
	}
 
	error = FT_New_Face(library, filename, 0, &face); /* create face object */
	if (error) {
		printf("FT_New_Face failed\n");
		FT_Done_FreeType(library);
		return -1;
	}
 
	for (n = 0; n < num_chars; n++)
	{
		gen_tga(&face, &library, text[n]);
	}
	FT_Done_Face(face);
	FT_Done_FreeType(library);
 
	return 0;
}