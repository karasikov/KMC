/*
  This file is a part of KMC software distributed under GNU GPL 3 licence.
  The homepage of the KMC project is http://sun.aei.polsl.pl/kmc
  
  Authors: Sebastian Deorowicz, Agnieszka Debudaj-Grabysz, Marek Kokot
  
  Version: 3.1.1
  Date   : 2019-05-19
*/

#ifndef _INTR_COPY_H
#define _INTR_COPY_H

#if __SSE2__
#include <emmintrin.h>
#endif

#ifndef WIN32
typedef long long __int64;
#endif

// 64b copy function
// size - in 8B words (determined during execution)
// dest and src must be aligned to 8B
inline void IntrCopy64fun(void *_dest, void *_src, uint32_t size)
{
	__int64* dest = (__int64 *)_dest;
	__int64* src = (__int64 *)_src;

	for (unsigned i = 0; i < size; ++i) {
#if __SSE2__
		_mm_stream_si64(dest + i, src[i]);
#else
		dest[i] = src[i];
#endif
	}
}


// 64bit copy function
// SIZE - in 8B words
template <unsigned SIZE> struct IntrCopy64
{
	static inline void Copy(void *_dest, void *_src)
	{
		__int64* dest = (__int64*)_dest;
		__int64* src = (__int64*)_src;

		for (unsigned i = 0; i < SIZE; ++i) {
#if __SSE2__
			_mm_stream_si64(dest + i, src[i]);
#else
			dest[i] = src[i];
#endif
		}
	}
};


template <unsigned SIZE, unsigned MODE> struct IntrCopy128
{
	static inline void Copy(void *_dest, void *_src)
	{
		cerr << "Error\n";
	}
};

// 128bit copy function
// SIZE - in 16B words
// dest - aligned to 16B
// src  - aligned to 16B
template <unsigned SIZE> struct IntrCopy128<SIZE, 1>
{
	static inline void Copy(void *_dest, void *_src)
	{
#if __SSE2__
		__m128i *dest = (__m128i *) _dest;
		__m128i *src = (__m128i *) _src;

		for (unsigned i = 0; i < SIZE; ++i)
			_mm_stream_si128(dest + i, _mm_load_si128(src + i));
#else
		memcpy(_dest, _src, SIZE * 16);
#endif
	}
};


// 128bit copy function
// SIZE - in 16B words
// dest - aligned to 8B
// src  - aligned to 16B
template <unsigned SIZE> struct IntrCopy128<SIZE, 0>
{
	static inline void Copy(void *dest, void *src)
	{
		if ((uint64_t)dest % 16)	// if only 8B aligned use 64b copy
			IntrCopy64<SIZE * 2>::Copy(dest, src);
		else // if 16B aligned use 128b copy
			IntrCopy128<SIZE, 1>::Copy(dest, src);
	}
};


#endif

// ***** EOF
