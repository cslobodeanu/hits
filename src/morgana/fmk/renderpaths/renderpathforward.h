#ifndef __MORGANA_FMK_RENDER_PATH_RENDER_PATH_FORWARD_H__
#define __MORGANA_FMK_RENDER_PATH_RENDER_PATH_FORWARD_H__

#include "renderpath.h"

namespace MorganaEngine
{
	namespace Framework
	{
		namespace RenderPaths
		{
			class RenderPathForward : public RenderPath
			{
				__declare_class(RenderPathForward, RenderPath);
			public:
				RenderPathForward();
				virtual ~RenderPathForward();

				virtual void	Setup(Camera* cam);
			private:

				CameraBackend*		stdBackend;
				Camera*				shadowMapCamera;
			};
		}
	}
}

#endif