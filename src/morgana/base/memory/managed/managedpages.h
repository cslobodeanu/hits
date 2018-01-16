#ifndef __MORGANA_BASE_MEMORY_MANAGED_MANAGED_PAGES_H__
#define __MORGANA_BASE_MEMORY_MANAGED_MANAGED_PAGES_H__

#include "metypes.h"
#include "memorypage.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace Memory
		{
			namespace Managed
			{
				template<class T, int sizex, int sizey, int type>
				class ManagedPages
				{
				public:

					ManagedPages();
					virtual ~ManagedPages();

					MemoryPage::Block_t*		Alloc(const Rect& size);
					MemoryPage::Block_t*		Alloc(const unsigned int& size);
					void						Free(MemoryPage::Block_t* b);
					void						FreeAllPages();

				protected:
					Array<T*>					m_pAllPages;
					MemoryPage::Block_t*		GetBestFitFreeBlock(const Rect& desired);
				};

				template<class T, int sizex, int sizey, int type>
				void ManagedPages<T, sizex, sizey, type>::Free(MemoryPage::Block_t* b)
				{
					if(b == NULL) return;
					b->page->allocatedBlocks.Remove(b);
					b->page->freeBlocks.Add(b);
				}

				template<class T, int sizex, int sizey, int type>
				void ManagedPages<T, sizex, sizey, type>::FreeAllPages()
				{
					for (int i = 0; i < m_pAllPages.Length(); i++)
						m_pAllPages[i]->FreePage();
				}

				template<class T, int sizex, int sizey, int type>
				MemoryPage::Block_t* ManagedPages<T, sizex, sizey, type>::Alloc(const unsigned int& size)
				{
					Rect r(0, 0, size, 1);
					return Alloc(r);
				}

				template<class T, int sizex, int sizey, int type>
				MemoryPage::Block_t* ManagedPages<T, sizex, sizey, type>::Alloc(const Rect& size)
				{
					MemoryPage::Block_t* fb = GetBestFitFreeBlock(size);
					if(fb->CanFit(size) > 0)
					{
						fb->page->SplitFreeBlock(fb, size);
					}
					else
					{
					}

					ASSERT(fb->GetSize() != 0, "Cannot fit in mem (%dbytes)", size.Area());
					//DEBUG_OUT("Allocated %dbytes in page%d of type %d", size.Area(), fb->page->index, fb->page->type);

					fb->page->allocatedBlocks.Add(fb);
					fb->page->freeBlocks.Remove(fb);

					return fb->page->allocatedBlocks.Last();
				}

				template<class T, int sizex, int sizey, int type>
				MemoryPage::Block_t* ManagedPages<T, sizex, sizey, type>::GetBestFitFreeBlock(const Rect& desired)
				{
					MemoryPage::Block_t* bestFit = NULL;
					int minDiffInSize = Mathf::MaxInt;

					for(int i = 0; i < m_pAllPages.Length(); i++)
					{
						MemoryPage* mp = m_pAllPages[i];
						for(int j = 0; j < mp->freeBlocks.Length(); j++)
						{
							MemoryPage::Block_t* fb = mp->freeBlocks[j];
							int diff = fb->CanFit(desired);
							if(diff >= 0)
							{
								if(diff < minDiffInSize)
								{
									minDiffInSize = diff;
									bestFit = fb;
								}
							}
						}
					}

					if(bestFit == NULL)
					{
						Rect sz(0, 0, sizex, sizey);
						T* page = new T(sz, type);
						page->AllocPage();
						page->index = m_pAllPages.Length();
						m_pAllPages.Add(page);
						bestFit = GetBestFitFreeBlock(desired);
					}

					return bestFit;
				}

				template<class T, int sizex, int sizey, int type>
				ManagedPages<T, sizex, sizey, type>::~ManagedPages()
				{
					for(int i = 0; i < m_pAllPages.Length(); i++)
					{
						T* page = m_pAllPages[i];
						page->FreePage();
						SAFE_DEL(page);
					}
				}

				template<class T, int sizex, int sizey, int type>
				ManagedPages<T, sizex, sizey, type>::ManagedPages()
				{
					m_pAllPages.SetLength(128);
					m_pAllPages.SetLength(0);
				}

			}
		}
	}
}

#endif