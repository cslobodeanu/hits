#ifndef __MORGANA_FMK_SCENE_CAMERAS_CAMERA_FRONTEND_H__
#define __MORGANA_FMK_SCENE_CAMERAS_CAMERA_FRONTEND_H__

#include "gameobjects.h"
#include "metypes.h"
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
				class CameraFrontend : public Component
				{
					__declare_component(CameraFrontend, Component);

				public:

					CameraFrontend();

					virtual void Awake();

					virtual void			PushRenderer(const Renderer* r);
					virtual void			PushLight(const Light* l);

					struct RenderableFrame_t
					{
						Array<RendererList>		renderQueue;
						Array<const Light*>		lights;
						const Light*			globalAmbientLight;
						const Light*			globalDirectionalLight;

						void Reset()
						{
							for (int i = 0; i < renderQueue.Length(); i++)
								renderQueue[i].SetLength(0);
							lights.Clear();
							globalDirectionalLight = NULL;
							globalAmbientLight = NULL;
						}
					};

					const RenderableFrame_t& GetFrame() const;

					const void				SetSharedFrontend(CameraFrontend* sharedFrontend);

				protected:

					virtual void			Start();
					virtual void			Update();

					Camera*					camera;

					CameraFrontend*			sharedFrontend;

					RenderableFrame_t		frame;
				};
			}
		}
	}
}

#endif