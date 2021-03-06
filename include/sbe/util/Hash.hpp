#ifndef SCHIFFBRUCHENGINE_HASH_HPP_H
#define SCHIFFBRUCHENGINE_HASH_HPP_H

#include <cstring>

namespace sbe {

	typedef unsigned long HashType;
	constexpr HashType InvalidHash = 0;

	constexpr int tolower(int c)
	{
		return (c>'A' && c<'Z') ? c + (0x61/* 'a' */ - 0x41/* 'A' */) : c;
	}

	constexpr size_t cstrlen(const char* str) {
		size_t result = 0;
		if (str) {
			while (*str++)
				++result;
		}
		return result;
	}


	/**
		// Relatively simple hash of arbitrary text string into a
		// 32-bit identifier Output value is
		// input-valid-deterministic, but no guarantees are made
		// about the uniqueness of the output per-input
		//
		// Input value is treated as lower-case to cut down on false
		// separations cause by human mistypes. Sure, it could be
		// construed as a programming error to mix up your cases, and
		// it cuts down on permutations, but in Real World Usage
		// making this text case-sensitive will likely just lead to
		// Pain and Suffering.
		//
		// This code lossely based upon the adler32 checksum by Mark
		// Adler and published as part of the zlib compression
		// library sources.
	 */
	constexpr HashType hash_name(char const* pIdentStr) {


		// largest prime smaller than 65536
		const unsigned long BASE = 65521L;

		// NMAX is the largest n such that 255n(n+1)/2 +
		// (n+1)(BASE-1) <= 2^32-1
		const unsigned long NMAX = 5552;

#define DO1(buf, i)  {s1 += tolower(buf[i]); s2 += s1;}
#define DO2(buf, i)  DO1(buf,i); DO1(buf,i+1);
#define DO4(buf, i)  DO2(buf,i); DO2(buf,i+2);
#define DO8(buf, i)  DO4(buf,i); DO4(buf,i+4);
#define DO16(buf)   DO8(buf,0); DO8(buf,8);

		if (pIdentStr == NULL)
			return InvalidHash;

		unsigned long s1 = 0;
		unsigned long s2 = 0;

		size_t len = cstrlen(pIdentStr);
		
		while (len > 0)
		{
			unsigned long k = len < NMAX ? len : NMAX;

			len -= k;

			while (k >= 16)
			{
				DO16(pIdentStr);
				pIdentStr += 16;
				k -= 16;
			}

			if (k != 0)
				do
				{
					s1 += tolower(*pIdentStr++);
					s2 += s1;
				} while (--k);

			s1 %= BASE;
			s2 %= BASE;
		}

#undef DO1
#undef DO2
#undef DO4
#undef DO8
#undef DO16
		return static_cast<HashType>((s2 << 16) | s1 );
	}

	namespace operators {
		constexpr sbe::HashType operator "" _Hash(const char* str, size_t) {
			return sbe::hash_name(str);
		}
	}
}


#endif //SCHIFFBRUCHENGINE_HASH_HPP_H
