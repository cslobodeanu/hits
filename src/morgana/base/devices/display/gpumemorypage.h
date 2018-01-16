#ifndef __MORGANA_BASE_DEVICES_DISPLAY_GPU_MEMORY_PAGE_H__
#define __MORGANA_BASE_DEVICES_DISPLAY_GPU_MEMORY_PAGE_H__

#include "mememory.h"
#include "objects/buffer.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace Devices
		{
			namespace Display
			{
				class GPUMemoryPage : public Managed::MemoryPage
				{
					DisplayObjects::Buffer* buffer;
				protected:

					GPUMemoryPage(const Rect& size, const int type) : Managed::MemoryPage(size, type)
					{
					}

					virtual void AllocPage()
					{
						buffer = new DisplayObjects::Buffer(type);
						buffer->Create(allocatedRect.width * allocatedRect.height, BUFFER_ACCESS_DYNAMIC);
					}

					virtual void FreePage()
					{
						delete buffer;
					}

					virtual void* Lock(Block_t* b)
					{
						return ((char*)buffer->Lock()) + b->GetOffset();
					}
					virtual void Unlock(Block_t* b)
					{
						buffer->Unlock();
					}

					virtual const unsigned long GetHandle() const
					{
						return (unsigned long)buffer;
					}

					template<class T, int sizex, int sizey, int type>
					friend class Managed::ManagedPages;
				};

				template<int size>
				class ManagedVBO : public Managed::ManagedPages<GPUMemoryPage, size, 1, BINDING_ARRAY_BUFFER> {};
				template<int size>
				class ManagedIBO : public Managed::ManagedPages<GPUMemoryPage, size, 1, BINDING_ELEMENT_ARRAY_BUFFER> {};

				typedef ManagedVBO<8192>			ManagedVBO8192;
				typedef ManagedIBO<8192>			ManagedIBO8192;
			}
		}
	}
}

#endif