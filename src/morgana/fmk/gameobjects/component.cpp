#include "component.h"
#include "gameobject.h"

using namespace MorganaEngine::Framework::GameObjects;

__implement_class(Component, MEObject);

int __componentIndex = 0;
int __registerNewComponent()
{
	int i = __componentIndex;
	__componentIndex++;
	return i;
}

Component::Component()
{
	isEnabled = true;
	gameObject = NULL;
	hasStarted = false;
}

Component::~Component()
{
	OnDestroy();
}

void Component::Awake()
{

}

void Component::Update()
{

}

void Component::LateUpdate()
{

}

void Component::Start()
{

}

void Component::OnComponentAdded(CLASSDESC)
{

}

void Component::OnComponentRemoved(CLASSDESC)
{
}

void Component::OnComponentRemoved(Component*)
{
}

void Component::OnDrawGizmosSelected()
{

}

void Component::OnDrawGizmos()
{

}

void Component::EventuallyStart()
{
	if (hasStarted == false)
	{
		Start();
		hasStarted = true;
	}
}

void Component::OnDestroy()
{
	if (gameObject != NULL)
		gameObject->RemoveComponent(this);
}

void Component::OnParentAttached()
{

}
