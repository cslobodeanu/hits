#ifndef __MORGANA_BASE_MEMORY_HEAP_MEMORY_BLOCK_H__
#define __MORGANA_BASE_MEMORY_HEAP_MEMORY_BLOCK_H__

#include <string.h>
#include "debug.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace Memory
		{
			class HeapMemoryBlock
			{
				static char*	buffer;
				static int		allocatedSize;
				static int		refCount;
			public:

				int		size;

				HeapMemoryBlock(const int sz, const int fillValue = -1)
				{
					ASSERT(refCount == 0);
					if (sz > allocatedSize)
					{
						if (buffer != NULL)
							delete[] buffer;
						buffer = new char[sz];
						allocatedSize = sz;
					}
					size = sz;
					if (fillValue >= 0)
						memset(buffer, sz, (size_t)fillValue);

					refCount++;
				}

				static void Release()
				{
					if (buffer != NULL)
					{
						delete[] buffer;
						buffer = NULL;
						allocatedSize = 0;
					}
				}

				~HeapMemoryBlock()
				{
					refCount--;
				}

				inline operator char* ()
				{
					return buffer;
				}

				inline char& operator[](int i) const
				{
					return buffer[i];
				}

				inline const unsigned char* ptr() const
				{
					return (const unsigned char*)buffer;
				}

				inline const char* c_str() const
				{
					return (const char*)buffer;
				}


				inline void Fill(const char value)
				{
					memset(buffer, value, size);
				}

				inline void Fill(const void* value, const int stride = 1)
				{
					int ofs = 0;
					while(ofs < size)
					{
						memcpy(buffer + ofs, value, stride);
						ofs += stride;
					}
				}

				inline void FillZero()
				{
					memset(buffer, 0, size);
				}
			};
		}
	}
}

#endif