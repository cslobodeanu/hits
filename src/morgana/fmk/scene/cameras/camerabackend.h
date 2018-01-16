#ifndef __MORGANA_FMK_SCENE_CAMERAS_CAMERA_BACKEND_H__
#define __MORGANA_FMK_SCENE_CAMERAS_CAMERA_BACKEND_H__

#include "gameobjects.h"
#include "camerafrontend.h"

namespace MorganaEngine
{
	namespace Framework
	{
		namespace Scene
		{
			namespace Cameras
			{
				class CameraBackend : public Component
				{
					__declare_component(CameraBackend, Component);
				public:
					CameraBackend();
					virtual ~CameraBackend();

					void	Flush();

					const void	DependsOn(Camera* cam);

				protected:

					virtual void Start();
					virtual void Update();

					virtual void			OnPreFlush(const CameraFrontend::RenderableFrame_t& frame);
					virtual void			OnFlush(const CameraFrontend::RenderableFrame_t& frame);

					CameraFrontend*			frontend;
					Camera*					camera;
					Camera*					dependsOnCamera;

					Array<ShaderProgram*>	usedShaders;
					void					MakeShaderList();
					void					OnResourceLoaded(MEObject*);
					virtual const bool		IsUsingShader(ShaderProgram* shader) const;
					virtual void			UploadGlobalShaderData(ShaderProgram* shader);
					virtual void			UploadGlobalShaderData();
				};
			}
		}
	}
}

#endif