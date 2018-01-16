#ifndef __MORGANA_FMK_SCENE_CAMERAS_CAMERA_RENDER_H__
#define __MORGANA_FMK_SCENE_CAMERAS_CAMERA_RENDER_H__

#include "gameobjects.h"
#include "camera.h"
#include "camerafrontend.h"
#include "camerabackend.h"
#include "../../canvas/canvas.h"

namespace MorganaEngine
{
	namespace Framework
	{
		namespace Scene
		{
			namespace Cameras
			{
				class CameraRender
				{
				private:
					static Array<bool>			activeCamerasReady;
					static Array<CanvasList>	canvases;
					static CanvasList			overlayCanvases;

					static void					ResetRenderData();
					static void					CollectRenderData();
					static void					SendRenderData();

					static void					ProcessNode(GameObject* node);

					static void					RenderCanvas(CanvasSurface* cs);

				public:
					static Array<Camera*>			activeCameras;
					static Array<CameraFrontend*>	activeCamerasFrontends;
					static Array<CameraBackend*>	activeCamerasBackends;
					static void						RenderAllCameras();
				};
			}
		}
	}
}

#endif