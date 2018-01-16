#include "camerabackend.h"
#include "camera.h"

using namespace MorganaEngine::Framework::Scene::Cameras;

__implement_component(CameraBackend, Component);

CameraBackend::CameraBackend()
{
	dependsOnCamera = NULL;

	frontend = NULL;
}

CameraBackend::~CameraBackend()
{
	ResourceContext::OnResourceLoaded -= Callback(OnResourceLoaded, MEObject*);
}

void CameraBackend::Start()
{
	super::Start();

	frontend = GetComponent<CameraFrontend>();
	camera = GetComponent<Camera>();

	MakeShaderList();
}

void CameraBackend::Update()
{
	super::Update();
}

void CameraBackend::Flush()
{
	OnPreFlush(frontend->GetFrame());

	UploadGlobalShaderData();

	OnFlush(frontend->GetFrame());
}

void CameraBackend::OnPreFlush(const CameraFrontend::RenderableFrame_t& frame)
{

}

void CameraBackend::OnFlush(const CameraFrontend::RenderableFrame_t& frame)
{

}

void CameraBackend::MakeShaderList()
{
	usedShaders.Clear();

	ResourceContext::OnResourceLoaded += Callback(OnResourceLoaded, MEObject*);

	Array<ShaderProgram*> list = ResourceContext::global->GetLoadedResources<ShaderProgram>();
	for (int i = 0; i < list.Length(); i++)
	{
		if (IsUsingShader(list[i]))
			usedShaders.Add(list[i]);
	}
}

void CameraBackend::OnResourceLoaded(MEObject* obj)
{
	if (obj->IsKindOf(ShaderProgram::AsClassDesc))
	{
		ShaderProgram* sh = (ShaderProgram*)obj;
		if (IsUsingShader(sh))
			usedShaders.Add(sh);
	}
}

void CameraBackend::UploadGlobalShaderData(ShaderProgram* shader)
{

}

void CameraBackend::UploadGlobalShaderData()
{
	for (int i = 0; i < usedShaders.Length(); i++)
		UploadGlobalShaderData(usedShaders[i]);
}

const bool CameraBackend::IsUsingShader(ShaderProgram* shader) const
{
	return false;
}

const void CameraBackend::DependsOn(Camera* cam)
{
	dependsOnCamera = cam;
}
