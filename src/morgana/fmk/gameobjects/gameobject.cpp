#include "gameobject.h"
#include "meutils.h"
#include "../application/meapplication.h"
#include "mescript.h"

using namespace MorganaEngine::Framework::GameObjects;
using namespace MorganaEngine::Framework::Application;

__implement_class(GameObject, MEObject);

GameObject*						TreeNode<GameObject>::root = null;
bool							TreeNode<GameObject>::isDeletingRoot = false;

GameObject*						GameObject::__systemRoot = NULL;

GameObject::GameObject()
{
	SetName(UniqueNameGenerator::Generate("GameObject"));

	layer = NamedMask::LayerFromName("Default");
	if (layer < 0)
	{
		NamedMask::AddLayer("Default");
		layer = 0;
	}

	isEnabled = true;

	data = NULL;

	transform = NULL;
	renderer = NULL;

	components.SetNullOnAlloc();
	AddComponent<Transform>();

	MEApplication::main->CallCreateGameObject(this);

	if (root != NULL)
		SetParent(root);
}

GameObject::~GameObject()
{
	ReleaseComponents();
	MEApplication::main->CallDeleteGameObject(this);
}

void GameObject::Update()
{
	if(isEnabled == false) return;

	FrameStats::current.numGameObjects++;

	UpdateComponents();

	OnUpdate();

	const int cc = GetChildrenCount();
	for (int i = 0; i < cc; i++)
		GetChild(i)->Update();
}

void GameObject::LateUpdate()
{
	if (isEnabled == false) return;

	const int cc = GetChildrenCount();
	for (int i = 0; i < cc; i++)
		GetChild(i)->LateUpdate();

	LateUpdateComponents();
}


void GameObject::OnEnabled()
{

}

const void GameObject::SetUserData(const void* data)
{
	this->data = data;
}

const void* GameObject::GetUserData() const
{
	return data;
}

bool GameObject::CompareNameFunc(const void* obj, const void* strToCompare)
{
	return ((const GameObject*)obj)->name.TestWildcard((const char*)strToCompare);
}

bool GameObject::CompareNameExactFunc(const void* obj, const void* strToCompare)
{
	return ((const GameObject*)obj)->name.Equals((const char*)strToCompare);
}

bool GameObject::CompareTagsFunc(const void* obj, const void* strToCompare)
{
	return ((const GameObject*)obj)->HasTag((const char*)strToCompare);
}

bool GameObject::CompareClassTypeFunc(const void* obj, const void* typeToCompare)
{
	return _Reflection.IsKindOf(((const GameObject*)obj)->GetClassDesc(), (CLASSDESC)typeToCompare);
}

bool GameObject::ObjectIsValid(const void* obj)
{
	if (obj == NULL) return false;
	return ((GameObject*)obj)->IsEnabled();
}

bool GameObject::ObjectHasComponentFunc(const void* obj, const void* componentInfo)
{
	FindComponentInfo_t* fci = (FindComponentInfo_t*)componentInfo;
	GameObject* go = (GameObject*)obj;

	if (fci->cd->IsKindOf(MEScript::AsClassDesc) == false)
	{
		if (fci->handle > go->components.Length() - 1)
			return NULL;

		Component* c = go->components[fci->handle];
		if (c == NULL)
			return false;
		if (c->IsKindOf(fci->cd) == false)
			return false;

		return true;
	}
	else
	{
		return go->GetComponent(fci->cd) != NULL;
	}

	return NULL;
}

void GameObject::ReleaseComponents()
{
	for (int i = 0; i < componentsIndices.Length(); i++)
	{
		SAFE_DEL(components[componentsIndices[i]]);
	}
	components.Clear();
	componentsIndices.Clear();

	for (int i = 0; i < scripts.Length(); i++)
	{
		SAFE_DEL(scripts[i]);
	}

	scripts.Clear();
}

void GameObject::UpdateComponents()
{
	for (int i = 0; i < componentsIndices.Length(); i++)
	{
		Component* c = components[componentsIndices[i]];
		if (c->IsEnabled())
		{
			c->EventuallyStart();
			c->Update();
		}
		else
		{
			c->hasStarted = false;
		}
	}

	for (int i = 0; i < scripts.Length(); i++)
	{
		MEScript *ptr = scripts[i];
		if (ptr == NULL) continue;
		if (ptr->IsEnabled())
		{
			ptr->EventuallyStart();
			ptr->Update();
		}
		else
		{
			ptr->hasStarted = false;
		}
	}

}

