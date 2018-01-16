#ifndef __MORGANA_FMK_RENDERERS_STREAMING_BUFFERS_H__
#define __MORGANA_FMK_RENDERERS_STREAMING_BUFFERS_H__

#include "renderer.h"

namespace MorganaEngine
{
	namespace Framework
	{
		namespace Renderers
		{
			template <class T, int maxElems, int bindingType>
			class StreamingBuffers
			{
				struct DataBuffer
				{
				public:
					DataBuffer()
					{
						buffer = new DisplayObjects::Buffer(bindingType);
						buffer->Create(maxElems * sizeof(T), BUFFER_ACCESS_DYNAMIC);
						elemsOffset = 0;
					}
					~DataBuffer()
					{
						delete buffer;
					}

					T* Alloc(int numElems)
					{
						ASSERT(numElems <= maxElems);
						if (numElems + elemsOffset > maxElems)
							return NULL;

						T* ret = (T*)buffer->Lock() + elemsOffset;
						elemsOffset += numElems;
						return ret;
					}

					T* TryAlloc(int numElems)
					{
						ASSERT(numElems <= maxElems);
						if (numElems + elemsOffset > maxElems)
							return NULL;

						T* ret = (T*)buffer->Lock() + elemsOffset;
						return ret;
					}

					void Free()
					{
						elemsOffset = 0;
					}

					void Unlock()
					{
						buffer->Unlock();
					}

					Buffer* buffer;
					unsigned int elemsOffset;
				};
				
				Array<DataBuffer*> buffers;
				int buffersIndex;
			public:
				StreamingBuffers()
				{
					buffersIndex = 0;
					buffers.Add(new DataBuffer());
				}

				~StreamingBuffers()
				{
					for (int i = 0; i < buffers.Length(); i++)
						delete buffers[i];
				}

				T* Alloc(unsigned int numElems)
				{
					if (buffersIndex >= buffers.Length())
						buffers.Add(new DataBuffer());

					T* ptr = buffers[buffersIndex]->Alloc(numElems);
					if (ptr == NULL)
					{
						buffers[buffersIndex]->Unlock();
						buffersIndex++;
						ptr = Alloc(numElems);
					}
					return ptr;
				}

				T* TryAlloc(unsigned int numElems)
				{
					if (buffersIndex >= buffers.Length())
						buffers.Add(new DataBuffer());

					T* ptr = buffers[buffersIndex]->TryAlloc(numElems);
					if (ptr == NULL)
					{
						buffers[buffersIndex]->Unlock();
						buffersIndex++;
						ptr = Alloc(numElems);
					}
					return ptr;
				}

				const unsigned int GetBufferOffset() const
				{
					return buffers[buffersIndex]->elemsOffset * sizeof(T);
				}

				Buffer*	GetBuffer() const
				{
					return buffers[buffersIndex]->buffer;
				}

				const unsigned int GetMaxElems()
				{
					return maxElems;
				}
				
				void Begin()
				{
					for (int i = 0; i <= buffersIndex; i++)
					{
						buffers[i]->Free();
					}
					buffersIndex = 0;
				}

				void End()
				{
					for (int i = 0; i <= buffersIndex; i++)
					{
						buffers[i]->Unlock();
					}
				}
			};

		}
	}
}
#endif