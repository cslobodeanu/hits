#ifndef __MORGANA_STANDARD_CAMERA_FPS_CAM_CONTROLLER_H__
#define __MORGANA_STANDARD_CAMERA_FPS_CAM_CONTROLLER_H__

#include "framework.h"

namespace MorganaEngine
{
	namespace Standard
	{
		namespace Cameras
		{
			class FpsCamController : public MEScript
			{
				__declare_morgana_script1(FpsCamController);
			public:
				FpsCamController();
				virtual ~FpsCamController();

				Scene::Cameras::Camera*		camera;
				_property<float>			moveSpeed, rotSpeed;

			protected:
				virtual void Update();
				virtual void DoAddProperties();

				Vector2						prevMousePos;
				Vector3						prevCameraPos;
			};
		}
	}
}

#endif