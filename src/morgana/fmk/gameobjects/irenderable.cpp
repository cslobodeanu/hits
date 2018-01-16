#include "irenderable.h"
#include "gameobject.h"

using namespace MorganaEngine::Framework::GameObjects;

__implement_class_abstract1(IRenderable);

IRenderable::IRenderable()
{
	rendererFlags = 0;
}
