#ifndef __MORGANA_BASE_REFLECTION_MEOBJECT_H__
#define __MORGANA_BASE_REFLECTION_MEOBJECT_H__

#include "meclasslib.h"
#include "platform.h"
#include "meutils.h"
#include "callbacks.h"

#ifdef GetClassDesc
#undef GetClassDesc
#endif

using namespace MorganaEngine::Base::Types;

namespace MorganaEngine
{
	namespace Base
	{
		namespace Reflection
		{
			class _propertyDef;

			class MEObject : public EmptyType
			{
				friend class _propertyDef;

				__declare_class_abstract1(MEObject);

			public:

				MEObject();
				virtual ~MEObject();

				virtual  String			GetDescription();

				/** copies all the properties **/
				virtual MEObject*		Clone();

				virtual void*			DynamicCast(const _CLASSDESC* cd) const;

				static Action<MEObject*>	OnObjectDestroy;

				String					uid;

			protected:

				virtual void            DoAddProperties();

				/** called when a property had changed its value **/

				virtual void            OnPropertySet(_propertyDef* atr) {}
				virtual const void		OnPropertyGet(const _propertyDef* atr, void* ptrVal) const {}

			public:

				virtual bool			Equals(MEObject* other);


				static MEObject*		Instantiate(const char* className);
				static MEObject*		Instantiate(CLASSDESC type);
				template<class T>
				static T*				Instantiate();
			};

			template<class T>
			T* MEObject::Instantiate()
			{
				return _Reflection.Instantiate<T>();
			}

		}
    }
}

template<class T>
T* __dynamic_cast(const MorganaEngine::Base::Reflection::MEObject* obj)
{
	if (obj == NULL) return NULL;
	return (T*)obj->DynamicCast(T::AsClassDesc);
}

#endif



