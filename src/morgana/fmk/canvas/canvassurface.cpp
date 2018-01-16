#include "canvassurface.h"
#include "screen.h"
#include "recttransform.h"
#include "meresources.h"
#include "image.h"

using namespace MorganaEngine::Framework::Canvas;
using namespace MorganaEngine::Framework::Renderers;
using namespace MorganaEngine::Framework::Scene::Cameras;

__implement_component(CanvasSurface, Component);

CanvasSurface::CanvasSurface() : Component()
{
	renderCamera = NULL;
	mode = CanvasMode::Overlay;
}

CanvasSurface::~CanvasSurface()
{
}

void CanvasSurface::Awake()
{
	gameObject->AddComponent<RectTransform>();

	Update();

	defaultMaterial = Material::DefaultCanvas;
}

void CanvasSurface::Update()
{
	Scene::Cameras::Camera* cam = renderCamera;

	cam = Camera::main;

	if (mode == CanvasMode::Overlay)
	{
		GetComponent<RectTransform>()->rect = Rectf(0.0f, 0.0f, (float)Screen::width, (float)Screen::height);
	}
	else
	if (mode == CanvasMode::Camera)
		GetComponent<RectTransform>()->rect = Rectf(0.0f, 0.0f, (float)Screen::width, (float)Screen::height);

	if (mode != CanvasMode::World)
	{
		worldMatrix = Matrix::Identity;
		const float nearP = -5000.0f;
		const float farP = 5000.0f;
		projectionMatrix = Matrix::CreateOrthographic(0.0f, viewport.width, viewport.height, 0.0f, nearP, farP);
	}

	viewport = GetComponent<RectTransform>()->rect;
}

void CanvasSurface::OnPreRenderObject()
{
	batches.Begin();
}

void CanvasSurface::OnPostRenderObject()
{
	batches.End();
}

void CanvasSurface::OnRenderObject()
{
	// just nothing
}