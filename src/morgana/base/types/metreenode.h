#ifndef __MORGANA_BASE_TYPES_METREE_NODE_H__
#define __MORGANA_BASE_TYPES_METREE_NODE_H__

#include "mearray.h"
#include "../memory/macros.h"
#include "../reflection/callbacks.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace Types
		{
			using namespace Reflection;
			template <class T>
			class TreeNode
			{
			private:
				Array<T*>		children;
				T*				parent;

				static bool		isDeletingRoot;

				void CallTreeChanged()
				{
					if (parent != NULL)
						parent->CallTreeChanged();
					OnTreeChanged((T*)this);
				}
			public:

				Action<T*>		OnTreeChanged;

				static T*		root;

				virtual void	OnChildAdded(T* child)
				{
					CallTreeChanged();
				}
				virtual void	OnChildRemoved(T* child)
				{
					CallTreeChanged();
				}
			protected:
				virtual void	OnParentAttached()
				{
					parent->CallTreeChanged();
				}

				virtual void	OnParentDetached()
				{
					parent->CallTreeChanged();
				}
			private:
				void OnParentAttached_NotifyAll()
				{
					T** ptr = children.ptr();
					T** end = children.end();
					for(; ptr < end; ptr++)
					{
						(*ptr)->OnParentAttached_NotifyAll();
					}
					OnParentAttached();
				}
				void OnParentDetached_NotifyAll()
				{
					T** ptr = children.ptr();
					T** end = children.end();
					for(; ptr < end; ptr++)
					{
						(*ptr)->OnParentDetached_NotifyAll();
					}
					OnParentDetached();
				}
			public:

				inline TreeNode<T>()
				{
					parent = NULL;
				}
				virtual ~TreeNode<T>()
				{
					if (root == (T*)this)
						isDeletingRoot = true;
					SetParent(NULL);
					DeleteAllChildren();
				}

				inline void SetParent(const T* p)
				{
					if (p == parent) return;

					if(parent != NULL)
					{
						OnParentDetached_NotifyAll();
						parent->children.Remove((T*)this);
						parent->OnChildRemoved((T*)this);
					}

					if (p == NULL && !isDeletingRoot)
						parent = (T*)root;
					else
						parent = (T*)p;
					if(parent)
					{
						parent->children.Add((T*)this);
						parent->OnChildAdded((T*)this);
					}

					if (isDeletingRoot == false)
						OnParentAttached_NotifyAll();
				}

				inline T* GetParent()
				{
					return parent;
				}

				inline const T* GetTreeRoot() const
				{
					if(GetParent() == NULL)
						return Myself();

					return GetParent()->GetTreeRoot();
				}

				inline T** GetChildren() const
				{
					return children.ptr();
				}

				inline T* GetChild(const int index)
				{
					return children[index];
				}

				inline T** GetChildrenEnd() const
				{
					return children.end();
				}

				inline const int GetChildrenCount() const
				{
					return children.Length();
				}

				inline void DeleteAllChildren()
				{
					while(children.Length() > 0)
					{
						T* c = children.Last();
						SAFE_DEL(c);
					}
				}

				inline void SetChildIndex(int index)
				{
					const int thisPos = parent->children.Find((T*)this);
					if (thisPos == index) return;

					T* other = parent->children[index];
					parent->children[index] = (T*)this;
					parent->children[thisPos] = other;

					parent->CallTreeChanged();
				}

				inline void SendToBack()
				{
					if (parent->children.First() == this) return;
					SetChildIndex(0);
				}

				inline void BringToFront()
				{
					if (parent->children.Last() == this) return;
					SetChildIndex(parent->children.Length() - 1);
				}

				typedef bool (*FINDCHILDCOMPAREFUNC)(const void* obj, const void* toCompare);
				typedef bool (*OBJECTVALIDFUNC)(const void* obj);

				static void Tree_FindChildren(const T* _this, FINDCHILDCOMPAREFUNC compareFunc, const void *toCompare, Array<T*>& results, const int numResults = -1, OBJECTVALIDFUNC objectValidFunc = NULL)
				{
					if (objectValidFunc != NULL && objectValidFunc(_this) == false)
						return;

					if(compareFunc(_this, toCompare))
						results.Add((T*)_this);

					if(numResults > 0 && results.Length() >= numResults)
						return;

					for(T **ptr = _this->GetChildren(), **end = ptr + _this->GetChildrenCount(); ptr < end; ptr++)
					{
						Tree_FindChildren(*ptr, compareFunc, toCompare, results, numResults);
						if (numResults > 0 && results.Length() >= numResults)
							break;
					}
				}

				static T* Tree_FindChildOneLevel(const T* _this, FINDCHILDCOMPAREFUNC compareFunc, const void *toCompare)
				{
					for (T **ptr = _this->GetChildren(), **end = ptr + _this->GetChildrenCount(), *child = ptr ? *ptr : NULL; ptr < end; ptr++, child = *ptr)
					{
						if (compareFunc(child, toCompare))
							return child;
					}

					return NULL;
				}

				static T* Tree_FindChild(const T* _this, FINDCHILDCOMPAREFUNC compareFunc, const void *toCompare)
				{
					static Array<T*> results;
					results.SetLength(0);
					Tree_FindChildren(_this, compareFunc, toCompare, results, 1);
					return results.Length() ? results.First() : NULL;
				}
			};
		}
	}
}

#endif