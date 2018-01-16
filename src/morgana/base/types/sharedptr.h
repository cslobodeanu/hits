#ifndef __MORGANA_BASE_TYPES_SHARED_PTR_H__
#define __MORGANA_BASE_TYPES_SHARED_PTR_H__

namespace MorganaEngine
{
	namespace Base
	{
		namespace Types
		{
			struct ptrinfo_t
			{
				void*			object;
				long			refCount;

				ptrinfo_t()
				{
					object = NULL;
					refCount = 1;
				}
			};

			template<class T>
			class SharedPtr
			{
				ptrinfo_t* ptr;

			public:
				template<typename U> friend class SharedPtr;
				SharedPtr()
				{
					ptr = NULL;
				}

				SharedPtr(T* p)
				{
					New();
					ptr->object = p;
				}

				SharedPtr(const SharedPtr& other)
				{
					if (other.ptr == NULL)
					{
						ptr = NULL;
						return;
					}
					ptr = other.ptr;
					other.ptr->refCount++;
				}

				template <typename U>
				SharedPtr(const SharedPtr<U>& other)
				{
					ptr = other.ptr;
					other.ptr->refCount++;
				}

				~SharedPtr()
				{
					Release();
				}

				template <typename U>
				inline SharedPtr operator = (const SharedPtr<U> p)
				{
					if (ptr == p.ptr)
						return *this;
					Release();

					ptr = p.ptr;

					p.ptr->refCount++;

					return *this;
				}

				template <typename U>
				inline SharedPtr operator = (U* p)
				{
					Release();

					if (ptr == NULL) New();
					ptr->object = p;

					return *this;
				}

				inline SharedPtr& operator = (const SharedPtr&  p)
				{
					if (&p == this)
						return *this;
					if (ptr == p.ptr)
						return *this;

					Release();

					ptr = p.ptr;

					p.ptr->refCount++;

					return *this;
				}

				inline bool operator == (const T*const p) const
				{
					if (ptr == NULL)
					{
						return p == NULL;
					}
					return ptr->object == p;
				}

				inline bool operator != (const T*const p) const
				{
					if (ptr == NULL)
					{
						return p != NULL;
					}
					return ptr->object != p;
				}

				inline T* Ptr() const
				{
					return (T*)ptr->object;// __dynamic_cast<T>((MEObject*)ptr->object);
				}

				inline operator T* () const
				{
					return (T*)ptr->object;// __dynamic_cast<T>((MEObject*)ptr->object);
				}

				/*template<typename U>
				inline operator ObjRef<U> () const
				{
				ObjRef<U> r;
				r.Release();
				r.ptr = ptr;
				return r;
				}*/

				/*template<typename U>
				inline SharedPtr<U> Cast()
				{
				SharedPtr<U> r(*this);

				return r;
				}*/

				inline T* operator -> () const
				{
					return Ptr();
				}

				const void New()
				{
					ptr = new ptrinfo_t();
				}

				const void Release()
				{
					if (ptr == NULL) return;
					ptr->refCount--;
					if (ptr->refCount == 0)
						SAFE_DEL(ptr);
				}

				const void ReleaseObject()
				{
					SAFE_DEL(ptr->object);
				}
			};
		}
	}
}

#endif