#ifndef __MORGANA_BASE_REFLECTION_OBJECT_PTR_REF_H__
#define __MORGANA_BASE_REFLECTION_OBJECT_PTR_REF_H__

namespace MorganaEngine
{
	namespace Base
	{
		namespace Reflection
		{
			template<class T>
			class ObjectPtrRef : public EmptyType
			{
				T* object;
				void OnObjectDestroyed(MEObject* who)
				{
					if (who == object)
					{
						object = NULL;
					}
				}
			public:
				ObjectPtrRef()
				{
					object = NULL;
					MEObject::OnObjectDestroy += CallbackT(ObjectPtrRef, OnObjectDestroyed, MEObject*);
				}

				ObjectPtrRef(const T* who)
				{
					object = who;
					MEObject::OnObjectDestroy += CallbackT(ObjectPtrRef, OnObjectDestroyed, MEObject*);
				}

				~ObjectPtrRef()
				{
					MEObject::OnObjectDestroy -= CallbackT(ObjectPtrRef, OnObjectDestroyed, MEObject*);
				}

				operator T*()
				{
					return object;
				}

				template<class U>
				ObjectPtrRef<T>& operator = (U* right)
				{
					object = __dynamic_cast<T>(right);
					return *this;
				}

				inline T* operator -> () const
				{
					return object;
				}
			};
		};
	}
}

#endif