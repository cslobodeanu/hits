#ifndef __MORGANA_BASE_MEMORY_MANAGED_CPU_MEMORY_PAGE_H__
#define __MORGANA_BASE_MEMORY_MANAGED_CPU_MEMORY_PAGE_H__

#include "memorypage.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace Memory
		{
			namespace Managed
			{
				class CPUMemoryPage : public MemoryPage
				{
				protected:

					CPUMemoryPage(const Rect& size, const int type) : MemoryPage(size, type)
					{
						allocatedBuffer = NULL;
					}

					virtual void AllocPage()
					{
						allocatedBuffer = new char[allocatedRect.width * allocatedRect.height];
					}
					virtual void FreePage()
					{
						delete [] allocatedBuffer;
					}
					virtual void* Lock(Block_t* b)
					{
						return allocatedBuffer + b->location.x + b->location.y * b->location.width;
					}
					virtual void Unlock(Block_t* b)
					{

					}

					template<class T, int sizex, int sizey, int type>
					friend class ManagedPages;
				private:
					char*						allocatedBuffer;
				};

				template<int size>
				class ManagedCPUMemory : public Managed::ManagedPages<CPUMemoryPage, size, 1, 0> {};
			}
		}
	}
}

#endif