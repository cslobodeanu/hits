#include "renderer.h"
#include "meresources.h"
#include "../resources/materials/materialcomponent.h"

using namespace MorganaEngine::Framework::Renderers;
using namespace MorganaEngine::Framework::Resources::Materials;

__implement_component_abstract(Renderer, Component);

Renderer::Renderer()
{
	ResetRenderable();
	bounds = Box::Zero;
	materialToUse = NULL;
	sharedMaterial = NULL;
	flags = 0;
	renderQueue = Opaque;
	castShadows = true;

	useInstancing = false;
	useBatching = false;
}

Renderer::~Renderer()
{
	CleanupMaterials();
}

void Renderer::Update()
{
	ResetRenderable();
	UpdateRenderables();
}

const Renderer::Drawable_t& Renderer::GetDrawable(const int index /*= 0*/) const
{
	return renderables[index];
}

const int Renderer::GetDrawablesCount() const
{
	return renderables.Length();
}

void Renderer::ResetRenderable()
{
	renderables.SetLength(0);
}

const Matrix* Renderer::GetCustomProjectionView() const
{
	return NULL;
}

void Renderer::Start()
{
	super::Start();

	RefreshMaterial();
}

void Renderer::OnComponentAdded(CLASSDESC ci)
{
	if (ci->IsKindOf(MaterialComponent::AsClassDesc))
	{
		RefreshMaterial();
	}
}

void Renderer::RefreshMaterial()
{
	MaterialComponent* mc = GetComponent<MaterialComponent>();
	if (mc)
	{
		sharedMaterial = mc->material;
		materialToUse = mc->material;
	}
}

Material* Renderer::writeMaterial()
{
	const Material* srcMat = sharedMaterial != NULL ? sharedMaterial : defaultMaterial();

	if ((srcMat == materialToUse && srcMat != NULL) || (defaultMaterial() != NULL && materialToUse == NULL))
	{
		CleanupMaterials();

		materialToUse = new Material(*srcMat);
		materialToUse->SetName(String::Format("%s's instance", srcMat->GetName().c_str()));

		sharedMaterial = srcMat;
	}

	ASSERT(materialToUse != NULL, "No material !");

	return materialToUse;
}

const Material* Renderer::readMaterial()
{
	return materialToUse;
}

void Renderer::CleanupMaterials()
{
	if (sharedMaterial != materialToUse)
		SAFE_DEL(materialToUse);
}

const Material* Renderer::defaultMaterial()
{
	return NULL;
}
