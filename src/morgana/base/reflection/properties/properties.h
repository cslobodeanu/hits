#ifndef __MORGANA_BASE_REFLECTION_PROPERTIES_H__
#define __MORGANA_BASE_REFLECTION_PROPERTIES_H__

#include "meobject.h"
#include "metypes.h"
#include "memath.h"
#include "../callbacks.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace Reflection
		{
			class _propertyDef : public EmptyType
			{
			protected:
				MEObject*	pOwner;
			public:

				_propertyDef();

				virtual String	ToString() const = 0;
				virtual void	FromString(const char*) = 0;

				inline const void SetOwner(MEObject* obj)
				{
					pOwner = obj;
				}

				inline MEObject* GetOwner() const
				{
					return pOwner;
				}            

				virtual void	InvalidateValue();

				virtual String&	GetName();
				virtual String&	GetDisplayName();

				virtual bool	Equals(_propertyDef* other);
			};

			template<class T>
			class _property : public _propertyDef
			{
			protected:
				T value;

				void OnAssignValue(const T& v)
				{
					if (value != v)
					{
						value = v;
						InvalidateValue();
					}
				}
			public:

				_property() {}

				_property(const T & v)
				{
					pOwner = NULL;
					OnAssignValue(v);
				}

				const T* operator->() const
				{
					return &value;
				}

				template<class U>
				void operator *= (const U& other)
				{
					OnAssignValue(((T)value) * (U)other);
				}

				template<class U>
				void operator += (const U& other)
				{
					OnAssignValue(value + other);
				}

				template<class U>
				void operator -= (const U& other)
				{
					OnAssignValue(value - other);
				}

				template<class U>
				const T operator * (const U& other) const
				{
					return (T)value * other;
				}

				template<class U>
				T operator + (const U& other) const
				{
					return value + other;
				}

				T operator + (const _property<T>& other) const
				{
					return value + other.value;
				}

				inline operator T() const
				{
					return value;
				}

				template<class U>
				inline bool operator ==(U other) { return value == other; }

				template<class U>
				inline bool operator != (U other) { return value != other; }

				template<class U>
				_property<T>& operator = (U v)
				{
					OnAssignValue(v);
					return *this;
				}

				virtual String	ToString() const;
				virtual void	FromString(const char*);
			};

			template <class T>
			class _property_object : public _propertyDef
			{
				T* value;

				void OnObjectDestroyed(MEObject* who)
				{
					if (value == who)
					{
						value = NULL;
					}
				}
			public:

				_property_object()
				{
					MEObject::OnObjectDestroy += CallbackT(_property_object<T>, OnObjectDestroyed, MEObject*);
					value = NULL;
				}

				virtual ~_property_object()
				{
					MEObject::OnObjectDestroy -= CallbackT(_property_object<T>, OnObjectDestroyed, MEObject*);
				}

				template<class U>
				_property_object<T>& operator = (U* v)
				{
					if (value != v)
					{
						value = v;
						InvalidateValue();
					}
					return *this;
				}

				template<class U>
				_property_object<T>& operator = (const U* v)
				{
					if (value != v)
					{
						value = (T*)v;
						InvalidateValue();
					}
					return *this;
				}

				_property_object<T>& operator = (int v)
				{
					if (value != (T*)v)
					{
						value = (T*)v;
						InvalidateValue();
					}
					return *this;
				}

				const T* operator->() const
				{
					return value;
				}

				T* operator->()
				{
					return value;
				}

				inline operator T* () const
				{
					return value;
				}

				template<class U>
				inline bool operator != (U other) { return value != (T*)other; }

				template<class U>
				inline bool operator == (U other) { return value == (T*)other; }

				virtual String	ToString() const
				{
					MEObject* ob = (MEObject*)value;
					if (ob == NULL)
						return "";
					else return ob->uid;
				}

				virtual void	FromString(const char* s)
				{

				}
			};
		}
	}
}

#endif
