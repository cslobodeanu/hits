#ifndef __MORGANA_BASE_TYPES_MEDICTIONARY_H__
#define __MORGANA_BASE_TYPES_MEDICTIONARY_H__

#include <stdlib.h>

namespace MorganaEngine
{
	namespace Base
	{
		namespace Types
		{
			template<class TKey, class TVal>
			class DictionaryFastChanges
			{
			public:
				class Pair
				{
				public:
					TKey key;
					TVal val;
				};

				Array<Pair>		list;
				int(*KeyCompareFunc)(const TKey, const TKey);
				DictionaryFastChanges()	{ KeyCompareFunc = NULL; }

				void Set(TKey key, TVal val)
				{
					if (KeyCompareFunc)
					{
						for (Pair *p = list.ptr(); p < list.end(); ++p)
						if (KeyCompareFunc(p->key, key) == 0)
						{
							p->val = val;
							return;
						}
					}
					else
					{
						for (Pair *p = list.ptr(); p < list.end(); ++p)
						if (p->key == key)
						{
							p->val = val;
							return;
						}
					}

					list.IncreaseSize(1);
					Pair* end = list.LastPtr();
					end.key = key;
					end->val = val;
				}

				TVal * Get(TKey key)
				{
					if (KeyCompareFunc)
					{
						for (Pair *p = list.ptr(); p < list.end(); ++p)
						if (KeyCompareFunc(p->key, key) == 0)
							return &p->val;
					}
					else
					{
						for (Pair *p = list.ptr(); p < list.end(); ++p)
						if (p->key == key)
							return &p->val;
					}
					return NULL;
				}

				TVal GetV(TKey key)
				{
					if (KeyCompareFunc)
					{
						for (Pair *p = list.ptr(); p < list.end(); ++p)
						if (KeyCompareFunc(p->key, key) == 0)
							return p->val;
					}
					else
					{
						for (Pair *p = list.ptr(); p < list.end(); ++p)
						if (p->key == key)
							return p->val;
					}
					return NULL;
				}

				inline TVal operator[](TKey key)
				{
					if (KeyCompareFunc)
					{
						for (Pair *p = list.ptr(); p < list.end(); ++p)
						if (KeyCompareFunc(p->key, key) == 0)
							return p->val;
					}
					else
					{
						for (Pair *p = list.ptr(); p < list.end(); ++p)
						if (p->key == key)
							return p->val;
					}
				}

				void Remove(TKey key)
				{
					if (KeyCompareFunc)
					{
						for (Pair *p = list.ptr(); p < list.end(); ++p)
						if (KeyCompareFunc(p->key, key) == 0)
						{
							p->key = end->key;
							p->val = end->val;
							list.Pop();
						}
					}
					else
					{
						for (Pair *p = Array, *const end = Array + (Size - 1); p <= end; ++p)
						if (p->key == key)
						{
							p->key = end->key;
							p->val = end->val;
							list.Pop();
						}
					}
				}

				void Clear()
				{
					list.SetLength(0);
				}

			};


			template<class TKey, class TVal>
			class DictionaryFastAccess
			{
			public:
				class Pair
				{
				public:
					TKey key;
					TVal val;
				};

				Array<Pair>		list;

				DictionaryFastAccess()	{ }

				TVal * Get(TKey key)
				{
					Pair *beg = list.ptr();
					Pair *end = list.end();
					Pair *mid;

					while (beg <= end)
					{
						mid = beg + ((end - beg) >> 1);
						if (mid->key == key)
							return &mid->val;
						else if (mid->key < key)
							beg = mid + 1;
						else
							end = mid - 1;
					}

					return NULL;
				}

				TVal * Set(TKey key, TVal val)
				{
					Pair *beg = list.ptr();
					Pair *end = list.end();
					Pair *mid;

					while (beg <= end)
					{
						mid = beg + ((end - beg) >> 1);
						if (mid->key == key)
						{
							mid->val = val;
							return &mid->val;
						}
						else if (mid->key < key)
							beg = mid + 1;
						else
							end = mid - 1;
					}

					Pair p;
					p.key = key;
					p.val = val;
					list.Insert(0, p);

					return list.FirstPtr();
				}

				void Remove(TKey key)
				{
					Pair *beg = list.ptr();
					Pair *end = list.end();
					Pair *mid;

					while (beg <= end)
					{
						mid = beg + ((end - beg) >> 1);
						if (mid->key == key)
						{
							list.Pop();
							for (; mid < list.end(); mid++)
							{
								mid->key = (mid + 1)->key;
								mid->val = (mid + 1)->val;
							}
							return;
						}
						else if (mid->key < key)
							beg = mid + 1;
						else
							end = mid - 1;
					}
				}

				TVal * FindOrAdd(TKey key, Pair *&beg)
				{
					beg = list.ptr();
					Pair *end = list.end();
					Pair *mid;

					while (beg <= end)
					{
						mid = beg + ((end - beg) >> 1);
						if (mid->key == key)
							return &mid->val;
						else if (mid->key < key)
							beg = mid + 1;
						else
							end = mid - 1;
					}

					return NULL;
				}

				void FindOrAdd(TKey key, TVal val, Pair *&beg)
				{
					list.IncreaseSize(1);
					for (Pair *end = list.end(); end > beg; --end)
					{
						end->key = (end - 1)->key;
						end->val = (end - 1)->val;
					}

					beg->key = key;
					beg->val = val;
				}

				void Clear()
				{
					list.SetLength(0);
				}
			};

			template<class TKey, class TVal>
			class Dictionary : public DictionaryFastAccess<TKey, TVal> {};
		}
	}
}

#endif