#ifndef __MORGANA_FMK_GAME_OBJECTS_GAME_OBJECT_H__
#define __MORGANA_FMK_GAME_OBJECTS_GAME_OBJECT_H__

#include "metypes.h"
#include "reflection.h"
#include "mescript.h"
#include "transform.h"
#include "base.h"
#include "../renderers/renderer.h"
#include "irenderable.h"


namespace MorganaEngine
{
	namespace Framework
	{
		using namespace Renderers;
		namespace GameObjects
		{
			using namespace Base;
			class GameObject : public TreeNode<GameObject>, public MEObject, public IsNamed, public IsTagged
			{
				__declare_class(GameObject, MEObject);
			public:

				GameObject();
				virtual ~GameObject();

				inline const void	Enable(const bool enable = true) { if(enable != isEnabled) { isEnabled = enable; OnEnabled(); }}
				inline const void	Disable() { Enable(false); }
				inline const bool	IsEnabled() const { return isEnabled; }

				void				Update();
				void				LateUpdate();

				const void			SetUserData(const void* data);
				const void*			GetUserData() const;

				template<class T>
				T*					FindParent();

				template<class typeToFind>
				Array<typeToFind*>	FindChildren(const int numResults = -1);
				template<class typeToFind>
				typeToFind*			Find();
				GameObject*			Find(const char* path);
				GameObject*			FindChild(const char* name);

				template<class T>
				T*					AddComponent();

				template<class T>
				void				RemoveComponent();

				void				RemoveComponent(Component* c);

				template<class T>
				T*					RequestComponent();

				template<class T>
				T*					GetComponent() const;

				Component*			GetComponent(CLASSDESC cd) const;
				Array<Component*>	GetComponents(CLASSDESC cd) const;
				template<class T>
				Array<T*>			GetComponents() const;

				template<class T>
				T*					GetComponentInChildren();
				template<class T>
				Array<T*>			GetComponentsInChildren();
				template<class T>
				T*					GetComponentInParent();

				Transform*			transform;
				Renderer*			renderer;

				int					layer;

				static void			IterateChildren(const GameObject* root, Action<const GameObject*>* actionOnFind, Action<const GameObject*, bool&> *filterAction = NULL, const bool ignoreNotEnabled = true);

				void				Render();

				static GameObject*	_systemRoot();

				template<class T>
				static T*			InitWithComponent();				

				template<class T1, class T2>
				static T1*			InitWithComponents();

				template<class T1, class T2, class T3>
				static T1*			InitWithComponents();
				
			protected:

				virtual void		OnEnabled();
				virtual void		OnUpdate() {}

				virtual void		OnParentAttached();

				virtual void		OnComponentAdded(Component* c);
				virtual void		OnComponentRemoved(CLASSDESC);
				virtual void		OnComponentRemoved(Component* c) {}

			private:
				bool				isEnabled;
				const void*			data;

				static GameObject*	__systemRoot;

				Array<Component*>		components;
				Array<MEScript*>	scripts;
				Array<unsigned short>	componentsIndices;
				void				ReleaseComponents();
				void				UpdateComponents();
				void				LateUpdateComponents();

				void				PreRenderAndCollectComponents();
				void				PostRenderComponents();
				void				RenderComponents();

				static bool			CompareNameFunc(const void* obj, const void* strToCompare);
				static bool			CompareNameExactFunc(const void* obj, const void* strToCompare);
				static bool			CompareTagsFunc(const void* obj, const void* strToCompare);
				static bool			CompareClassTypeFunc(const void* obj, const void* typeToCompare);
				static bool			ObjectHasComponentFunc(const void* obj, const void* componentInfo);
				static bool			ObjectIsValid(const void* obj);

				void				RegisterTransform(const Transform* t);
				void				AssignStandardComponent(Component* c);

				Array<IRenderable*> renderableComponents;


				struct FindComponentInfo_t
				{
					CLASSDESC cd;
					int handle;
				};
			};

			template<class T>
			T* GameObject::AddComponent()
			{
				ASSERT(T::AsClassDesc->IsKindOf(Component::AsClassDesc));

				T* c = MEObject::Instantiate<T>();
				c->gameObject = this;

				AssignStandardComponent(c);

				if (T::AsClassDesc->IsKindOf(MEScript::AsClassDesc))
				{
					scripts.Add((MEScript*)c);
				}
				else
				{
					components.SetNo(T::Handle, c);
					if (componentsIndices.Contains(T::Handle) == false)
						componentsIndices.Add(T::Handle);
				}

				OnComponentAdded(c);

				c->Awake();

				return c;
			}

			template<class T>
			void GameObject::RemoveComponent()
			{
				if (T::AsClassDesc->IsKindOf(MEScript::AsClassDesc))
				{
					for (int i = 0; i < scripts.Length(); i++)
					{
						MEScript* ms = scripts[i];
						if (ms->IsKindOf(T::AsClassDesc))
						{
							OnComponentRemoved(T::AsClassDesc);

							SAFE_DEL(ms);
							scripts.RemoveNoFast(i);
							i--;
						}
					}
				}
				else
				{
					Component* c = components[T::Handle];
					if (c != NULL)
					{
						OnComponentRemoved(c);
						componentsIndices.Remove(T::Handle);
					}
					SAFE_DEL(components[T::Handle]);
				}

				OnComponentRemoved(T::AsClassDesc);
			}

