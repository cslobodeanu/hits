#include "meapplication.h"
#include "framework.h"
#include "mestandard.h"

using namespace MorganaEngine::Framework::Application;
using namespace MorganaEngine::Framework::Utils;

__implement_class(MEApplication, MEObject);

MEApplication* MEApplication::main = NULL;

MEApplication::MEApplication()
{
	shouldClose = false;
	isInitialized = false;
	isShutdownComplete = false;
	mainView = NULL;

	if (main == NULL)
		main = this;
}

MEApplication::~MEApplication()
{
	if (main == this)
		main = NULL;
}

void MEApplication::Initialize()
{
	Time::Reset();
	GameObject::root = new GameObject();
	GameObject::root->SetName("GAMEOBJECTS_ROOT");

	OnInitialize();

	if (mainView)
		mainView->Show();

	METween::Initialize();
	Screen::Initialize();

	MEDevice::DeviceContext_t dc;
	dc.viewHandle = mainView->GetHandle();
	singleton<MEDevices>()->ConnectAllDevices(dc);

	DisplayDevice::main->BindObject(DisplayObjects::RenderTarget::Screen);

	Material::LoadDefaults();

	isInitialized = true;
}

void MEApplication::Run()
{
	if (isInitialized == false)
		Initialize();

	while(shouldClose == false || isShutdownComplete == false)
	{
		if (IsShuttingdown())
			Shutdown();

		if (IsShutdownComplete())
			return;

		FrameStats::Update();

		singleton<MEDevices>()->UpdateAllDevices();

		METimer::UpdateAll();
		METween::UpdateAll();

		Screen::Update();

		OnBeginFrame();

		GameObject::root->Update();

		GameObject::root->Render();

		RunFrame();
		
		CameraRender::RenderAllCameras();

		GameObject::root->LateUpdate();

		OnEndFrame();

		DisplayDevice::main->PresentBuffer();

		Time::Tick();
		FrameStats::Reset();
	}
}

void MEApplication::RunFrame()
{
	OnRunFrame();
}

void MEApplication::Shutdown()
{
	if (isShutdownComplete) return;

	OnShutdown();

	METween::Destroy();
	METimer::Destroy();

	singleton<MFiles>()->Cleanup();
	singleton<MEDevices>()->DisconnectAllDevices();

	DEBUG_OUT("Deleting resources ...");
	ResourceContext::global->Free();

	DEBUG_OUT("Deleting scene tree ...");
	SAFE_DEL(GameObject::root);

	shouldClose = true;
	isShutdownComplete = true;

	DEBUG_OUT("Should exit now ...");
}

String MEApplication::GetDescription()
{
	return String("MorganaEngine Application");
}

void MEApplication::NotifyShutdown()
{
	shouldClose = true;
}

const bool MEApplication::IsShuttingdown() const
{
	return shouldClose && isShutdownComplete == false;
}

const bool MEApplication::IsShutdownComplete() const
{
	return shouldClose && isShutdownComplete;
}

void MEApplication::CallCreateGameObject(GameObject* obj)
{
	OnCreateGameObject(obj);
}

void MEApplication::CallDeleteGameObject(GameObject* obj)
{
	OnDeleteGameObject(obj);
}

void MEApplication::CallApplicationWait()
{
	OnApplicationWait();
}