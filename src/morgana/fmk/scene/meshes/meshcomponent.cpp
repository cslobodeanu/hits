#include "meshcomponent.h"

using namespace MorganaEngine::Framework::Scene::Meshes;

__implement_component(MeshComponent, Component);

MeshComponent::MeshComponent()
{
	mesh = NULL;
}

MeshComponent::~MeshComponent()
{

}