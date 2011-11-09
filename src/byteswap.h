/*
Ter2Heightmap
Copyright (c) 2011, Alexander C. Gessler

Licensed under a 3-clause BSD license. See the LICENSE file for more information.

Strongly based on ByteSwap.cpp from assimp (http://assimp.sourceforge.net),
which is too licensed under a 3-clause BSD license.

Assimp is (c) 2008-2011 Assimp Development Team
*/


#ifndef AI_BYTESWAP_H_INC
#define AI_BYTESWAP_H_INC

#include <assert.h>
#include "pstdint.h"

#if _MSC_VER >= 1400 
#include <stdlib.h>
#endif


class ByteSwap
{
	ByteSwap() {}

public:

	static inline void Swap2(void* _szOut)
	{
		assert(_szOut);

#if _MSC_VER >= 1400
		uint16_t* const szOut = reinterpret_cast<uint16_t*>(_szOut);
		*szOut = _byteswap_ushort(*szOut);
#else
		uint8_t* const szOut = reinterpret_cast<uint8_t*>(_szOut);
		std::swap(szOut[0],szOut[1]);
#endif
	}


	static inline void Swap4(void* _szOut)
	{
		assert(_szOut);

#if _MSC_VER >= 1400
		uint32_t* const szOut = reinterpret_cast<uint32_t*>(_szOut);
		*szOut = _byteswap_ulong(*szOut);
#else
		uint8_t* const szOut = reinterpret_cast<uint8_t*>(_szOut);
		std::swap(szOut[0],szOut[3]);
		std::swap(szOut[1],szOut[2]);
#endif
	}

	static inline void Swap8(void* _szOut)
	{
		assert(_szOut);

#if _MSC_VER >= 1400
		uint64_t* const szOut = reinterpret_cast<uint64_t*>(_szOut);
		*szOut = _byteswap_uint64(*szOut);
#else
		uint8_t* const szOut = reinterpret_cast<uint8_t*>(_szOut);
		std::swap(szOut[0],szOut[7]);
		std::swap(szOut[1],szOut[6]);
		std::swap(szOut[2],szOut[5]);
		std::swap(szOut[3],szOut[4]);
#endif
	}

	
	static inline void Swap(float* fOut) {
		Swap4(fOut);
	}

	static inline void Swap(double* fOut) {
		Swap8(fOut);
	}

	static inline void Swap(int16_t* fOut) {
		Swap2(fOut);
	}

	static inline void Swap(uint16_t* fOut) {
		Swap2(fOut);
	}

	static inline void Swap(int32_t* fOut){
		Swap4(fOut);
	}

	static inline void Swap(uint32_t* fOut){
		Swap4(fOut);
	}

	static inline void Swap(int64_t* fOut) {
		Swap8(fOut);
	}

	static inline void Swap(uint64_t* fOut) {
		Swap8(fOut);
	}

	template<typename Type> 
	static inline Type Swapped(Type tOut)
	{
		return _swapper<Type,sizeof(Type)>()(tOut);
	}

private:

	template <typename T, size_t size> struct _swapper;
};

template <typename T> struct ByteSwap::_swapper<T,2> {
	T operator() (T tOut) {
		Swap2(&tOut); 
		return tOut;
	}
};

template <typename T> struct ByteSwap::_swapper<T,4> {
	T operator() (T tOut) {
		Swap4(&tOut); 
		return tOut;
	}
};

template <typename T> struct ByteSwap::_swapper<T,8> {
	T operator() (T tOut) {
		Swap8(&tOut); 
		return tOut;
	}
};


// --------------------------------------------------------------------------------------
// ByteSwap macros for BigEndian/LittleEndian support 
// --------------------------------------------------------------------------------------
#if (defined AI_BUILD_BIG_ENDIAN)
#	define AI_LE(t)	(t)
#	define AI_BE(t) ByteSwap::Swapped(t)
#	define AI_LSWAP2(p)
#	define AI_LSWAP4(p)
#	define AI_LSWAP8(p)
#	define AI_LSWAP2P(p)
#	define AI_LSWAP4P(p)
#	define AI_LSWAP8P(p)
#	define LE_NCONST const
#	define AI_SWAP2(p) ByteSwap::Swap2(&(p))
#	define AI_SWAP4(p) ByteSwap::Swap4(&(p))
#	define AI_SWAP8(p) ByteSwap::Swap8(&(p))
#	define AI_SWAP2P(p) ByteSwap::Swap2((p))
#	define AI_SWAP4P(p) ByteSwap::Swap4((p))
#	define AI_SWAP8P(p) ByteSwap::Swap8((p))
#	define BE_NCONST
#else
#	define AI_BE(t)	(t)
#	define AI_LE(t) ByteSwap::Swapped(t)
#	define AI_SWAP2(p)
#	define AI_SWAP4(p)
#	define AI_SWAP8(p)
#	define AI_SWAP2P(p)
#	define AI_SWAP4P(p)
#	define AI_SWAP8P(p)
#	define BE_NCONST const
#	define AI_LSWAP2(p)		ByteSwap::Swap2(&(p))
#	define AI_LSWAP4(p)		ByteSwap::Swap4(&(p))
#	define AI_LSWAP8(p)		ByteSwap::Swap8(&(p))
#	define AI_LSWAP2P(p)	ByteSwap::Swap2((p))
#	define AI_LSWAP4P(p)	ByteSwap::Swap4((p))
#	define AI_LSWAP8P(p)	ByteSwap::Swap8((p))
#	define LE_NCONST
#endif


namespace Intern {

// --------------------------------------------------------------------------------------------
template <typename T, bool doit>
struct ByteSwapper	{
	void operator() (T* inout) {
		ByteSwap::Swap(inout);
	}
};

template <typename T> 
struct ByteSwapper<T,false>	{
	void operator() (T*) {
	}
};


template <bool SwapEndianess, typename T> 
struct Getter {

	void operator() (T* inout) {
		// static branch
		ByteSwapper<T,(SwapEndianess && sizeof(T)>1)> () (inout);
	}
};
} // end Intern

#endif //!! AI_BYTESWAP_H_INC
