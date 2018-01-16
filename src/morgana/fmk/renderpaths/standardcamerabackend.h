#ifndef __MORGANA_FMK_RENDER_PATHS_STANDARD_CAMERA_BACKEND_H__
#define __MORGANA_FMK_RENDER_PATHS_STANDARD_CAMERA_BACKEND_H__

#include "../cameras/camerabackend.h"

namespace MorganaEngine
{
	namespace Framework
	{
		using namespace Scene::Cameras;
		namespace RenderPaths
		{
			class StandardCameraBackend : public CameraBackend
			{
				__declare_class(StandardCameraBackend, CameraBackend);

			public:
				StandardCameraBackend();

			protected:
				virtual void		OnFlush(const CameraFrontend::RenderableFrame_t& frame);

				virtual void		UploadGlobalShaderData(ShaderProgram* shader);
				virtual const bool	IsUsingShader(ShaderProgram* shader) const;
			};
		}
	}
}

#endif