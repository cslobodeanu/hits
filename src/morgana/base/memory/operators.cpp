#include "operators.h"

#ifdef M_USE_MEMORY_TRACKING
#include <stdlib.h>
#include "memtracker.h"
using namespace MorganaEngine::Base::Memory;

void* __m_alloc(size_t num)
{
	MemTracker::IncreaseAllocatedSize(num, MemTracker::MEMORY_TYPE_CPU);
	unsigned char* ptr = (unsigned char*)malloc(num + 4);
	*((unsigned int*)ptr) = num;
	return ptr+4;
}

void __m_free(void *ptr)
{
	unsigned char* actual_ptr = ((unsigned char*)ptr) - 4;
	MemTracker::DecreaseAllocatedSize(*((unsigned int*)actual_ptr), MemTracker::MEMORY_TYPE_CPU);
	free(actual_ptr);
}

void* operator new(size_t num)
{
	return __m_alloc(num);
}

void* operator new[](size_t num)
{
	return __m_alloc(num);
}

void operator delete(void *ptr)
{
	__m_free(ptr);
}

void operator delete[](void *ptr)
{
	__m_free(ptr);
}

#endif