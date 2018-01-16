#include "materialcomponent.h"

using namespace MorganaEngine::Framework::Resources::Materials;

__implement_component(MaterialComponent, Component);

MaterialComponent::MaterialComponent()
{
	material = ResourceContext::global->Load<Material>("default");
}

MaterialComponent::~MaterialComponent()
{

}