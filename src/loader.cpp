/*
Ter2Heightmap
Copyright (c) 2011, Alexander C. Gessler

Licensed under a 3-clause BSD license. See the LICENSE file for more information.

Strongly based on TERImporter.cpp from assimp (http://assimp.sourceforge.net),
which is too licensed under a 3-clause BSD license.

Assimp is (c) 2008-2011 Assimp Development Team
*/


#include "main.h"
#include "streamreader.h"

#include <math.h>

void read_file(FILE* in, unsigned int& width, unsigned int& height, std::vector<float>& out) 
{
	// Construct a stream reader to read all data in the correct endianess
	StreamReaderLE reader(in);
	if(reader.GetRemainingSize() < 16) {
		throw "file is too small";
	}

	// Check for the existence of the two magic strings 'TERRAGEN' and 'TERRAIN '
	if (::strncmp((const char*)reader.GetPtr(),AI_TERR_BASE_STRING,8)) {
		throw "magic string \'TERRAGEN\' not found";
	}

	if (::strncmp((const char*)reader.GetPtr()+8,AI_TERR_TERRAIN_STRING,8)) {
		throw "magic string \'TERRAIN\' not found";
	}

	float sx,sy,sz;

	// Now read all chunks until we're finished or an EOF marker is encountered
	reader.IncPtr(16);
	while (reader.GetRemainingSize() >= 4)	
	{
		const char* head = (const char*)reader.GetPtr();
		reader.IncPtr(4);

		// EOF, break in every case
		if (!::strncmp(head,AI_TERR_EOF_STRING,4)) {
			break;
		}

		// Number of x-data points
		if (!::strncmp(head,AI_TERR_CHUNK_XPTS,4))
		{
			width = static_cast<unsigned int>(reader.GetI2());
		}
		// Number of y-data points
		else if (!::strncmp(head,AI_TERR_CHUNK_YPTS,4))
		{
			height = static_cast<unsigned int>(reader.GetI2());
		}
		// Squared terrains width-1. 
		else if (!::strncmp(head,AI_TERR_CHUNK_SIZE,4))
		{
			width = height = static_cast<unsigned int>(reader.GetI2()) + 1;
		}
		// terrain scaling
		else if (!::strncmp(head,AI_TERR_CHUNK_SCAL,4))
		{
			sx = reader.GetF4();
			sy = reader.GetF4();
			sz = reader.GetF4();
		}
		// actual terrain data
		else if (!::strncmp(head,AI_TERR_CHUNK_ALTW,4))
		{
			float hscale  = static_cast<float>(reader.GetI2())  / 65536;
			if (fabs(hscale) < 1e-6f) {
				hscale = 1.f;
			}

			// Ensure we have enough data
			if (reader.GetRemainingSize() < width*height*2) {
				throw "ALTW chunk is too small";
			}

			if (width <= 1 || height <= 1) {
				throw "invalid terrain size";
			}

			const int16_t* data = reinterpret_cast<const int16_t*>(reader.GetPtr());

			out.resize(width*height);
			std::vector<float>::iterator it = out.begin();

			for(const int16_t* end = data + width*height; data != end; ++data) {
				*it++ = static_cast<float>(*data) * hscale;
			}
		}

		// Get to the next chunk (4 byte aligned)
		unsigned dtt;
		if ((dtt = reader.GetCurrentPos() & 0x3)) {
			reader.IncPtr(4-dtt);
		}
	}
}