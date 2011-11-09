/*
Ter2Heightmap
Copyright (c) 2011, Alexander C. Gessler

Licensed under a 3-clause BSD license. See the LICENSE file for more information.

Strongly based on ImageExtractor.cpp from assimp (http://assimp.sourceforge.net),
which is too licensed under a 3-clause BSD license.

Assimp is (c) 2008-2011 Assimp Development Team
*/


#include "main.h"
#include <assert.h>

#include "pushpack1.h"

namespace {

// Data structure for the first header of a BMP
struct BITMAPFILEHEADER 
{
	uint16_t  bfType ;
	uint32_t  bfSize;
	uint16_t  bfReserved1 ;
	uint16_t  bfReserved2; 
	uint32_t  bfOffBits; 
} PACK_STRUCT;


// Data structure for the second header of a BMP
struct BITMAPINFOHEADER
{
	int32_t        biSize;
	int32_t        biWidth;
	int32_t        biHeight;
	int16_t        biPlanes;
	int16_t        biBitCount;
	uint32_t       biCompression;
	int32_t        biSizeImage;
	int32_t        biXPelsPerMeter;
	int32_t        biYPelsPerMeter;
	int32_t        biClrUsed;
	int32_t        biClrImportant;

	// pixel data follows header
} PACK_STRUCT;


// Data structure for the header of a TGA file
struct TGA_HEADER
{
	uint8_t  identsize;          // size of ID field that follows 18 byte header (0 usually)
	uint8_t  colourmaptype;      // type of colour map 0=none, 1=has palette
	uint8_t  imagetype;          // type of image 0=none,1=indexed,2=rgb,3=grey,+8=rle packed

	uint16_t colourmapstart;     // first colour map entry in palette
	uint16_t colourmaplength;    // number of colours in palette
	uint8_t  colourmapbits;      // number of bits per palette entry 15,16,24,32

	uint16_t xstart;             // image x origin
	uint16_t ystart;             // image y origin
	uint16_t width;              // image width in pixels
	uint16_t height;             // image height in pixels
	uint8_t  bits;               // image bits per pixel 8,16,24,32
	uint8_t  descriptor;         // image descriptor bits (vh flip bits)

	// pixel data follows header
} PACK_STRUCT;


#include "poppack1.h"
} // !}

void save_bmp (FILE* file, const uint8_t* data, unsigned int width, unsigned int height)
{
	assert(!!file);
	assert(!!data);

	BITMAPFILEHEADER header;
	header.bfType      = 'B' | (int('M') << 8u);
	header.bfOffBits   = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+256*4;
	header.bfSize      = header.bfOffBits+width*height;
	header.bfReserved1 = header.bfReserved2 = 0;

	fwrite(&header,sizeof(BITMAPFILEHEADER),1,file);

	BITMAPINFOHEADER info;
	info.biSize     = 40;
	info.biWidth    = width;
	info.biHeight   = height;
	info.biPlanes   = 1;
	info.biBitCount = 8;
	info.biCompression = 0;
	info.biSizeImage   = width*height;
	info.biXPelsPerMeter = 1; // dummy
	info.biYPelsPerMeter = 1; // dummy
	info.biClrUsed = 0;
	info.biClrImportant = 0;

	fwrite(&info,sizeof(BITMAPINFOHEADER),1,file);

	// write a dummy palette
	for (unsigned int i = 0; i < 256; ++i) {
		const char t = static_cast<char>(i);
		fwrite(&t,1,1,file);
		fwrite(&t,1,1,file);
		fwrite(&t,1,1,file);
		fwrite(&t,1,1,file);
	}

	const uint8_t* temp = data+info.biSizeImage;

	for (int y = 0; temp -= width,y < info.biHeight;++y)	{
		fwrite(temp,width,1,file);
	}
}


void save_tga (FILE* file, const uint8_t* data, unsigned int width, unsigned int height)
{
	assert(!!file);
	assert(!!data);

	TGA_HEADER head = {0};
	head.bits   = 8;
	head.height = (uint16_t)height;
	head.width  = (uint16_t)width;
	head.descriptor |= (1u<<5);

	head.imagetype = 3; 
	fwrite(&head,sizeof(TGA_HEADER),1,file);
	fwrite(data, width*height, 1, file);
}
