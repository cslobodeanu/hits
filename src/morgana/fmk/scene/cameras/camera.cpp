#include "camera.h"
#include "meutils.h"
#include "devices.h"
#include "framework.h"


using namespace MorganaEngine::Framework::Scene::Cameras;
using namespace MorganaEngine::Framework::RenderPaths;

__implement_component(Camera, Component);

Camera*			Camera::main = NULL;
Array<Camera*>	Camera::allCameras;

Camera*			Camera::systemCameras[Camera::SystemCameraTypes::SystemCamerasCount];

/*
#define _PP_RSEQ_N() 8,7,6,5,4,3,2,1,0
#define _PP_ARG_N(_1,_2,_3,_4,_5,_6,_7,_8, N,...) N
#define ___PP_ARG_N(args_list) _PP_ARG_N args_list
#define _PP_NARG_(...) ___PP_ARG_N((__VA_ARGS__))
#define __PP_NARG_(args_list) _PP_NARG_ args_list
#define _PP_NARG(...) __PP_NARG_((__VA_ARGS__,_PP_RSEQ_N()))*/

Camera::Camera()
{
	int p1 = 1;
	int p2 = 2;
	int p3 = 3;
	int p4 = 4;
	//int k = PP_NARG(p1, p2, p3, p4);

	fieldOfView = 45.0f;
	nearPlane = -5000.0f;
	farPlane = 5000.0f;

	projectionType = Parallel;
	orthographicSize = 1.0f;

	viewport = Rectf(0.0f, 0.0f, 1.0f, 1.0f);

	depth = 0.0f;

	clearFlags = ClearEverything;

	target = RenderTarget::Screen;
	backgroundColor = Color255(49, 77, 121);

	renderPathType = RenderPathForward::AsClassDesc;
	renderPath = NULL;
	
	renderMask.MarkAll();

	nodeToRender = NULL;

	systemCameraIndex = 0;

	DoAddProperties();
}

Camera::~Camera()
{
	RemoveCamera(this);
}

void Camera::Awake()
{
	super::Awake();

	AddCamera(this);

	gameObject->AddComponent<CameraFrontend>();
}

void Camera::UpdateProjectionMatrix(const float nearP, const float farP, Matrix& result)
{
	Rect vp = NormalizedViewport(target);
	if(projectionType == Parallel)
	{
		result = Matrix::CreateOrthographic(-orthographicSize, orthographicSize, orthographicSize, -orthographicSize, nearPlane, farPlane);
	}
	else if(projectionType == PerspectiveInfinite)
	{
		result = Matrix::CreatePerspectiveInfinite(fieldOfView, vp.Aspect());
	}
	else
	{
		result = Matrix::CreatePerspective(fieldOfView, vp.Aspect(), nearP, farP);
	}
}

const Matrix& Camera::GetProjectionMatrix() const
{
	return projectionMatrix;
}

void Camera::DoAddProperties()
{
	super::DoAddProperties();

	__add_property1(nearPlane);
	__add_property1(farPlane);
	__add_property1(fieldOfView);
	__add_property1(orthographicSize);
}

void Camera::OnPropertySet(_propertyDef* p)
{
	super::OnPropertySet(p);

	if (p == &nearPlane || p == &farPlane || p == &fieldOfView || p == &orthographicSize || p == &projectionType || p == &viewport)
	{
		UpdateProjectionMatrix(nearPlane, farPlane, projectionMatrix);
	}
}

void Camera::Update()
{
	super::Update();

	UpdateProjectionMatrix(nearPlane, farPlane, projectionMatrix);

	Matrix viewMat = transform->GetWorldMatrix();
	viewMat.Invert();

	frustum.Update(&projectionMatrix, &viewMat);

	CameraRender::activeCameras.Add(this);
}

void Camera::AddCamera(Camera* c)
{
	allCameras.Add(c);
	if (main == NULL)
		main = c;
}

void Camera::RemoveCamera(Camera* c)
{
	allCameras.Remove(c);
	if (main == c)
	{
		if (allCameras.Length())
			main = allCameras.First();
		else
			main = NULL;
	}
}

void Camera::SetupBackground()
{
	if (target == NULL)
		target = RenderTarget::Screen;

	DisplayDevice::main->BindObject(target);

	const Rect vp = NormalizedViewport(target);
	DisplayDevice::main->SetViewport(vp);

	if (clearFlags != ClearNothing)
	{
		if (clearFlags == ClearEverything)
			DisplayDevice::main->ClearBuffer(MDISPLAY_BUFFER_COLOR | MDISPLAY_BUFFER_DEPTH, backgroundColor);
		else
		if (clearFlags == ClearColor)
			DisplayDevice::main->ClearBuffer(MDISPLAY_BUFFER_COLOR, backgroundColor);
		else
		if (clearFlags == ClearDepth)
			DisplayDevice::main->ClearBuffer(MDISPLAY_BUFFER_DEPTH);
	}
}

void Camera::CreateRenderPath()
{
	if (systemCameraIndex > 0) return;

	ASSERT(renderPathType->IsKindOf(RenderPath::AsClassDesc));
	//renderPath = (RenderPath*)_Reflection.GetClassInstance(renderPathType);
	if (renderPath == NULL || renderPath->IsKindOf(renderPathType) == false)
	{
		SAFE_DEL(renderPath);
		renderPath = (RenderPath*)MEObject::Instantiate(renderPathType);
		renderPath->Setup(this);
	}
}

Rect Camera::NormalizedViewport(RenderTarget* rt)
{
	Rect r;
	const Rect bounds = rt->GetBounds();
	r.x = (int)Mathf::Floor(viewport->x * bounds.width + bounds.x);
	r.y = (int)Mathf::Floor(viewport->y * bounds.height + bounds.y);
	r.width = (int)Mathf::Ceil(viewport->width * bounds.width + bounds.x);
	r.height = (int)Mathf::Ceil(viewport->height * bounds.height + bounds.y);

	return r;
}

Matrix Camera::GetProjectionForNearFar(const float nearP, const float farP)
{
	Matrix p;
	UpdateProjectionMatrix(nearP, farP, p);
	return p;
}

const void Camera::SetAsSystemCamera(const int sytemType)
{
	systemCameraIndex = sytemType;
	systemCameras[systemCameraIndex] = this;
}

const int Camera::GetSystemCameraType() const
{
	return systemCameraIndex;
}

Camera* Camera::GetSystemCamera(const int systemType)
{
	return systemCameras[systemType];
}

void Camera::Start()
{
	CreateRenderPath();
}
