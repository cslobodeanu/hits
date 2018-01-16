#ifndef __MORGANA_BASE_TYPES_MEARRAY_H__
#define __MORGANA_BASE_TYPES_MEARRAY_H__

#include <stdlib.h>
#include "debug.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace Types
		{
			template <class T> class Array
			{
				void _safedel()
				{
					if(_buf != NULL)
						delete [] _buf;
					_buf = NULL;
					length = 0;
					cursor = 0;
					truelength = 0;

					if(_sorted_indices != NULL) delete [] _sorted_indices;
					_sorted_indices = NULL;
				}

				T*  _buf;

				int length, cursor;
				int truelength;
				int _indices_length;
				int* _sorted_indices;
				bool setNullOnAlloc;

				int elemsBatchSize = 32;

			public:

				inline const int Length() const { return length;}

				inline const int GetAllocatedLength() const { return truelength;}

				inline Array<T>()
				{
					length = 0;
					cursor = 0;
					truelength = 0;
					_buf = NULL;
					_sorted_indices = NULL;
					_indices_length = 0;
					setNullOnAlloc = false;
				}

				inline Array<T>(int count)
				{
					cursor = 0;
					truelength = 0;
					length = 0;
					_buf = NULL;
					_sorted_indices = NULL;
					_indices_length = 0;
					setNullOnAlloc = false;
					//SetCapacity(count);
					New(count);
					length = 0;
				}

				inline Array<T>(const Array<T>& src)
				{
					length = 0;
					cursor = 0;
					truelength = 0;
					_buf = NULL;
					_sorted_indices = NULL;
					_indices_length = 0;
					setNullOnAlloc = src.setNullOnAlloc;
					SetLength(src.length);
					//memcpy(_buf, src._buf, src.length * sizeof(T));
					//for(int i = 0; i < src.length; i++) _buf[i] = src[i];
					for (T *b = _buf, *s = src.ptr(); s < src.end(); *b = *s, b++, s++);
				}

				inline void SetNullOnAlloc(const bool b = true)
				{
					setNullOnAlloc = b;
				}

				inline void SetElemsBatchSize(const int sz)
				{
					elemsBatchSize = sz;
				}

				virtual ~Array<T>()
				{
					_safedel();
				}

				inline T& operator[](int i) const
				{
					//ASSERT( i < length);
					return _buf[i];
				}

				inline T& Get(int i) const
				{
					//ASSERT( i < length);
					return _buf[i];
				}


				inline T& GetSafe(int i) const
				{
					if (i >= length || i < 0)
						return NULL;
					//ASSERT( i < length);
					return _buf[i];
				}
			
				inline void Alloc(const int capacity)
				{
					_safedel();
					cursor = 0;
					truelength = 0;
					length = 0;
					_buf = NULL;
					_sorted_indices = NULL;
					_indices_length = 0;
					New(capacity);
					length = 0;
				}

				inline void Set(const T& value)
				{
					for(int i = 0; i < length; _buf[i++] = value);
				}

				inline Array<T>& operator = (const Array<T>& src)
				{
					_safedel();
					Copy(src.c_ptr(), src.length);
					return *this;
				}

				inline bool operator != (T* l)
				{
					return _buf != l;        
				}

				inline bool operator == (T* l)
				{
					return _buf == l;        
				}

				operator T* ()
				{
					return _buf;
				}

				inline T* ptr() const
				{
					return _buf;
				}

				inline const T* c_ptr() const
				{
					return _buf;
				}

				inline T* end() const
				{
					return _buf + length;
				}

				inline const unsigned int bufsize() const
				{
					return length * sizeof(T);
				}

				inline void New(const int count)
				{
					if(count > truelength)
					{
						_safedel();
						truelength = elemsBatchSize;
						while(count > truelength)
							truelength *= 2;
						_buf = new T[truelength];

						if (setNullOnAlloc)
						{
							memset(_buf, 0, sizeof(T)* truelength);
						}
					}
					length = count;
				}

			   inline  void New()
				{
					_safedel();
					SetLength(32);
				}

				inline void SetLength(const int& count)
				{
					if(count > truelength)
					{
						New(count);
					}
					else
						length = count;
				}

				inline void IncreaseSize(const int ammount)
				{
					Realloc(length + ammount);
				}

				inline void Realloc(const int& count)
				{
					if (count > truelength)
					{
						if (length > 0)
						{
							Array<T> copy;
							copy.Copy(_buf, length);

							New(count);

							//memcpy(_buf, copy._buf, copy.Length() * sizeof(T));
							for (int i = 0; i < copy.Length(); i++) _buf[i] = copy[i];
						}
						else
						{
							New(count);
						}
					}
					else
						length = count;
				}

				inline void Clear()
				{
					SetLength(0);
				}

				inline void Free()
				{
					_safedel();
				}

				inline void Copy(const T* src, const int& count)
				{
					SetLength(count);
					//memcpy(_buf, src, count * sizeof(T));
					for(int i = 0; i < count; i++) _buf[i] = src[i];
				}

				inline void Append(T *start, int count)
				{
					for(int i =0;i<count;i++)
					{
						Add(start[i]);
					}

				}

				inline void Add(const T& el)
				{
					const int len = length + 1;
					Realloc(len);
					_buf[len-1] = el;
				}

				inline void PushUniqueNoNULL(const T& el)
				{
					if(el && !Contains(el))
					{
						Add(el);
					}
				}

            
 				inline T& Pop()
				{
					//ASSERT(length > 0);
 					length--;
					return _buf[length];
				}

				inline bool IsEmpty() const
				{
					return (length == 0);
				}


				inline T& Last() const
				{
					//ASSERT(length > 0);
					return _buf[length-1];
				}

				inline T* LastPtr()
				{
					//ASSERT(length > 0);
					return _buf + length - 1;
				}

				inline T& First() const
				{
					//ASSERT(length > 0);
					return _buf[0];
				}

				inline T* FirstPtr()
				{
					return _buf;
				}

				inline void SetNo(const int& index, T el)
				{
					if(index >= length)
						Realloc(index + 1);
					_buf[index] = el;
				}

				inline void Insert(const int& index, const T& el)
				{
					Realloc(length + 1);
					if (index == length - 1)
					{
						_buf[length - 1] = el;
						return;
					}
					const int l = length;
					for(int i = l-1; i > index; i--)
						 _buf[i] = _buf[i-1];

					_buf[index] = el;
                

				}

				inline void Remove(const T& el)
				{
					const int l = length;
					for(int i = 0; i < l; i++)
						if(_buf[i] == el)
						{
							for(int j = i + 1; j < l; j++)
								_buf[j - 1] = _buf[j];
							length--;
							break;
						}
				}

				inline void RemoveNo(int index)
				{
					if (index >= length)
						return;
					for(int j = index + 1; j < length; j++)
						_buf[j - 1] = _buf[j];
					length--;
				}

				inline void RemoveNoFast(int index)
				{
					if (index >= length)
						return;
					if(index < length - 1)
					{
						_buf[index] = Last();	
					}
					length--;
				}

				void Reverse(void)
				{
					int mid = Length() >> 1;
					int len = Length() - 1;
					for (int a = 0;a < mid;a += 1)
					{
						Swap(Get(a),Get(len - a));
					}
				}

				inline void Copy(const Array<T>& source)
				{
					Copy(source.c_ptr(), source.length);
				}

				inline void AddArray(Array<T>& addarray)
				{

					//SetLengthAndKeepData(length + addarray.length);
					//memcpy(_buf + length, addarray.ptr(), addarray.length * sizeof(T));

					for(int i = 0 ;i<addarray.length;i++)
						Add(addarray[i]);

				}

				inline int Find(const T& obj) const
				{
					for(int i = 0; i < length; i++)
						if(_buf[i] == obj) return i;

					return -1;
				}

				inline const bool Contains(const T obj) const
				{
					for(int i = 0; i < length; i++)
						if(_buf[i] == obj) return true;

					return false;
				}

				inline void SetCursor(const int& c)
				{
					cursor = c;
				}

				inline int GetCursor()
				{
					return cursor;
				}

				inline T& GetCursorElem()
				{
					return _buf[cursor];
				}

				inline T* GetCursorElemPtr()
				{
					return &(_buf[cursor]);
				}

				inline const unsigned int GetElementSize() const
				{
					return sizeof(T);
				}

				typedef int (*CompareFunc)(T& e1, T& e2);

				inline void Sort(CompareFunc cf)
				{
					if (length == 1) return;
					if((_sorted_indices != NULL && _indices_length < length) || (_sorted_indices == NULL))
					{
						if(_sorted_indices != NULL)
							delete [] _sorted_indices;
						_sorted_indices = new int[length];
						_indices_length = length;
					}
                
					for(int i = 0; i < length; i++) _sorted_indices[i] = i;

					quicksort(0, length - 1, cf);
				}

				inline void Swap(T& e1, T& e2)
				{
					T aux = e1;
					e1 = e2;
					e2 = aux;
				}

				void SwapInd(int i, int j)
				{
					int aux = _sorted_indices[i];
					_sorted_indices[i] = _sorted_indices[j];
					_sorted_indices[j] = aux;

				}

			protected:
				int choose_pivot(int i, int j)
				{
					return ((i+j) >> 1);
				}

				void quicksort(int m, int n, CompareFunc cf)
				{
					int i, j, k;
					T key;
					if(m < n)
					{
						k = choose_pivot(m, n);

						Swap(_buf[m], _buf[k]);
						SwapInd(m, k);

						key = _buf[m];
						i = m + 1;
						j = n;
						while(i <= j)
						{
							while((i <= n) && (cf(_buf[i], key) <= 0))
								i++;
							while((j >= m) && (cf(_buf[j], key) > 0))
								j--;
							if(i < j)
							{
								Swap(_buf[i], _buf[j]);
								SwapInd(i, j);
							}
						}
						// swap two elements
						Swap(_buf[m], _buf[j]);
						SwapInd(m, j);
						// recursively sort the lesser list
						quicksort(m, j - 1, cf);
						quicksort(j + 1, n, cf);
					}
				}

				public:

					// easy porting

					void push_back(const T& elem)
					{
						Add(elem);
					}

					void clear()
					{
						Clear();
					}

					const size_t size() const
					{
						return Length();
					}

					void erase(int pos)
					{
						RemoveNo(pos);
					}

					const int begin() const
					{
						return 0;
					}

					T& back() const
					{
						return Last();
					}

					const void insert(const int pos, const T& val)
					{
						Insert(pos, val);
					}
			};
		}
	}
}

#endif