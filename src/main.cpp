/*
Ter2Heightmap
Copyright (c) 2011, Alexander C. Gessler

Licensed under a 3-clause BSD license. See the LICENSE file for more information.

*/

#define _CRT_SECURE_NO_WARNINGS

#include "version.h"
#include "main.h"

#include <algorithm>
#include <iostream>
#include <string>


int unrecog_exit(int ex = -1)
{
	std::cout << "usage: ter2heightmap input output" << std::endl;
	return ex;
}

void printver() 
{
	std::cout << "ter2heightmap v" << TER2HEIGHTMAP_VERSION_MAJOR << "." << TER2HEIGHTMAP_VERSION_MINOR << "\n";
	std::cout << "(c) 2011, Alexander C. Gessler\nLicensed under a 3-clause BSD license\n" 
		<< std::endl;
}

void printhelp()
{
	unrecog_exit();
	std::cout << " output formats currently supported: tga" << std::endl;
}



int main (int argc, char *argv[])
{
	if (argc == 1) {
		return unrecog_exit(-1);
	}

	int nextarg = 1;
	while(nextarg < argc && argv[nextarg][0] == '-') {
		if (!strcmp(argv[nextarg],"--help")) {
			printhelp();
			return 0;
		}
		else if (!strcmp(argv[nextarg],"--version")) {
			printver();
			return 0;
		}
		++nextarg;
	}

	if (argc < nextarg+2) {
		return unrecog_exit(-2);
	}

	const char* in = argv[nextarg], *out = argv[nextarg+1];
	
	// read .ter input file
	unsigned int width, height;
	std::vector<float> buff;

	FILE* infile = fopen(in,"rb");
	if(!infile) {
		std::cerr << "failed to open file: " << in << std::endl;
		return -1;
	}

	try {
		read_file(infile, width, height, buff);
	}
	catch(const char* err) {
		std::cerr << "failed reading .ter file: " << err << std::endl;
		fclose(infile);
		return -2;
	}

	fclose(infile);

	// rescale to 8 bit
	float hmin = 1e10f, hmax = -1e10f;
	for(std::vector<float>::iterator it = buff.begin(); it != buff.end(); ++it) {
		hmin = std::min(hmin,*it);
		hmax = std::max(hmax,*it);
	}

	hmax -= hmin;

	std::vector<uint8_t> scaled(buff.size());
	std::vector<uint8_t>::iterator iout = scaled.begin();
	for(std::vector<float>::iterator it = buff.begin(); it != buff.end(); ++it) {
		*iout++ = static_cast<uint8_t>( (255.f *  (*it - hmin) / hmax) );
	}

	// write output file
	FILE* outfile = fopen(out,"wb");

	std::string sout = out, ext = sout.substr(sout.find_last_of(".") + 1);
	std::transform(ext.begin(),ext.end(), ext.begin(), ::tolower);

	if (ext == "tga") {
		save_tga(outfile,&*scaled.begin(),width,height);
	}
	else if (ext == "bmp") {
		save_bmp(outfile,&*scaled.begin(),width,height);
	}
	else {
		std::cerr << "output file extension not recognized: " << ext << std::endl; 
		fclose(outfile);
		return -3;
	}

	fclose(outfile);
	return 0;
}