void GameObject::LateUpdateComponents()
{
	for (int i = 0; i < componentsIndices.Length(); i++)
	{
		Component* c = components[componentsIndices[i]];
		if (c->IsEnabled())
			c->LateUpdate();
	}

	for (int i = 0; i < scripts.Length(); i++)
	{
		MEScript *ptr = scripts[i];
		if (ptr == NULL) continue;
		if (ptr->IsEnabled())
		{
			ptr->LateUpdate();
		}
	}
}

GameObject* GameObject::Find(const char* path)
{
	if (strchr(path, '/') == NULL && strchr(path, '\\') == NULL)
	{
		if (strchr(path, '*') != NULL)
			return TreeNode<GameObject>::Tree_FindChild(this, CompareNameFunc, path);
		else
			return TreeNode<GameObject>::Tree_FindChild(this, CompareNameExactFunc, path);
	}

	String p(path);
	static Array<String> parts;
	parts.SetLength(0);
	p.Split("\\/", parts);

	GameObject* whereToLook = this;
	for (int i = 0; i < parts.Length(); i++)
	{
		GameObject* go = TreeNode<GameObject>::Tree_FindChildOneLevel(whereToLook, CompareNameExactFunc, parts[i]);
		if (go != NULL)
		{
			whereToLook = go;
			continue;
		}
		else
		{
			return NULL;
		}
	}

	return whereToLook;
}

GameObject* GameObject::FindChild(const char* name)
{
	const int cc = GetChildrenCount();
	for (int i = 0; i < cc; i++)
	{
		GameObject* child = GetChild(i);
		if (child->HasName(name))
			return child;
	}

	return NULL;
}

Component* GameObject::GetComponent(CLASSDESC cd) const
{
	if (cd->IsKindOf(MEScript::AsClassDesc))
	{
		for (MEScript** ptr = scripts.ptr(); ptr < scripts.end(); ptr++)
		{
			if ((*ptr) == NULL) continue;
			if ((*ptr)->IsKindOf(cd) && (*ptr)->IsEnabled())
				return (*ptr);
		}
	}
	else
	{
		for (int i = 0; i < componentsIndices.Length(); i++)
		{
			Component* c = components[componentsIndices[i]];
			if (c->IsEnabled() && c->IsKindOf(cd))
				return c;
		}
	}

	return NULL;
}

Array<Component*> GameObject::GetComponents(CLASSDESC cd) const
{
	Array<Component*> res;

	if (cd->IsKindOf(MEScript::AsClassDesc))
	{
		for (MEScript** ptr = scripts.ptr(); ptr < scripts.end(); ptr++)
		{
			if ((*ptr) == NULL) continue;
			if ((*ptr)->IsKindOf(cd) && (*ptr)->IsEnabled())
			{
				res.Add(*ptr);
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
				res.Add(c);
			}
		}
	}

	return res;
}

void GameObject::RegisterTransform(const Transform* t)
{
	transform = (Transform*)t;

	for (int i = 0; i < componentsIndices.Length(); i++)
	{
		Component* c = components[componentsIndices[i]];
		c->transform = (Transform*)t;
	}

	for (MEScript** ptr = scripts.ptr(); ptr < scripts.end(); ptr++)
	{
		if ((*ptr) == NULL) continue;
		(*ptr)->transform = (Transform*)t;
	}
}

void GameObject::IterateChildren(const GameObject* root, Action<const GameObject*> *actionOnFind, Action<const GameObject*, bool&> *filterAction /*= NULL*/, const bool ignoreNotEnabled /*= true*/)
{
	if (actionOnFind == NULL) return;
	if (root == NULL) return;
	if (root->IsEnabled() == false && ignoreNotEnabled) return;

	if (filterAction != NULL)
	{
		bool ignore = false;
		(*filterAction)(root, ignore);
		if (ignore) return;
	}

	
	(*actionOnFind)(root);

	for (GameObject** child = root->GetChildren(); child < root->GetChildrenEnd(); child++)
	{
		IterateChildren(*child, actionOnFind, filterAction, ignoreNotEnabled);
	}
}

void GameObject::OnParentAttached()
{
	for (int i = 0; i < componentsIndices.Length(); i++)
	{
		Component* cc = components[componentsIndices[i]];
		cc->OnParentAttached();
	}

	for (MEScript** ptr = scripts.ptr(); ptr < scripts.end(); ptr++)
	{
		if ((*ptr) == NULL) continue;
		(*ptr)->OnParentAttached();
	}
}

