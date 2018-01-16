#ifndef __MORGANA_FMK_RENDER_PATHS_RENDER_PATH_H__
#define __MORGANA_FMK_RENDER_PATHS_RENDER_PATH_H__

#include "reflection.h"

#include "../scene/meshes/mesh.h"
#include "melights.h"
#include "meresources.h"
#include "renderers.h"

namespace MorganaEngine
{
	namespace Framework
	{
		namespace Scene
		{
			namespace Cameras
			{
				class Camera;
				class CameraFrontend;
				class CameraBackend;
			}
		}

		using namespace Scene::Cameras;

		namespace RenderPaths
		{
			class RenderPath : public MEObject
			{
				__declare_class_abstract(RenderPath, MEObject);
			public:
				RenderPath();
				virtual ~RenderPath();

				static void				DrawIndexed(const Renderer::Drawable_t& r);
				static void				SetMaterial(const Material* mat);
				static ShaderProgram*	SetProgram(ShaderProgram* prg);

				virtual void			Setup(Camera* cam);

			protected:

				static ShaderProgram*	invalidProgram;

				template<class backendT>
				Camera*					CreateSystemCameraWithBackend(Camera* parent, const int systemIndex)
				{
					GameObject* c = new GameObject();
					c->SetParent(GameObject::_systemRoot());
					String name = Camera::SystemCameraTypes::ToString(systemIndex);

					c->SetName(String::Format("SystemCamera (%s)", name.c_str()));
					Camera* cam = c->AddComponent<Camera>();
					cam->SetAsSystemCamera(systemIndex);
					cam->gameObject->AddComponent<backendT>()->DependsOn(parent);
					cam->depth = -80000.0f + (systemIndex);

					return cam;
				}
			};

		}
	}
}

#endif