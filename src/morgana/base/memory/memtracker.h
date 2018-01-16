#ifndef __MORGANA_BASE_MEMORY_MEMTRACKER_H__
#define __MORGANA_BASE_MEMORY_MEMTRACKER_H__

namespace MorganaEngine
{
	namespace Base
	{
		namespace Memory
		{
			class MemTracker
			{
			public:
				MemTracker();
				~MemTracker();

				enum
				{
					MEMORY_TYPE_UNKNOWN,
					MEMORY_TYPE_CPU,
					MEMORY_TYPE_GPU_VBO,
					MEMORY_TYPE_GPU_IBO,
					MEMORY_TYPE_GPU_TEX,
					MEMORY_TYPE_GPU_PBO,
					MEMORY_TYPES_COUNT
				};

				static void				IncreaseAllocatedSize(const int sz, const int type = MEMORY_TYPE_UNKNOWN);
				static void				DecreaseAllocatedSize(const int sz, const int type = MEMORY_TYPE_UNKNOWN);

				static unsigned long	GetAllocatedMem(const int type = MEMORY_TYPE_UNKNOWN);
				static unsigned long	GetAllocatedMemPeak(const int type = MEMORY_TYPE_UNKNOWN);

				static unsigned long	GetAllAllocatedMem();
				static unsigned long	GetAllAllocatedMemPeak();

				static unsigned long	GetCPUMem();
				static unsigned long	GetGPUMem();

			private:
				static unsigned long	allocatedMemPeak[MEMORY_TYPES_COUNT];
				static unsigned long	allocatedMem[MEMORY_TYPES_COUNT];
				static void				Reset();
			};
		}
	}
}

#define __mt_kilo(x)		(float)(x / 1024.0f)
#define __mt_mega(x)		(float)(x / (1024.0f * 1024.0f))
#define __mt_giga(x)		(float)(x / (1024.0f * 1024.0f * 1024.0f))

#define MegaBytes			 / (1024.0f * 1024.0f)

#endif