#include "heapmemoryblock.h"

using namespace MorganaEngine::Base::Memory;

char*	HeapMemoryBlock::buffer = NULL;
int		HeapMemoryBlock::allocatedSize = 0;
int		HeapMemoryBlock::refCount = 0;