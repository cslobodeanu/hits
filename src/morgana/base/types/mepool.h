#ifndef __MORGANA_BASE_TYPES_MEPOOL_H__
#define __MORGANA_BASE_TYPES_MEPOOL_H__

#include "mearray.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace Types
		{

			template <class T> class MEPool
			{
				Array<T>		objects;
				Array<bool>		validity;
				Array<int>		dirty;

				inline void		IncreaseObjectCount()
				{
					objects.Realloc(objects.Length() + 1);
					validity.Realloc(validity.Length() + 1);
				}
			public:

				inline MEPool<T>()
				{

				}

				inline ~MEPool<T>()
				{
					Clear();
				}

				inline void Clear()
				{
					objects.Alloc(0);
					validity.Alloc(0);
					dirty.Alloc(0);
				}

				inline int GenerateObject()
				{
					int index = -1;
					if(dirty.Length())
					{
						index = dirty.Pop();
					}
					else
					{
						IncreaseObjectCount();
						index = objects.Length() - 1;
					}

					validity[index] = true;
					return index;
				}

				inline const bool IsValidObject(const int index) const
				{
					if(index < 0 || index >= objects.Length()) return false;
					return validity[index];
				}

				inline void DeleteObject(const int index)
				{
					if(IsValidObject(index))
					{
						dirty.Add(index);
						validity[index] = false;
					}
				}

				inline T& GetObject(const int index)
				{
					return objects[index];
				}

				inline T& operator[](const int i) const
				{
					return objects[i];
				}

				inline const int GetValidObjectsCount() const
				{
					return objects.Length() - dirty.Length();
				}

				inline const int GetObjectsCount() const
				{
					return objects.Length();
				}
			};
		}
	}
}

#endif