			template<class T>
			T* GameObject::RequestComponent()
			{
				T* c = GetComponent<T>();
				if (c == NULL)
					c = AddComponent<T>();
				return c;
			}

			template<class T>
			T* GameObject::GetComponent() const
			{
				if (T::customScript == false)
				{
					Component* c = components[T::Handle];
					if (c == NULL || c->IsKindOf(T::AsClassDesc) == false)
						return NULL;

					return (T*)c;
				}
				else
				{
					return (T*)GetComponent(T::AsClassDesc);// .Cast<T>();
				}

				return NULL;
			}

			template<class T>
			Array<T*>		GameObject::GetComponents() const
			{
				Array<T*> res;

				const _CLASSDESC* cd = T::AsClassDesc;

				if (cd->IsKindOf(MEScript::AsClassDesc))
				{
					for (MEScript** ptr = scripts.ptr(); ptr < scripts.end(); ptr++)
					{
						if ((*ptr) == NULL) continue;
						if ((*ptr)->IsKindOf(cd) && (*ptr)->IsEnabled())
						{
							res.Add(__dynamic_cast<T>((*ptr).Ptr()));
						}
					}
				}
				else
				{
					for (int i = 0; i < componentsIndices.Length(); i++)
					{
						Component* c = components[componentsIndices[i]];
						if (c->IsEnabled() && c->IsKindOf(cd))
						{
							res.Add(__dynamic_cast<T>(c.Ptr()));
						}
					}
				}

				return res;
			}

			template<class T>
			T* GameObject::GetComponentInChildren()
			{
				static Array<GameObject*> _results;
				_results.SetLength(0);
				FindComponentInfo_t fci;
				fci.handle = T::Handle;
				fci.cd = T::AsClassDesc;
				Tree_FindChildren(this, ObjectHasComponentFunc, (const void*)&fci, _results, 1, ObjectIsValid);

				if (_results.Length() == 1)
					return _results.First()->GetComponent<T>();

				return NULL;
			}

			template<class T>
			Array<T*> GameObject::GetComponentsInChildren()
			{
				static Array<GameObject*> _results;
				_results.SetLength(0);

				FindComponentInfo_t fci;
				fci.handle = T::Handle;
				fci.cd = T::AsClassDesc;
				Tree_FindChildren(this, ObjectHasComponentFunc, (const void*)&fci, _results, -1, ObjectIsValid);

				static Array<T*> compRes;
				compRes.SetLength(0);
				for (int i = 0; i < _results.Length(); i++)
				{
					T* c = _results[i]->GetComponent<T>();
					if (c != NULL)
						compRes.Add(c);
				}

				return compRes;
			}

			template<class T>
			T* GameObject::FindParent()
			{
				GameObject* p = GetParent();
				if (p == NULL) return NULL;

				if (p->IsKindOf(T::AsClassDesc))
					return (T*)p;

				return p->FindParent<T>();
			}

			template<class T>
			T* GameObject::GetComponentInParent()
			{
				GameObject* p = GetParent();
				if (p == NULL) return NULL;

				if (p->IsEnabled())
				{
					T* c = p->GetComponent<T>();
					if (c != NULL)
						return c;
				}

				return p->GetComponentInParent<T>();
			}

			template<class typeToFind>
			typeToFind* GameObject::Find()
			{
				GameObject* obj = TreeNode<GameObject>::Tree_FindChild(this, CompareClassTypeFunc, typeToFind::AsClassDesc);
				if (obj == NULL) return NULL;
				return __dynamic_cast<typeToFind>(obj);
			}

			template<class typeToFind>
			Array<typeToFind*> GameObject::FindChildren(const int numResults /*= -1*/)
			{
				static Array<GameObject*> _results;
				_results.SetLength(0);
				Tree_FindChildren(this, CompareClassTypeFunc, typeToFind::AsClassDesc, _results, numResults);

				Array<typeToFind*> results;
				results.SetLength(_results.Length());
				memcpy(results.ptr(), _results.ptr(), _results.ptrsize());
				return results;
			}

			template<class T>
			static T* GameObject::InitWithComponent()
			{
				GameObject* obj = new GameObject();
				obj->SetName(T::AsClassDesc->className);
				return obj->AddComponent<T>();
			}

			template<class T1, class T2>
			T1* GameObject::InitWithComponents()
			{
				GameObject* obj = new GameObject();
				obj->SetName(T1::AsClassDesc->className);
				T1* t1 = obj->AddComponent<T1>();
				obj->AddComponent<T2>();
				return t1;
			}

			template<class T1, class T2, class T3>
			T1* GameObject::InitWithComponents()
			{
				GameObject* obj = new GameObject();
				obj->SetName(T1::AsClassDesc->className);
				T1* t1 = obj->AddComponent<T1>();
				obj->AddComponent<T2>();
				obj->AddComponent<T3>();
				return t1;
			}
		}
	}
}

#endif