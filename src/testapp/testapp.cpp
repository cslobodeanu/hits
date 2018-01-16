#include "testapp.h"
#include "mestandard.h"

__implement_class(TestApplication, MEApplication);

TestApplication::TestApplication()
{
}

TestApplication::~TestApplication()
{

}

void TestApplication::OnImageClick(Component* sender, Vector2 p, bool& ret)
{
	GameObject* par = sender->gameObject->GetParent()->GetParent();
	par->BringToFront();
}

void TestApplication::Initialize()
{
	super::Initialize();

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

	ObjectBuilder::CreateFPSCameraController();

	GameObject* obj = ObjectBuilder::CreateCanvasObject(Desktop::main, "DisplayFPS");
	DisplayFPS* fps = obj->AddComponent<DisplayFPS>();



	for (int i = 0; i < 2; i++)
	{
		StdWindow* wnd = GameObject::InitWithComponent<StdWindow>();
		wnd->gameObject->SetParent(Desktop::main->gameObject);
		RectTransform* rt = wnd->GetComponent<RectTransform>();
		rt->rect = Rectf(100.0f + i * 50.0f, 100.0f + i * 80.0f, Mathf::Rand01() * 320.0f + 100.0f, Mathf::Rand01() * 240.0f + 100.0f);

		wnd->caption = String::Format("Window #%d", i + 1);
		wnd->gameObject->SetName(wnd->caption->c_str());

		wnd->sizeable = true;
		wnd->movable = true;


#if 0
		StdWindow* wnd2 = GameObject::InitWithComponent<StdWindow>();
		wnd2->gameObject->SetParent(wnd->gameObject);
		rt = wnd2->GetComponent<RectTransform>();

		Rectf rr = wnd->GetComponent<RectTransform>()->rect->ToOrigin();
		rr.Inflate(-20.0f, -60.0f);
		rr.Offset(4.0f, 20.0f);
		rt->rect = rr;
		

		wnd2->caption = String::Format("SubWindow #%d", i + 1);
		wnd2->gameObject->SetName(wnd2->caption->c_str());

		wnd2->movable = true;
#endif

		StdTreeView* std = GameObject::InitWithComponent<StdTreeView>();

		rt = std->GetComponent<RectTransform>();
		std->gameObject->SetParent(wnd->gameObject);
		std->SetObject((TreeLeaf*)GameObject::root);
		std->GetComponent<RectTransform>()->rect = Rectf(20.0f, 50.0f, 100.0f, 100.0f);
	}
}

