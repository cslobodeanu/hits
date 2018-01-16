#ifndef __MORGANA_BASE_MEMORY_OPERATORS_H__
#define __MORGANA_BASE_MEMORY_OPERATORS_H__

#ifdef _DEBUG
#define M_USE_MEMORY_TRACKING
#endif

#ifdef M_USE_MEMORY_TRACKING
void* operator new(size_t num);
void* operator new[](size_t num);
void operator delete(void *ptr);
void operator delete[](void *ptr);
#endif

#endif