void GameObject::OnComponentAdded(Component* c)
{
	for (int i = 0; i < componentsIndices.Length(); i++)
	{
		Component* cc = components[componentsIndices[i]];
		cc->OnComponentAdded(c->GetClassDesc());
	}

	for (MEScript** ptr = scripts.ptr(); ptr < scripts.end(); ptr++)
	{
		if ((*ptr) == NULL) continue;
		(*ptr)->OnComponentAdded(c->GetClassDesc());
	}
}

void GameObject::OnComponentRemoved(CLASSDESC t)
{
	for (int i = 0; i < componentsIndices.Length(); i++)
	{
		Component* c = components[componentsIndices[i]];
		if (c != NULL)
			c->OnComponentRemoved(t);
	}

	for (MEScript** ptr = scripts.ptr(); ptr < scripts.end(); ptr++)
	{
		if ((*ptr) == NULL) continue;
		(*ptr)->OnComponentRemoved(t);
	}
}

void GameObject::AssignStandardComponent(Component* c)
{
	if (c->IsKindOf(Transform::AsClassDesc))
	{
		RegisterTransform((Transform*)c);
	}

	if (c->IsKindOf(Renderer::AsClassDesc))
	{
		renderer = (Renderer*)c;
	}

	c->transform = transform;
}

void GameObject::Render()
{	
	if (isEnabled == false) return;

	PreRenderAndCollectComponents();

	RenderComponents();

	const int cc = GetChildrenCount();
	for (int i = 0; i < cc; i++)
		GetChild(i)->Render();

	PostRenderComponents();
}

void GameObject::PreRenderAndCollectComponents()
{
	renderableComponents.Clear();
	for (int i = 0; i < componentsIndices.Length(); i++)
	{
		Component* c = components[componentsIndices[i]];
		if (c->IsEnabled() == false) return;
		IRenderable* ir = __dynamic_cast<IRenderable>(c);
		if (ir != NULL)
		{
			renderableComponents.Add(ir);
			ir->OnPreRenderObject();
		}
	}

	for (MEScript **ptr = scripts.ptr(), **end = scripts.end(); ptr < end; ptr++)
	{
		if ((*ptr) == NULL) continue;
		if ((*ptr)->IsEnabled() == false) return;
		IRenderable* ir = __dynamic_cast<IRenderable>((*ptr));
		if (ir != NULL)
		{
			renderableComponents.Add(ir);
			ir->OnPreRenderObject();
		}
	}
}

void GameObject::PostRenderComponents()
{
	for (IRenderable **r = renderableComponents.ptr(), **end = renderableComponents.end(); r < end; r++)
	{
		(*r)->OnPostRenderObject();
	}
}

void GameObject::RenderComponents()
{
	for (IRenderable **r = renderableComponents.ptr(), **end = renderableComponents.end(); r < end; r++)
	{
		(*r)->OnRenderObject();
	}
}

void GameObject::RemoveComponent(Component* c)
{
	if (c->IsKindOf(MEScript::AsClassDesc))
	{
		for (int i = 0; i < scripts.Length(); i++)
		{
			MEScript* ms = scripts[i];
			if (ms->IsKindOf(c->GetClassDesc()))
			{
				scripts.RemoveNoFast(i);
				break;
			}
		}
	}
	else
	{
		for (int i = 0; i < componentsIndices.Length(); i++)
		{
			if (components[componentsIndices[i]] == c)
			{
				components[componentsIndices[i]] = NULL;
				componentsIndices.Remove(i);
				break;
			}
		}
	}

	OnComponentRemoved(c);
	OnComponentRemoved(c->GetClassDesc());
}

GameObject*	GameObject::_systemRoot()
{
	if (__systemRoot != NULL)
		return __systemRoot;

	__systemRoot = new GameObject();
	__systemRoot->SetName("__system");
	__systemRoot->SetParent(NULL);

	return __systemRoot;
}

MEScript* GameObject::AddScript(const char* name)
{
	MEScript* c = __dynamic_cast<MEScript>(MEObject::Instantiate(name));

	if (c == null) return null;

	c->gameObject = this;

	AssignStandardComponent(c);

	scripts.Add((MEScript*)c);

	OnComponentAdded(c);

	c->Awake();

	return c;
}