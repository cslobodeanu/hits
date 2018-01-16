#include "camerafrontend.h"
#include "camera.h"

using namespace MorganaEngine::Framework::Scene::Cameras;

__implement_component(CameraFrontend, Component);

CameraFrontend::CameraFrontend()
{
	frame.renderQueue.SetLength(Renderer::LastInQueue);
	sharedFrontend = NULL;
}

void CameraFrontend::Awake()
{
	super::Awake();
}

void CameraFrontend::Start()
{
	super::Start();

	camera = GetComponent<Camera>();
}

void CameraFrontend::Update()
{
	super::Update();

	if (sharedFrontend == NULL)
		frame.Reset();
}

void CameraFrontend::PushRenderer(const Renderer* r)
{
	if (sharedFrontend != NULL) return;

	if (r->flags == Renderer::AlwaysVisible || camera->frustum.Contains(&r->bounds))
	{
		frame.renderQueue[r->renderQueue].Add((Renderer*)r);
		FrameStats::current.numRenderables += r->GetDrawablesCount();
	}
}

void CameraFrontend::PushLight(const Light* l)
{
	if (sharedFrontend != NULL) return;

	if (l->type == Light::PointLight)
	{
		if (camera->frustum.Contains(l->transform->position, l->radius) == false)
			return;
	}

	if (l->type == Light::AmbientLight)
		frame.globalAmbientLight = l;
	else
		if (l->type == Light::DirectionalLight)
			frame.globalDirectionalLight = l;
		else
		{
			frame.lights.Add(l);
		}
}

const void CameraFrontend::SetSharedFrontend(CameraFrontend* sharedFrontend)
{
	this->sharedFrontend = sharedFrontend;
	frame.Reset();
}

const CameraFrontend::RenderableFrame_t& CameraFrontend::GetFrame() const
{
	return sharedFrontend ? sharedFrontend->frame : frame;
}
