/*
Ter2Heightmap
Copyright (c) 2011, Alexander C. Gessler

Licensed under a 3-clause BSD license. See the LICENSE file for more information.

Strongly based on StreamReader.h from assimp (http://assimp.sourceforge.net),
which is too licensed under a 3-clause BSD license.

Assimp is (c) 2008-2011 Assimp Development Team

*/

#ifndef AI_STREAMREADER_H_INCLUDED
#define AI_STREAMREADER_H_INCLUDED

#include "byteswap.h"
#include <assert.h>

#include "pstdint.h"

// --------------------------------------------------------------------------------------------
/** Wrapper class around FILE* to allow for consistent reading of binary data in both 
 *  little and big endian format. Don't attempt to instance the template directly. Use 
 *  StreamReaderLE to read from a little-endian stream and StreamReaderBE to read from a 
 *  BE stream. */
// --------------------------------------------------------------------------------------------
template <bool SwapEndianess = false>
class StreamReader
{

public:
	
	typedef int diff;
	typedef unsigned int pos;

public:


	// ---------------------------------------------------------------------
	StreamReader(FILE* infile)
		: infile(infile)
	{
		assert(infile); 
		InternBegin();
	}


	// ---------------------------------------------------------------------
	~StreamReader() {
		delete[] buffer;
	}

public:

	
	float GetF4()	{
		return Get<float>();
	}

	double GetF8()	{
		return Get<double>();
	}

	int16_t GetI2()	{
		return Get<int16_t>();
	}

	int8_t GetI1()	{
		return Get<int8_t>();
	}

	int32_t GetI4()	{
		return Get<int32_t>();
	}

	int64_t GetI8()	{
		return Get<int64_t>();
	}

	uint16_t GetU2()	{
		return Get<uint16_t>();
	}

	uint8_t GetU1()	{
		return Get<uint8_t>();
	}

	uint32_t GetU4()	{
		return Get<uint32_t>();
	}

	uint64_t GetU8()	{
		return Get<uint64_t>();
	}

public:

	unsigned int GetRemainingSize() const {
		return (unsigned int)(end - current);
	}

	unsigned int GetRemainingSizeToLimit() const {
		return (unsigned int)(end - current);
	}

	void IncPtr(int plus)	{
		current += plus;
		if (current > end) {
			throw "EOF or read end was reached";
		}
	}

	
	int8_t* GetPtr() const	{
		return current;
	}

	void SetPtr(int8_t* p)	{

		current = p;
		if (current > end || current < buffer) {
			throw "EOF or read end was reached";
		}
	}

	int GetCurrentPos() const	{
		return (unsigned int)(current - buffer);
	}

	void SetCurrentPos(size_t pos) {
		SetPtr(buffer + pos);
	}

	template <typename T>
	StreamReader& operator >> (T& f) {
		f = Get<T>(); 
		return *this;
	}

private:

	template <typename T> T Get()	{
		if (current + sizeof(T) > end) {
			throw "EOF or read end was reached";
		}

		T f = *((const T*)current);
		Intern :: Getter<SwapEndianess,T>() (&f);

		current += sizeof(T);
		return f;
	}


	void InternBegin() {
		
		int ft = ftell(infile);
		fseek(infile,0,SEEK_END);
		int endf = ftell(infile);
		fseek(infile,ft,SEEK_SET);

		const int s = endf-ft;
		if (!s) {
			throw "file is empty or EOF was already reached";
		}

		current = buffer = new int8_t[s];
		const int read = fread(current,1,s,infile);
		// (read < s) can only happen if the stream was opened in text mode
		assert(read <= s);
		end = &buffer[read];
	}

private:


	FILE* infile;
	int8_t *buffer, *current, *end;
};


// --------------------------------------------------------------------------------------------
// `static` StreamReaders. Their byte order is fixed and they might be a little bit faster.
#ifdef AI_BUILD_BIG_ENDIAN
	typedef StreamReader<true>  StreamReaderLE;
	typedef StreamReader<false> StreamReaderBE;
#else
	typedef StreamReader<true>  StreamReaderBE;
	typedef StreamReader<false> StreamReaderLE;
#endif


#endif // !! AI_STREAMREADER_H_INCLUDED
