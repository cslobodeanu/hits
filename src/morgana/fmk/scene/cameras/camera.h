#ifndef __MORGANA_FMK_SCENE_CAMERAS_CAMERA_H__
#define __MORGANA_FMK_SCENE_CAMERAS_CAMERA_H__

#include "gameobjects.h"
#include "../../renderpaths/renderpath.h"

namespace MorganaEngine
{
	namespace Framework
	{
		namespace Scene
		{
			using namespace RenderPaths;
			namespace Cameras
			{
				class Camera : public Component
				{
					__declare_component(Camera, Component);

				public:
					Camera();
					virtual ~Camera();

					enum ProjectionTypes_t
					{
						Parallel,
						Perspective,
						PerspectiveInfinite
					};

					enum ClearFlags_t
					{
						ClearEverything,
						ClearNothing,
						ClearDepth,
						ClearColor
					};

					_property<float>	fieldOfView, nearPlane, farPlane;
					_property<int>		projectionType;
					_property<float>	orthographicSize;

					_property<Rectf>	viewport;
					_property<int>		clearFlags;
					_property<float>	depth;

					_property<Color>	backgroundColor;

					NamedMask			renderMask;
					Array<String>		renderQueue;

					RenderTarget*		target;
					CLASSDESC	renderPathType;

					Frustum				frustum;

					const Matrix&		GetProjectionMatrix() const;

					Matrix				GetProjectionForNearFar(const float nearP, const float farP);

					static Camera*		main;

					virtual void		Awake();

					make_enum(SystemCameraTypes,
						Unused,
						LightsDepthBuffer,
						RenderScreenSpaceNormals,
						RenderScreenSpacePositions,
						SystemCamerasCount
					);

					const void			SetAsSystemCamera(const int sytemType);
					const int			GetSystemCameraType() const;
					static Camera*		GetSystemCamera(const int systemType);

				protected:

					void				DoAddProperties();
					virtual void		OnPropertySet(_propertyDef* p);

					virtual void		Start();
					virtual void		Update();

					Matrix				projectionMatrix;

					void				UpdateProjectionMatrix(const float nearP, const float farP, Matrix& result);

					int					systemCameraIndex;
					static Camera*		systemCameras[SystemCameraTypes::SystemCamerasCount];


					friend class CameraRender;

					static Array<Camera*>	allCameras;

					static void			AddCamera(Camera* c);
					static void			RemoveCamera(Camera* c);

					void				SetupBackground();

					RenderPath*			renderPath;
					void				CreateRenderPath();

					GameObject*			nodeToRender;

					Rect				NormalizedViewport(RenderTarget* rt);
				};
			}
		}
	}
}

#endif