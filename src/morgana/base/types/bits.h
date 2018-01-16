#ifndef __MORGANA_BASE_TYPES_BITS_H__
#define __MORGANA_BASE_TYPES_BITS_H__

#include <string.h>

namespace MorganaEngine
{
	namespace Base
	{
		namespace Types
		{
			template <int N> class Bits
			{
				unsigned char	m_pBits[N / 8];
			public:
				Bits()
				{
					Clear();
				}

				~Bits()
				{
				}

				inline const bool IsBitSet(const int& bitindex) const
				{
					const byte v = 1 << (bitindex & 7);
					return (m_pBits[bitindex >> 3] & v) != 0;
				}

				inline void SetBit(const int& bitindex)
				{
					const byte v = 1 << (bitindex & 7);
					m_pBits[bitindex >> 3] |= v;
				}

				inline void SetBits(const int& bitindex1, const int& bitindex2)
				{
					SetBit(bitindex1);
					SetBit(bitindex2);
				}

				inline void SetBits(const int& bitindex1, const int& bitindex2, const int& bitindex3)
				{
					SetBit(bitindex1);
					SetBit(bitindex2);
					SetBit(bitindex3);
				}

				inline void SetBits(const int& bitindex1, const int& bitindex2, const int& bitindex3, const int& bitindex4)
				{
					SetBit(bitindex1);
					SetBit(bitindex2);
					SetBit(bitindex3);
					SetBit(bitindex4);
				}

				inline void DeleteBit(const int& bitindex)
				{
					const byte v = 1 << (bitindex & 7);
					m_pBits[bitindex >> 3] &= ~v;
				}

				inline void Clear()
				{
					memset(m_pBits, 0, N / 8);
				}

				inline void SetAll()
				{
					memset(m_pBits, 255, N / 8);
				}
			};
		}
	}
}

#define __BIT_SET(value, bit)		value |= bit
#define __BIT_DEL(value, bit)		value &= ~bit

#endif