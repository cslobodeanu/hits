#ifndef __MORGANA_BASE_MEMORY_MANAGED_MEMORY_PAGE_H__
#define __MORGANA_BASE_MEMORY_MANAGED_MEMORY_PAGE_H__

#include "metypes.h"
#include "memath.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace Memory
		{
			namespace Managed
			{
				class MemoryPage
				{
				public:
					class Block_t
					{
					private:
						void*				lockedPtr;
					public:
						~Block_t(){}

						Rect				location;
						MemoryPage*			page;

						Block_t()
						{
							location.x = 0;
							location.y = 0;
							location.width = 0;
							location.height = 0;
							page = NULL;
							lockedPtr = NULL;
						}

						Block_t(const Rect& r, MemoryPage* mp)
						{
							location = r;
							page = mp;
							lockedPtr = NULL;
						}

						inline int CanFit(const Rect& b)
						{
							const bool fit = b.width < location.width && b.height <= location.height;

							if(fit)
							{
								const int diffW = location.width - b.width;
								const int diffH = location.height - b.height;
								if(diffW == 0 && diffH == 0)
									return 0;
								else
								{
									return Math::Mathf::Max(diffW, 1) * Math::Mathf::Max(diffH, 1);
								}
							}
							return -1;
						}

						inline Block_t& operator = (const Block_t& src)
						{
							location.x = src.location.x;
							location.y = src.location.y;
							location.width = src.location.width;
							location.height = src.location.height;
							page = src.page;
							lockedPtr = NULL;
							return *this;
						}

						inline bool operator == (Block_t const & b) const
						{
							return page == b.page && memcmp(&location, &b.location, sizeof(Rect)) == 0;
						}

						inline void* Lock()
						{
							if(lockedPtr == NULL)
							{
								lockedPtr = page->Lock(this);
							}
							return lockedPtr;
						}
						inline void Unlock()
						{
							if(lockedPtr)
								page->Unlock(this);
							lockedPtr = NULL;
						}

						inline const unsigned long GetHandle() const
						{
							return page->GetHandle();
						}

						inline const unsigned int GetSize() const
						{
							return location.width * location.height;
						}

						inline const unsigned int GetOffset() const
						{
							return location.x + location.y * location.width;
						}
					};

				protected:

					unsigned long				handle;
					Array<Block_t*>				freeBlocks;
					Array<Block_t*>				allocatedBlocks;

					const Rect					allocatedRect;
					const int					type;

					friend class Block_t;
					virtual void*				Lock(Block_t* b) = 0;
					virtual void				Unlock(Block_t* b) = 0;

					virtual void				SplitFreeBlock(Block_t* b, const Rect& desiredAlloc);

					virtual void				AllocPage() = 0;
					virtual void				FreePage() = 0;

					virtual const unsigned long	GetHandle() const;

					MemoryPage(const Rect& size, const int type);
					virtual ~MemoryPage();

					template<class T, int sizex, int sizey, int type>
					friend class ManagedPages;

				public:
					int							index;
				};
			}
		}
	}
}

#endif