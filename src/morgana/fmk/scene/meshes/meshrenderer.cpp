#include "meshrenderer.h"
#include "mescene.h"
#include "meshcomponent.h"

using namespace MorganaEngine::Framework::Scene::Meshes;

__implement_class(MeshRenderer, Renderer);

MeshRenderer::MeshRenderer()
{

}

MeshRenderer::~MeshRenderer()
{

}

void MeshRenderer::UpdateRenderables()
{
	MeshComponent* mc = GetComponent<MeshComponent>();
	if (mc == NULL) return;

	Drawable_t r;

	r.vbo = (Buffer*)mc->mesh->gpuDataVertices->GetHandle();
	r.ibo = (Buffer*)mc->mesh->gpuDataIndices->GetHandle();
	r.vertexOffset = mc->mesh->gpuDataVertices->GetOffset();
	r.indexOffset = mc->mesh->gpuDataIndices->GetOffset();
	r.indexCount = mc->mesh->numIndices;
	r.vertexCount = mc->mesh->numVertices;
	r.vertexLayout = mc->mesh->vertexLayout;

	renderables.Add(r);

	Box::Transform(&mc->mesh->bounds, transform->GetWorldMatrixPtr(), &bounds);
}