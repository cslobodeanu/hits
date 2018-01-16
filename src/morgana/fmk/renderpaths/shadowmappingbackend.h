#ifndef __MORGANA_FMK_RENDER_PATHS_SHADOW_MAPPING_BACKEND_H__
#define __MORGANA_FMK_RENDER_PATHS_SHADOW_MAPPING_BACKEND_H__

#include "../cameras/camerabackend.h"

namespace MorganaEngine
{
	namespace Framework
	{
		using namespace Scene::Cameras;
		namespace RenderPaths
		{
			class ShadowMappingBackend : public CameraBackend
			{
				__declare_class(ShadowMappingBackend, CameraBackend);

			public:

				ShadowMappingBackend();
				virtual ~ShadowMappingBackend();

				_property<float>	firstSplitDistance;
				_property<float>	secondSplitDistance;

			protected:

				virtual void		DoAddProperties();
				virtual void		OnPropertySet(_propertyDef* atr);

				virtual void		Start();
				virtual void		Update();

				virtual void		OnPreFlush(const CameraFrontend::RenderableFrame_t& frame);
				virtual void		OnFlush(const CameraFrontend::RenderableFrame_t& frame);

				void				Render();

				virtual void		UploadGlobalShaderData(ShaderProgram* shader);
				virtual const bool	IsUsingShader(ShaderProgram* shader) const;

				RendererList		renderersInSplits[3];
				void				UpdateRenderersInSplits();

				float				splitPlanes[3];
				void				GenerateSplitPlanes();
				Vector3				splitViewports[3];
				void				GenerateSplitCoords();

				Frustum				splitFrustums[3];
				void				UpdateSplitFrustums();

				Matrix				splitProjectionViewMatrices[3];

				Matrix				ComputeCropMatrix(const Frustum& splitFrustum, const Matrix& lightProj, const Matrix& lightView);

				Material*			writeDepthMaterial;
			};
		}
	}
}

#endif