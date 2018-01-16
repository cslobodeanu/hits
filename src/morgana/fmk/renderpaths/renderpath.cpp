#include "renderpath.h"
#include "mescene.h"

using namespace MorganaEngine::Framework::RenderPaths;
using namespace MorganaEngine::Framework;

__implement_class_abstract(RenderPath, MEObject);

ShaderProgram* RenderPath::invalidProgram = NULL;

RenderPath::RenderPath() : MEObject()
{
	if (invalidProgram == NULL)
		invalidProgram = ResourceContext::global->Load<ShaderProgram>("invalid.vsh;invalid.fsh");
}

RenderPath::~RenderPath()
{
}

void RenderPath::Setup(Camera* cam)
{
	ASSERT(false, "Add frontends/backends or other cameras here");
}

void RenderPath::SetMaterial(const Material* mat)
{
	if (mat == NULL) return;

	DisplayDevice::main->SetPipelineState(mat->depthTest);
	DisplayDevice::main->SetPipelineState(mat->cull);
	DisplayDevice::main->SetPipelineState(mat->blend);

	for (int i = 0; i < mat->textures.Length(); i++)
	{
		if (mat->textures[i] != NULL)
			DisplayDevice::main->BindObject(mat->textures[i], i);
	}

	mat->shader->SetFloat("shininess", mat->shininess);
	mat->shader->SetColor("specularColor", mat->specular);
	mat->shader->SetColor("emissiveColor", mat->emissive);

	((Material*)mat)->UploadParameters(Material::ParameterSet::Default);
	((Material*)mat)->UploadParameters(Material::ParameterSet::User);
}

void RenderPath::DrawIndexed(const Renderer::Drawable_t& r)
{
	DisplayDevice::main->BindObject(r.vbo);
	DisplayDevice::main->BindObject(r.ibo);

	DisplayDevice::main->SetInputLayout(r.vertexLayout);
	DisplayDevice::main->DrawIndexed(r.drawMode, r.indexCount, r.vertexOffset, r.indexOffset);
}

ShaderProgram* RenderPath::SetProgram(ShaderProgram* prg)
{
	if (prg->valid)
	{
		DisplayDevice::main->BindObject(prg);
		return prg;
	}

	DisplayDevice::main->BindObject(invalidProgram);
	return invalidProgram;
}
