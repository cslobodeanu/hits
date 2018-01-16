#include "memtracker.h"
#include <memory.h>

using namespace MorganaEngine::Base::Memory;

unsigned long	MemTracker::allocatedMemPeak[MemTracker::MEMORY_TYPES_COUNT];
unsigned long	MemTracker::allocatedMem[MemTracker::MEMORY_TYPES_COUNT];

MemTracker::MemTracker()
{
	Reset();
}

MemTracker::~MemTracker()
{

}

void MemTracker::Reset()
{
	memset(allocatedMem, 0, MEMORY_TYPES_COUNT * sizeof(unsigned long));
	memset(allocatedMemPeak, 0, MEMORY_TYPES_COUNT * sizeof(unsigned long));
}

void MemTracker::IncreaseAllocatedSize(const int sz, const int type /*= MEMORY_TYPE_UNKNOWN*/)
{
	allocatedMem[type] += sz;

	if(allocatedMem[type] > allocatedMemPeak[type])
		allocatedMemPeak[type] = allocatedMem[type];
}

void MemTracker::DecreaseAllocatedSize(const int sz, const int type /*= MEMORY_TYPE_UNKNOWN*/)
{
	allocatedMem[type] -= sz;
}

unsigned long MemTracker::GetAllocatedMem(const int type /*= MEMORY_TYPE_UNKNOWN*/)
{
	return allocatedMem[type];
}

unsigned long MemTracker::GetAllocatedMemPeak(const int type /*= MEMORY_TYPE_UNKNOWN*/)
{
	return allocatedMemPeak[type];
}

unsigned long MemTracker::GetAllAllocatedMem()
{
	unsigned long sz = 0;
	for(int i = 0; i < MEMORY_TYPES_COUNT; i++)
		sz += allocatedMem[i];

	return sz;
}

unsigned long MemTracker::GetAllAllocatedMemPeak()
{
	unsigned long sz = 0;
	for(int i = 0; i < MEMORY_TYPES_COUNT; i++)
		sz += allocatedMemPeak[i];

	return sz;
}

unsigned long MemTracker::GetCPUMem()
{
	return allocatedMem[MEMORY_TYPE_CPU];
}

unsigned long MemTracker::GetGPUMem()
{
	return allocatedMem[MEMORY_TYPE_GPU_VBO] + allocatedMem[MEMORY_TYPE_GPU_IBO] + allocatedMem[MEMORY_TYPE_GPU_TEX] + allocatedMem[MEMORY_TYPE_GPU_PBO];
}