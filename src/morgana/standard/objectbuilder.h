#ifndef __MORGANA_STANDARD_OBJECT_BUILDER_H__
#define __MORGANA_STANDARD_OBJECT_BUILDER_H__

#include "gameobjects.h"
#include "mescene.h"
#include "renderers.h"
#include "../scene/meshes/meshes.h"
#include "../canvas/canvas.h"
#include "camera/fpscamcontroller.h"

namespace MorganaEngine
{
	namespace Standard
	{
		using namespace Cameras;
		class ObjectBuilder
		{
			static GameObject* AssureCanvasParent(GameObject* parent)
			{
				GameObject* p = parent;
				if (parent->GetComponentInParent<CanvasSurface>() == NULL && parent->GetComponent<CanvasSurface>() == NULL)
				{
					p = CreateCanvas();
					p->SetParent(parent);
				}
				return p;
			}

		public:
			static GameObject* CreateCamera(const char* name = NULL)
			{
				String nm = name;
				if (GameObject::root->GetComponentInChildren<Camera>() == NULL)
				{
					nm = "MainCamera";
				}

				GameObject* go = new GameObject();
				go->AddComponent<Camera>();
				go->SetName(nm);

				go->SetName(name);

				return go;
			}

			static GameObject* CreateMeshObject(const char* name = NULL, const MeshData* src = NULL)
			{
				GameObject* go = new GameObject();
				MeshComponent* mc = go->AddComponent<MeshComponent>();
				if (src != NULL)
					mc->mesh = singleton<MeshCollection>()->Import(src, name);
				go->AddComponent<MeshRenderer>();

				go->SetName(name);

				return go;
			}

			static GameObject* CreateLight(int type, const Color& c = Color::white, const char* name = NULL)
			{
				GameObject* go = new GameObject();
				Light* l = go->AddComponent<Light>();
				l->color = c;
				l->type = type;

				go->SetName(name);

				return go;
			}

			static GameObject* CreateCanvasObject(Component* parent, const char* name = NULL)
			{
				return CreateCanvasObject(parent->gameObject, name);
			}

			static GameObject* CreateCanvasObject(GameObject* parent, const char* name = NULL)
			{
				GameObject* p = AssureCanvasParent(parent);
				GameObject* go = new GameObject();
				go->AddComponent<RectTransform>();
				go->SetName(name);
				go->SetParent(p);
				return go;
			}

			static GameObject* CreateCanvas(const char* name = "Canvas")
			{
				GameObject* go = new GameObject();
				go->AddComponent<CanvasSurface>();
				go->AddComponent<InputEventsManager>();
				go->SetName(name);
				return go;
			}

			static GameObject* CreateCanvasImage(Component* parent, const Texture* tex, const char* name = NULL)
			{
				return CreateCanvasImage(parent->gameObject, tex, name);
			}

			static GameObject* CreateCanvasImage(GameObject* parent, const Texture* tex, const char* name = NULL)
			{
				GameObject* go = CreateCanvasObject(parent, name);
				Image* img = go->AddComponent<Image>();
				img->texture = tex;
					
				go->SetName(name);

				return go;
			}

			static GameObject* CreateCanvasText(Component* parent, const char* text)
			{
				return CreateCanvasText(parent->gameObject, text);
			}

			static GameObject* CreateCanvasText(GameObject* parent, const char* text)
			{
				GameObject* go = CreateCanvasObject(parent);
				Text* txt = go->AddComponent<Text>();
				txt->text = text;
					
				String name = text;
				go->SetName(name);

				return go;
			}

			static GameObject*	CreateFPSCameraController()
			{
				GameObject* go = new GameObject();
				go->AddComponent<FpsCamController>();
				go->SetName("FpsCamController");
				return go;
			}
		};
	}
}

#endif