void TestApplication::RunFrame()
{
	super::RunFrame();

#if 1
	if (Input::GetKeyDown(Keys::Left) || Input::GetKeyDown(Keys::Right))
	{
		GameObject* dl = GameObject::root->Find("directionalL");
		Vector3 angles = dl->transform->rotation->ToEuler();
		if (Input::GetKeyDown(Keys::Left))
			angles.y += Time::deltaTime * 30.0f;
		if (Input::GetKeyDown(Keys::Right))
			angles.y -= Time::deltaTime * 30.0f;

		dl->transform->rotation = Quaternion::FromEuler(angles);
	}
#endif

#if 1 // test objects
	if (GameObject::root->Find("Dummy") == NULL)
	{
		GameObject* dummy = new GameObject();
		dummy->SetName("Dummy");
		dummy->SetParent(GameObject::root);

		MeshData* sm = Primitive::GeneratePolygon<VertexPosNormalTex>(32);
		Matrix tm = Matrix::Identity;
		tm.Scale(5.0f);
		tm.Translation() = Vector3(0.0f, 0.0f, -10.0f);
		Primitive::Transform(sm, &tm);
		Vector3 up(0.0f, 0.0f, 20.0f);
		MeshData* sm2 = Primitive::ExtrudePolygon(sm, up, 2);
		Primitive::AppendMesh(sm2, sm);
		tm = Matrix::Identity;
		tm.Translate(up);
		Primitive::AppendMesh(sm2, sm, &tm);
		Primitive::SetVertexColors(sm2, Color::green);

		/*GameObject* test = ObjectBuilder::CreateMeshObject("generated hexagon", sm2);
		test->SetParent(dummy);
		test->SetName("TestCylinder");
		test->AddComponent<MaterialComponent>()->material = ResourceContext::global->Load<Material>("perpixelphong");*/

		// random cubes
		for (int k = 0; k < 0; k++)
		{
			int edges = (int)Mathf::Rand(4.0f, 19.0f);
			sm = Primitive::GeneratePolygon<VertexPosNormalTex>(edges);
			Matrix tm = Matrix::Identity;

			const float cubeSize = Mathf::Rand(4.5f, 8.0f) * 0.5f;
			const float cubeSize2 = cubeSize * Mathf::Sqrt2;

			tm.RotateX(90.0f);
			tm.Scale(cubeSize);

			tm.Translation() = Vector3(0.0f, 0.0f, -cubeSize2 * 0.5f);
			Primitive::Transform(sm, &tm);
			Vector3 up(0.0f, cubeSize2 * Mathf::Rand(7.0f, 20.0f), 0.0f);
			sm2 = Primitive::ExtrudePolygon(sm, up, 2);

			Primitive::AppendMesh(sm2, sm);
			tm = Matrix::Identity;
			tm.Translate(up);
			Primitive::AppendMesh(sm2, sm, &tm);
			Primitive::SetVertexColors(sm2, Color::green);

			GameObject* test = ObjectBuilder::CreateMeshObject(String("generated cube") + k, sm2);
			test->SetParent(dummy);
			test->SetName(String::Format("Cube %d", k));

			tm = Matrix::Identity;
			tm.RotateY(Mathf::Rand01() * 360.0f);
			tm.Translate(Vector3::UnitZ * Mathf::Rand01() * 200.0f);// +Vector3::UnitY * cubeSize2);

			test->transform->position = tm.Translation();
			//test->transform->rotation.RotateX(Mathf::Rand01() * 90.0f);

			Quaternion q = test->transform->rotation;
			q.RotateY(Mathf::Rand01() * 90.0f);
			test->transform->rotation = q;
			//test->transform->rotation.RotateZ(Mathf::Rand01() * 90.0f);
			test->AddComponent<MaterialComponent>()->material = ResourceContext::global->Load<Material>("perpixelphong");
		}



		sm = Primitive::GeneratePolygon<VertexPosNormalTex>(4);
		tm = Matrix::Identity;
		tm.Scale(300.0f);
		tm.RotateX(-90.0f);
		Primitive::Transform(sm, &tm);

		GameObject* test2 = ObjectBuilder::CreateMeshObject("generated square", sm);
		test2->SetParent(dummy);
		test2->SetName("TestSquare");
		test2->AddComponent<MaterialComponent>()->material = ResourceContext::global->Load<Material>("perpixelphong");
		test2->GetComponent<Renderer>()->castShadows = false;


		METimer* testTimer = new METimer(1.0f / 60.0f);
		testTimer->SetName("testTimer");

		testTimer->OnTick += Callback(OnTimer, METimer*, float);

#if 1
		GameObject* al = ObjectBuilder::CreateLight(Light::AmbientLight, Color::gray, "ambient");
		al->SetParent(GameObject::root);

		GameObject* dl = ObjectBuilder::CreateLight(Light::DirectionalLight, Color::orange, "directionalL");
		dl->SetParent(GameObject::root);

		Quaternion q = dl->transform->rotation; q.RotateX(-50.0f);
		dl->transform->rotation = q;

		const int numLights = 8;
		for (int i = 0; i < numLights; i++)
		{
			String ln = String("pointlight") + i;

			Color c = ColorHSB::RandomHue();
			GameObject* pl = ObjectBuilder::CreateLight(Light::PointLight, c, ln);
			pl->SetParent(GameObject::root);

			pl->transform->localRotation = Quaternion::Identity;

			const float dist = Mathf::Rand(3.0f, 5.0f) * 40.0f;
			Vector3 v = Vector3(0.0f, 0.0f, dist);
			pl->transform->localPosition = pl->transform->localRotation * v;

			pl->GetComponent<Light>()->attenuation = Vector3(1.0f, 0.03f, 0.0f);//0.032f + Mathf::Rand11() * 0.008f);

			float* ff = new float[2];
			ff[0] = Mathf::Sign(Mathf::Rand11()) * Mathf::Rand(100.0f, 180.0f);
			ff[1] = dist;
			pl->SetUserData(ff);
		}
#endif
	}
#endif
}

void TestApplication::OnTimer(METimer* t, const float overShootFactor)
{
	GameObject* node = GameObject::root->Find("TestCylinder");
	if (node)
	{
		node->transform->localPosition = Vector3(0.0f, 10.0f, 0.0f);

		Quaternion q = node->transform->localRotation;
		q.RotateX(2.0f);
		q.RotateZ(4.0f);
		node->transform->localRotation = q;
		node->transform->localRotation;
	}

	node = GameObject::root->Find("TestSquare");
	if (node)
	{
	}

#if 0
	Array<Light*> lights = GameObject::root->GetComponentsInChildren<Light>();
	for (int i = 0; i < lights.Length(); i++)
	{
		Light* pl = lights[i];
		if (pl->type != Light::PointLight) continue;

		pl->transform->localPosition = Vector3::Zero;
		const float ang = ((float*)pl->gameObject->GetUserData())[0];
		const float dist = ((float*)pl->gameObject->GetUserData())[1];

		Quaternion q = pl->transform->localRotation;
		q.RotateY(ang * Time::deltaTime);
		pl->transform->localRotation = q;
		pl->transform->localPosition = pl->transform->localRotation * Vector3(0.0f, 20.0f, dist);

		//pl->m_vAttenuation = Vector3(0.0f, 0.027f, 0.0028f + MathUtils::Rand11() * 0.001f);
	}
#endif
}
