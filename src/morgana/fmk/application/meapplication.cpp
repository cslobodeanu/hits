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

	GameObject* go = ObjectBuilder::CreateCamera("MainCamera");
	Camera* cam = go->GetComponent<Camera>();

	cam->projectionType = Camera::Perspective;
	cam->farPlane = 1500.0f;
	cam->nearPlane = 0.1f;
	cam->transform->position = Vector3(0, 10, 60);
	cam->backgroundColor = Color::dark_slate_gray;

	GameObject* canvas = ObjectBuilder::CreateCanvas();

	Desktop* d = GameObject::InitWithComponent<Desktop>();
	d->gameObject->SetParent(canvas);

	LoadConfiguration();

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

	HeapMemoryBlock::Release();

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

void MEApplication::LoadConfiguration()
{
	String path = Paths::ConcatPathWithFilename(Paths::GetSystemFilesPath(), "configuration.xml");

	singleton<MFiles> files;
	Stream* s = files->GetStream(path);

	LocalMemoryBlock lmb;
	s->ToNullTerminatedString(lmb);

	DEBUG_OUT("Loading configuration [%s] ...", s->location.c_str());

	if (lmb.size <= 0)
	{
		DEBUG_OUT("[1]Cannot open [%s].", s->location.c_str());
		return;
	}

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError err = doc.Parse(lmb);

	if (err != tinyxml2::XML_SUCCESS)
	{
		DEBUG_OUT("XML parse error (%d) for %s", err, s->location.c_str());
		return;
	}

	tinyxml2::XMLElement* rootElem = doc.FirstChildElement("configuration");
	if (rootElem == NULL)
	{
		ASSERT(false, "Not a configuration file.");
	}

	const char* atrstr = rootElem->Attribute("name");
	if (atrstr != null)
		DEBUG_OUT("Configuration is \"%s\"", atrstr);

	tinyxml2::XMLNode* startupscripts = rootElem->FirstChildElement("startupscripts");

	if (startupscripts != null)
	{
		tinyxml2::XMLElement* child = startupscripts->FirstChildElement();
		while (child != null)
		{
			if (strcmp(child->Name(), "script") == 0)
			{
				const char* scriptname = child->Attribute("name");
				DEBUG_OUT("Instantiate object with script \"%s\"", scriptname);

				if (child->BoolAttribute("canvas"))
				{
					GameObject* obj = ObjectBuilder::CreateCanvasObject(Desktop::main, scriptname);
					obj->AddScript(scriptname);
				}
			}

			child = child->NextSiblingElement();
		}
	}
}