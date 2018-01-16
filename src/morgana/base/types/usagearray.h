#ifndef __MORGANA_BASE_TYPES_USAGE_ARRAY_H__
#define __MORGANA_BASE_TYPES_USAGE_ARRAY_H__

#include "mearray.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace Types
		{
			template <class T>
			struct UsageArrayItem
			{
				T				ref;
				unsigned long	usage;

				UsageArrayItem()
				{
					usage = 0;
				}

				static const void IncrementUsage(UsageArrayItem<T>*& elem, const UsageArrayItem<T>* first)
				{
					elem->usage++;
					if (elem > first)
					{
						UsageArrayItem<T>* prev = elem - 1;
						if (elem->usage > prev->usage)
						{
							UsageArrayItem<T> aux;
							aux = *elem;
							*elem = *prev;
							*prev = aux;
							elem = prev;
						}
					}
				}
			};
			template <class T>
			class UsageArray : public Array<UsageArrayItem<T>>
			{
			public:
				UsageArray() : Array()
				{

				}

				inline void Push(const T& el)
				{
					UsageArrayItem<T> item;
					item.ref = el;
					Array::Add(item);
				}
			};
		}
	}
}


#endif