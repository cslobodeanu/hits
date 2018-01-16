#include "renderpathforward.h"
#include "mescene.h"
#include "standardcamerabackend.h"
#include "shadowmappingbackend.h"

using namespace MorganaEngine::Framework::RenderPaths;

__implement_class(RenderPathForward, RenderPath);

RenderPathForward::RenderPathForward() : RenderPath()
{
	stdBackend = NULL;
	shadowMapCamera = NULL;
}

void RenderPathForward::Setup(Camera* cam)
{
	stdBackend = cam->gameObject->AddComponent<StandardCameraBackend>();
	
	shadowMapCamera = CreateSystemCameraWithBackend<ShadowMappingBackend>(cam, Camera::SystemCameraTypes::LightsDepthBuffer);
}

RenderPathForward::~RenderPathForward()
{
	SAFE_DEL(stdBackend);
	SAFE_DEL(shadowMapCamera);
}

#if 0
void RenderPathForward::OnFlush(RenderInfo_t& fd)
{
	const Matrix* csmMVP = GetEffect<CascadedShadowMaps>()->depthProjectionView.ptr();
	const Vector3* csmViewports = GetEffect<CascadedShadowMaps>()->splitViewports.ptr();
	const float* csmPlanes = GetEffect<CascadedShadowMaps>()->splitPlanes.ptr();
	Texture* depthTex = GetEffect<CascadedShadowMaps>()->depthTexture;
	const int numShadowSplits = GetEffect<CascadedShadowMaps>()->depthProjectionView.Length();

	for (int order = Renderer::FirstInQueue; order < Renderer::LastInQueue; order++)
	{
		RendererList& list = renderQueue[order];
		if (list.IsEmpty()) continue;

		for (int i = 0; i < list.Length(); i++)
		{
			Renderer* renderer = list[i];
			if(renderer->GetRenderablesCount() == 0) continue;

			const Material* mat = renderer->readMaterial();

			if (mat == NULL)
				continue;

			for (int k = 0; k < renderer->GetRenderablesCount(); k++)
			{
				const Renderer::Drawable_t& r = renderer->GetRenderable(k);
				const Matrix* customPVMat = renderer->GetCustomProjectionView();
				const Matrix* pvMat = customPVMat ? customPVMat : fd.camera->frustum.ProjectionViewPtr();

				bool hasPointLights = mat->shader->HasParameter("pLightsPosWS");

				if (hasPointLights)
				{
					//const int lc = Mathf::NextPow2(FillLightsInfo(renderer->bounds));
					//mat->UseDefinition(String("NUM_POINT_LIGHTS ") + lc);
					hasPointLights = false;// lc > 0;
				}

				SetMaterial(mat);

				if (depthTex != NULL)
					DisplayDevice::main->BindObject(depthTex, 5);

				ShaderProgram* prg = SetProgram(mat->shader);

				prg->SetMatrix("pvwMat", *pvMat);

				if (prg->HasParameter("shadowMat"))
				{
					prg->SetMatrices("shadowMat", csmMVP, numShadowSplits);

					if (prg->HasParameter("splitPlanes"))
						prg->SetFloats("splitPlanes", csmPlanes, numShadowSplits);

					if (prg->HasParameter("splitViewports"))
						prg->SetVectors("splitViewports", csmViewports, numShadowSplits);

					if (prg->HasParameter("poissonDisk"))
					{
						static const Vector2 vec2[4] = {
							Vector2(-0.94201624f, -0.39906216f),
							Vector2(0.94558609f, -0.76890725f),
							Vector2(-0.094184101f, -0.92938870f),
							Vector2(0.34495938f, 0.29387760f)
						};
						prg->SetVectors("poissonDisk", vec2, 4);
					}
				}

				if (prg->HasParameter("worldMat"))
				{
					prg->SetMatrix("worldMat", renderer->transform->GetWorldMatrix());
				}

				prg->SetFloat("shininess", 20.0f);

				if (hasPointLights)
				{
					/*FillLightsInfo(renderer->bounds);
					const int numPointLights = 8;
					prg->SetVectors("pLightsPosWS", lightsInfo.pointLightsWorldPositions, numPointLights);
					prg->SetVectors("pLightsColor", lightsInfo.pointLightsColors, numPointLights);
					prg->SetVectors("pLightsAtt", lightsInfo.pointLightsAttenuations, numPointLights);*/
				}

				DrawIndexed(r);
			}
		}
	}
}
#endif

