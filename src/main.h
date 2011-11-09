
#ifndef INCLUDED_MAIN_H
#define INCLUDED_MAIN_H

#include <stdio.h>
#include <vector>

#include "pstdint.h"

// .ter magic strings
#define AI_TERR_BASE_STRING         "TERRAGEN"
#define AI_TERR_TERRAIN_STRING      "TERRAIN "
#define AI_TERR_EOF_STRING          "EOF "

// .ter chunks
#define AI_TERR_CHUNK_XPTS          "XPTS"
#define AI_TERR_CHUNK_YPTS          "YPTS"
#define AI_TERR_CHUNK_SIZE          "SIZE"
#define AI_TERR_CHUNK_SCAL          "SCAL"
#define AI_TERR_CHUNK_CRAD          "CRAD"
#define AI_TERR_CHUNK_CRVM          "CRVM"
#define AI_TERR_CHUNK_ALTW          "ALTW"


// loader.cpp
void read_file(FILE* in, unsigned int& width, unsigned int& height, std::vector<float>& out);

// writer.cpp
void save_bmp (FILE* file, const uint8_t* data, unsigned int width, unsigned int height);
void save_tga (FILE* file, const uint8_t* data, unsigned int width, unsigned int height);

#endif 
