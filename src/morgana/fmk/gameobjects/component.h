#ifndef __MORGANA_FMK_DESKTOP_COMPONENT_MODIFIER_H__
#define __MORGANA_FMK_DESKTOP_COMPONENT_MODIFIER_H__

#include "metypes.h"
#include "reflection.h"

namespace MorganaEngine
{
	namespace Framework
	{
		namespace GameObjects
		{
			class GameObject;
			class Transform;
			class Component : public MEObject
			{
				__declare_class(Component, MEObject);

				bool	isEnabled;

			public:
				Component();
				virtual ~Component();

				GameObject*				gameObject;
				Transform*				transform;

				const bool IsEnabled() const { return isEnabled; }
				void Enable(const bool b) { isEnabled = b; }

				virtual void Awake();

				template<class T>
				T*	GetComponent()
				{
					return gameObject->GetComponent<T>();
				}

			protected:
				friend class GameObject;
				virtual void Update();
				virtual void LateUpdate();

				bool		 hasStarted;
				void		 EventuallyStart();
				virtual void Start();

				virtual void OnDestroy();

				virtual void OnComponentAdded(CLASSDESC);
				virtual void OnComponentRemoved(CLASSDESC);
				virtual void OnComponentRemoved(Component* c);

				virtual void OnParentAttached();

				virtual void OnDrawGizmosSelected();
				virtual void OnDrawGizmos();
			};
		}
	}
}

#define __declare_component(name, base)  \
	__declare_class(name, base); \
public: \
	static int	Handle; \
	static bool customScript; \
private:

#define __declare_component_abstract(name, base)  \
	__declare_class_abstract(name, base); \
public: \
	static int	Handle; \
	static bool customScript; \
private:

#define __implement_component(name, base) \
	__implement_class(name, base); \
	extern int __registerNewComponent(); \
	int name::Handle = __registerNewComponent(); \
	bool name::customScript = false;

#define __implement_component_empty(name, base) \
	__implement_class(name, base); \
	int name::Handle = -1; \
	bool name::customScript = false;

#define __declare_morgana_script(name, base) __declare_component(name, base)
#define __implement_morgana_script(name, base) \
	__implement_class(name, base); \
	int name::Handle = -1; \
	bool name::customScript = true;

#define __declare_morgana_script1(name) __declare_morgana_script(name, MEScript)
#define __implement_morgana_script1(name) __implement_morgana_script(name, MEScript)

#define __implement_component_abstract(name, base) \
	__implement_class_abstract(name, base); \
	extern int __registerNewComponent(); \
	int name::Handle = __registerNewComponent(); \
	bool name::customScript = false;

#endif