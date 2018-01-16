#ifndef __MORGANA_BASE_TYPES_SMART_PTR_H__
#define __MORGANA_BASE_TYPES_SMART_PTR_H__

namespace MorganaEngine
{
	namespace Base
	{
		namespace Types
		{
			template<class T>
			class SmartPtr
			{
				int		refCount;
				T*		ptr;
			public:
				SmartPtr()
				{
					refCount = 0;
					ptr = NULL;
				}

				SmartPtr(const T* p)
				{
					ptr = (T*)p;
					refCount = 1;
				}

				SmartPtr(const SmartPtr<T>& other)
				{
					ptr = other.ptr;
					refCount = other.refCount;
					refCount++;
				}

				virtual ~SmartPtr()
				{
					refCount--;
					if (refCount == 0)
						delete ptr;
				}

				inline void Release()
				{
					refCount--;
					if (refCount <= 0 && ptr)
						delete ptr;
				}

				inline operator T* ()
				{
					return ptr;
				}

				inline T& operator* ()
				{
					return *ptr;
				}

				inline T* operator-> ()
				{
					return ptr;
				}

				inline SmartPtr<T>& operator = (const SmartPtr<T>& sp)
				{
					if (this != &sp)
					{
						refCount--;
						if (refCount == 0)
						{
							delete ptr;
						}
						ptr = sp.ptr;
						refCount = sp.refCount;
						refCount++;
					}
					return *this;
				}
			};
		}
	}
}

#endif