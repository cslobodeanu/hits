#include "memorypage.h"
#include "meio.h"

using namespace MorganaEngine::Base::Memory::Managed;

const unsigned long MemoryPage::GetHandle() const
{
	return handle;
}

void MemoryPage::SplitFreeBlock(Block_t* b, const Rect& desiredAlloc)
{
	Block_t* newOne = new Block_t(*b);
	newOne->location.width -= desiredAlloc.width;
	newOne->location.height -= desiredAlloc.height;
	if (b->location.height == 1)
		newOne->location.height = 1;
	newOne->location.x += desiredAlloc.width;
	/*newOne.location.y += desiredAlloc.height;*/

	b->page->freeBlocks.Add(newOne);

	b->location.width = desiredAlloc.width;
	b->location.height = desiredAlloc.height;
}

MemoryPage::MemoryPage(const Rect& size, const int _type) : allocatedRect(size), type(_type)
{
	handle = 0;
	Block_t* b = new Block_t(size, this);
	freeBlocks.Add(b);
}

MemoryPage::~MemoryPage()
{
	for (int i = 0; i < allocatedBlocks.Length(); i++)
	{
		SAFE_DEL(allocatedBlocks[i]);
	}
	allocatedBlocks.Clear();

	for (int i = 0; i < freeBlocks.Length(); i++)
	{
		SAFE_DEL(freeBlocks[i]);
	}
	freeBlocks.Clear();
}