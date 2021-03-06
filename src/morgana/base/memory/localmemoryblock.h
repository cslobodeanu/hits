#ifndef __MORGANA_BASE_MEMORY_LOCAL_MEMORY_BLOCK_H__
#define __MORGANA_BASE_MEMORY_LOCAL_MEMORY_BLOCK_H__

#include <string.h>

namespace MorganaEngine
{
	namespace Base
	{
		namespace Memory
		{
			class LocalMemoryBlock
			{
				char*	buffer;
			public:

				int		size;

				LocalMemoryBlock()
				{
					buffer = NULL;
					size = 0;
				}

				LocalMemoryBlock(const int sz, const int fillValue = -1)
				{
					buffer = NULL;
					Realloc(sz);
					if (fillValue >= 0)
						memset(buffer, sz, (size_t)fillValue);
				}

				~LocalMemoryBlock()
				{
					delete [] buffer;
				}

				char* Realloc(const int sz)
				{
					if (buffer != null)
						delete[] buffer;

					buffer = new char[sz];
					size = sz;

					return buffer;
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